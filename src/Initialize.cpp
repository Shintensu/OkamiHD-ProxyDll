#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "MinHook.h"

#include "MathStructs.h"

extern math::Vec3* detourCopyVec3(math::Vec3* vector1, math::Vec3* vector2);
extern long __stdcall detour_present(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags);

typedef float* (__cdecl* CopyVec3)(float*, float*);
typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);

extern present p_present;
extern present p_present_target;


uintptr_t mainModuleBase;
uintptr_t flowerKernelModuleBase;

math::Vec2* pitchAndYawDistant;
math::Vec2* pitchAndYaw;

float* fov;
void* pVec3 = nullptr;

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
	pitchAndYawDistant = (math::Vec2*)(mainModuleBase + 0xB66390);
	pitchAndYaw = (math::Vec2*)(mainModuleBase + 0xB6659C);
	fov = (float*)(mainModuleBase + 0xB663B0);

	// create detour hook for the CopyVec3 function in Okami
	uintptr_t addressOfFunction = (flowerKernelModuleBase + 0x1A9D0);

	CopyVec3 pVec3target = reinterpret_cast<CopyVec3>(addressOfFunction);

	if (MH_CreateHook(reinterpret_cast<void**>(addressOfFunction), &detourCopyVec3, reinterpret_cast<void**>(&pVec3)) != MH_OK) {
		return 1;
	}

	//enable present function detour
	if (MH_EnableHook(p_present_target) != MH_OK) {
		return 1;
	}
}
