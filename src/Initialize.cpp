#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "MinHook.h"

#include "GameStructs.h"

#include "main.h"
#include "Initialize.h"
#include "Detours.h"

#include "wk.h"

uintptr_t mainModuleBase;
uintptr_t flowerKernelModuleBase;

CopyVec3 pVec3 = nullptr;
PlayerMove pPlayerMove = nullptr;
PlayerGetInput pPlayerGetInput = nullptr;
PlayerConstructor pPlayerConstructor = nullptr;
ObjectConstructor pObjectConstructor = nullptr;
cModelAnimationUpdate pcModelAnimationUpdate = nullptr;

uintptr_t copyVec3Address;
uintptr_t playerMoveAddress;
uintptr_t playerGetInputAddress;
uintptr_t playerConstructorAddress;
uintptr_t objectConstructorAdress;
uintptr_t cModelAnimationUpdateAddress;

CopyVec3 pVec3target;
PlayerMove pPlayerMovetarget;
PlayerGetInput pPlayerGetInputtarget;
PlayerConstructor pPlayerConstructortarget;
ObjectConstructor pObjectConstructorTarget;
cModelAnimationUpdate pcModelAnimationUpdateTarget;

float* fov;
wk::math::cVec* pitchAndYawDistant;
wk::math::cVec* pitchAndYaw;

// somewhat self explanatory in what it does, if you have any questions about MinHook, there is plenty of explanations on youtube
int Initialize()
{
	// initialize minhook
	if (MH_Initialize())
	{
		return 1;
	}

	// create detour hook for the present function
	if (MH_CreateHook(reinterpret_cast<void**>(p_present_target), &detour_present, reinterpret_cast<void**>(&p_present)) != MH_OK) {
		return 1;
	}

	// wait until flower_kernel.dll is loaded
	while (!GetModuleHandle(L"flower_kernel.dll"))
	{
		Sleep(50);
	}

	// get module base addresses
	mainModuleBase = (uintptr_t)GetModuleHandle(L"main.dll");
	flowerKernelModuleBase = (uintptr_t)GetModuleHandle(L"flower_kernel.dll");

	// get pitch, yaw and fov pointers
	pitchAndYawDistant = (wk::math::cVec*)(mainModuleBase + 0xB66390);
	pitchAndYaw = (wk::math::cVec*)(mainModuleBase + 0xB6659C);
	fov = (float*)(mainModuleBase + 0xB663B0);

	// create detour hook for the CopyVec3 function in Okami
	copyVec3Address = (flowerKernelModuleBase + 0x1A9D0);
	pVec3target = reinterpret_cast<CopyVec3>(copyVec3Address);
	if (MH_CreateHook(reinterpret_cast<void**>(copyVec3Address), &detourCopyVec3, reinterpret_cast<void**>(&pVec3)) != MH_OK) {
		return 1;
	}

	// create detour hook for the PlayerMove function in Okami
	playerMoveAddress = (mainModuleBase + 0x3AF020);
	pPlayerMovetarget = reinterpret_cast<PlayerMove>(playerMoveAddress);
	if (MH_CreateHook(reinterpret_cast<void**>(playerMoveAddress), &detourPlayerMove, reinterpret_cast<void**>(&pPlayerMove)) != MH_OK) {
		return 1;
	}

	// create detour hook for the PlayerGetInput function in Okami
	playerGetInputAddress = (mainModuleBase + 0x3ACF90);
	pPlayerGetInputtarget = reinterpret_cast<PlayerGetInput>(playerGetInputAddress);
	if (MH_CreateHook(reinterpret_cast<void**>(playerGetInputAddress), &detourPlayerGetInput, reinterpret_cast<void**>(&pPlayerGetInput)) != MH_OK) {
		return 1;
	}

	// create detour hook for the PlayerConstructor in Okami
	playerConstructorAddress = (mainModuleBase + 0x3D2EC0);
	pPlayerConstructortarget = reinterpret_cast<PlayerConstructor>(playerConstructorAddress);
	if (MH_CreateHook(reinterpret_cast<void**>(playerConstructorAddress), &detourPlayerConstructor, reinterpret_cast<void**>(&pPlayerConstructor)) != MH_OK) {
		return 1;
	}

	// create detour hook for the ObjectConstructor in Okami
	objectConstructorAdress = (mainModuleBase + 0x359DE0);
	pObjectConstructorTarget = reinterpret_cast<ObjectConstructor>(objectConstructorAdress);
	if (MH_CreateHook(reinterpret_cast<void**>(objectConstructorAdress), &detourObjectConstructor, reinterpret_cast<void**>(&pObjectConstructor)) != MH_OK) {
		return 1;
	}

	// create detour hook for the cModel::AnimatioUpdate(Not actually, only the transposing of the matrix) in Okami
	cModelAnimationUpdateAddress = (flowerKernelModuleBase + 0x2a86);
	pcModelAnimationUpdateTarget = reinterpret_cast<cModelAnimationUpdate>(cModelAnimationUpdateAddress);
	if (MH_CreateHook(reinterpret_cast<void**>(cModelAnimationUpdateAddress), &detourcModelUpdateAnimation, reinterpret_cast<void**>(&pcModelAnimationUpdate)) != MH_OK) {
		return 1;
	}

	//enable present function detour
	if (MH_EnableHook(p_present_target) != MH_OK) {
		return 1;
	}
}
