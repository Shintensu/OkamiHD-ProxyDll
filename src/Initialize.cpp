#include "pch.h"

#include "main.h"

#include "Initialize.h"
#include "Detours.h"

#include "BaseFunctionHook.h"


#include "wk.h"

int* entityHeapExpansionSize = nullptr;

bool* useUnknownMode = nullptr;
bool* useEncryption = nullptr;

short* encryptionJmpInstruction1 = nullptr;
short* encryptionJmpInstruction2 = nullptr;

BaseFunctionHook<present>* presentHook;

BaseFunctionHook<CopyVec3>* copyVec3Hook;
PlayerMovementUpdateHook* playerMoveHook;
BaseFunctionHook<PlayerGetInput>* playerGetInputHook;
BaseFunctionHook<PlayerConstructor>* playerConstructorHook;
BaseFunctionHook<ObjectConstructor>* objectConstructorHook;
BaseFunctionHook<cModelAnimationUpdate>* cModelAnimationUpdateHook;
BaseFunctionHook<cpadUpdate>* cpadHook;

float* fov;
wk::math::cVec* pitchAndYawDistant;
wk::math::cVec* pitchAndYaw;


// somewhat self explanatory in what it does, if you have any questions about MinHook, there is plenty of explanations on youtube
int Initialize()
{
	while (!(uintptr_t)GetModuleHandle(L"main.dll"))
	{

	}

	// get module base addresses
	mainModuleBase = (uintptr_t)GetModuleHandle(L"main.dll");
	flowerKernelModuleBase = (uintptr_t)GetModuleHandle(L"flower_kernel.dll");

	//Expand Player Heap
	entityHeapExpansionSize = (int*)(mainModuleBase + 0x12CAC5);
	DWORD oldprotect;
	VirtualProtect(entityHeapExpansionSize, 4, PAGE_EXECUTE_READWRITE, &oldprotect);
	*entityHeapExpansionSize = 0x90000000; // Default is  0x00200000
	VirtualProtect(entityHeapExpansionSize, 4, oldprotect, &oldprotect);

	//Set encryption mode
	useEncryption = (bool*)(mainModuleBase + 0x15269D);
	VirtualProtect(useEncryption, 4, PAGE_EXECUTE_READWRITE, &oldprotect);
	*useEncryption = 0; // Default is 1
	VirtualProtect(useEncryption, 4, oldprotect, &oldprotect);

	if (!*useEncryption)
	{
		encryptionJmpInstruction1 = (short*)(mainModuleBase + 0x4B5C6D);

		VirtualProtect(encryptionJmpInstruction1, 2, PAGE_EXECUTE_READWRITE, &oldprotect);
		*encryptionJmpInstruction1 = 0x9090; // Default is  0x0C7F
		VirtualProtect(encryptionJmpInstruction1, 2, oldprotect, &oldprotect);

		encryptionJmpInstruction2 = (short*)(mainModuleBase + 0x4B5FA4);

		VirtualProtect(encryptionJmpInstruction2, 2, PAGE_EXECUTE_READWRITE, &oldprotect);
		*encryptionJmpInstruction2 = 0x9090; // Default is  0x0C7F
		VirtualProtect(encryptionJmpInstruction2, 2, oldprotect, &oldprotect);
	}

	// get pitch, yaw and fov pointers
	pitchAndYawDistant = (wk::math::cVec*)(mainModuleBase + 0xB66390);
	pitchAndYaw = (wk::math::cVec*)(mainModuleBase + 0xB6659C);
	fov = (float*)(mainModuleBase + 0xB663B0);

	while (!FindWindowExW(0, 0, 0, L"ŌKAMI HD"))
	{
		Sleep(50);
	}

	if (!get_present_pointer())
	{
		return 1;
	}

	// initialize minhook
	if (MH_Initialize())
	{
		return 1;
	}

	// create detour hook for present
	presentHook = new BaseFunctionHook<present>(p_present_target, &detour_present);
	if (presentHook->CreateHook() || presentHook->EnableHook())
		return 1;

	// create detour hook for the CopyVec3 function in Okami
	copyVec3Hook = new BaseFunctionHook<CopyVec3>(reinterpret_cast<CopyVec3>(flowerKernelModuleBase + 0x1A9D0), &detourCopyVec3);
	if (copyVec3Hook->CreateHook())
		return 1;

	// create detour hook for the PlayerMovementUpdate function in Okami
	playerMoveHook = new PlayerMovementUpdateHook();
	if (playerMoveHook->CreateHook())
		return 1;

	// create detour hook for the PlayerGetInput function in Okami
	playerGetInputHook = new BaseFunctionHook<PlayerGetInput>(reinterpret_cast<PlayerGetInput>(mainModuleBase + 0x3ACF90), &detourPlayerGetInput);
	if (playerGetInputHook->CreateHook())
		return 1;

	// create detour hook for the PlayerConstructor in Okami
	playerConstructorHook = new BaseFunctionHook<PlayerConstructor>(reinterpret_cast<PlayerConstructor>(mainModuleBase + 0x3D2EC0), &detourPlayerConstructor);
	if (playerConstructorHook->CreateHook())
		return 1;

	// create detour hook for the ObjectConstructor in Okami
	objectConstructorHook = new BaseFunctionHook<ObjectConstructor>(reinterpret_cast<ObjectConstructor>(mainModuleBase + 0x359DE0), &detourObjectConstructor);
	if (objectConstructorHook->CreateHook())
		return 1;

	// create detour hook for the cModel::AnimatioUpdate(Not actually, only the transposing of the matrix) in Okami
	cModelAnimationUpdateHook = new BaseFunctionHook<cModelAnimationUpdate>(reinterpret_cast<cModelAnimationUpdate>(flowerKernelModuleBase + 0x2a86), &detourcModelUpdateAnimation);
	if (cModelAnimationUpdateHook->CreateHook())
		return 1;

	// create detour hook for cpad::Update
	cpadHook = new BaseFunctionHook<cpadUpdate>(reinterpret_cast<cpadUpdate>(mainModuleBase + 0x186650), &detourCpadUpdate);
	if (cpadHook->CreateHook() || cpadHook->EnableHook())
		return 1;
}
