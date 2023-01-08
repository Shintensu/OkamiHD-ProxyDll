#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>
#include <cstdint>

#include <iostream>

#include <d3d11.h>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "MinHook.h"

#include "Overlay.h"

#define PI 3.14159265359f

// Globals
HINSTANCE dll_handle;

typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);
present p_present;
present p_present_target;

int cursorPos[2];

void* pVec3 = nullptr;

struct Vec2
{
	float x, y;
};
struct Vec3 
{
	Vec3(float initX, float initY, float initZ)
		: x(initX), y(initY), z(initZ)
	{

	}
	float x, y, z;
};

//float* CopyVec3(float* vector1, float* vector2)
//{
//	vector1[0] = vector2[0];
//	vector1[1] = vector2[1];
//	vector1[2] = vector2[2];
//	return vector1;
//}

typedef float* (__cdecl* CopyVec3)(float*, float*);

uintptr_t mainModuleBase = 0;
uintptr_t* pMainModuleBase = &mainModuleBase;
uintptr_t flowerKernelModuleBase = 0;
uintptr_t* pFlowerKernelModuleBase = &flowerKernelModuleBase;


float cameraMoveSpeed = 20.0f;
float* pCameraMoveSpeed = &cameraMoveSpeed;
Vec2* pitchAndYawDistant = nullptr;
Vec2* pitchAndYaw = nullptr;

Vec3* cameraPostion;
Vec3* cameraFocus;

float* fov = nullptr;

Vec3* ammyCoordinates;
Vec3 coordinateDelta(0.0f, 0.0f, 0.0f);
Vec3* pCoordinateDelta = &coordinateDelta;

Vec3 coordinateDeltaHalf(0.0f, 0.0f, 0.0f);
Vec3* pCoordinateDeltaHalf = &coordinateDeltaHalf;

const float cameraDistanceFromFocus = 100.0f;

int* cameraType;

Vec3* detourCopyVec3(Vec3* vector1, Vec3* vector2)
{
	if (vector1 != vector2)
	{
		if (GetAsyncKeyState(0x43))
		{
			if ((uintptr_t)vector1 == (uintptr_t)(mainModuleBase + 0xB66370))
			{
				//todo, map camera focus to mouse

				return vector1;
			}


			if ((uintptr_t)vector1 == (uintptr_t)(mainModuleBase + 0xB66380))
			{
				float tempSpeed = *pCameraMoveSpeed;
				if (GetAsyncKeyState(VK_LSHIFT))
				{
					tempSpeed *= 3.0f;
				}
				if (*(uintptr_t*)(*pMainModuleBase + 0xB6B2D0))
				{
					//cameraType = (int*)(*pMainModuleBase + 0xB664BC);

					pCoordinateDelta->x = cosf(pitchAndYawDistant->y * -1) * tempSpeed;
					pCoordinateDelta->y = sinf(pitchAndYawDistant->x * -1) * tempSpeed;
					pCoordinateDelta->z = sinf(pitchAndYawDistant->y * -1) * tempSpeed;

					pCoordinateDeltaHalf->x = sinf(pitchAndYawDistant->y * 1) * tempSpeed;
					pCoordinateDeltaHalf->y = sinf(pitchAndYawDistant->x * 1) * tempSpeed;
					pCoordinateDeltaHalf->z = cosf(pitchAndYawDistant->y * 1) * tempSpeed;

					if (GetAsyncKeyState(0x57))
					{
						vector1->x += pCoordinateDelta->x;
						vector1->y += pCoordinateDelta->y;
						vector1->z += pCoordinateDelta->z;
					}
					if (GetAsyncKeyState(0x53))
					{
						vector1->x -= pCoordinateDelta->x;
						vector1->y -= pCoordinateDelta->y;
						vector1->z -= pCoordinateDelta->z;
					}
					if (GetAsyncKeyState(0x41))
					{
						vector1->x -= pCoordinateDeltaHalf->x;
						vector1->z -= pCoordinateDeltaHalf->z;
					}
					if (GetAsyncKeyState(0x44))
					{
						vector1->x += pCoordinateDeltaHalf->x;
						vector1->z += pCoordinateDeltaHalf->z;
					}

					cameraPostion = (Vec3*)(mainModuleBase + 0xB66380);
					cameraFocus = (Vec3*)(mainModuleBase + 0xB66370);

					pCoordinateDelta->x = cosf(pitchAndYawDistant->y * -1) * cameraDistanceFromFocus;
					pCoordinateDelta->y = sinf(pitchAndYawDistant->x * -1) * cameraDistanceFromFocus;
					pCoordinateDelta->z = sinf(pitchAndYawDistant->y * -1) * cameraDistanceFromFocus;

					cameraFocus->x = cameraPostion->x + pCoordinateDelta->x;
					cameraFocus->y = cameraPostion->y + pCoordinateDelta->y;
					cameraFocus->z = cameraPostion->z + pCoordinateDelta->z;

					return vector1;
				}
			}
		}

		vector1->x = vector2->x;
		vector1->y = vector2->y;
		vector1->z = vector2->z;
	}
	return vector1;
}

bool get_present_pointer()
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = FindWindowExW(0, 0, 0, L"ŌKAMI HD");
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGISwapChain* swap_chain;
	ID3D11Device* device;

	const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_levels, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &device, nullptr, nullptr) == S_OK)
	{
		void** p_vtable = *reinterpret_cast<void***>(swap_chain);
		swap_chain->Release();
		device->Release();
		//context->Release();
		p_present_target = (present)p_vtable[8];
		return true;
	}
	return false;
}

WNDPROC oWndProc;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse)
	{
		ClipCursor(NULL);
		ShowCursor(TRUE);
		return true;
	}
	if (GetAsyncKeyState(VK_END))
	{
		//EnableMouseInPointer(TRUE);
		ClipCursor(NULL);
		ShowCursor(TRUE);
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		while (ShowCursor(FALSE) > -1);
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HWND window = NULL;
ID3D11Device* p_device = NULL;
ID3D11DeviceContext* p_context = NULL;
ID3D11RenderTargetView* mainRenderTargetView = NULL;
static long __stdcall detour_present(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags) {
	if (!init) {
		if (SUCCEEDED(p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&p_device)))
		{
			p_device->GetImmediateContext(&p_context);
			DXGI_SWAP_CHAIN_DESC sd;
			p_swap_chain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			p_device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(p_device, p_context);
			init = true;
		}
		else
			return p_present(p_swap_chain, sync_interval, flags);
	}

	Overlay();

	p_context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return p_present(p_swap_chain, sync_interval, flags);
}


DWORD __stdcall EjectThread(LPVOID lpParameter) {
	Sleep(100);
	FreeLibraryAndExitThread(dll_handle, 0);
	Sleep(100);
	return 0;
}

//"main" loop
int WINAPI main(HINSTANCE hinstDLL)
{
	dll_handle = hinstDLL;

	while (!FindWindowExW(0, 0, 0, L"ŌKAMI HD"))
	{
		Sleep(50);
	}

	if (!get_present_pointer())
	{
		return 1;
	}

	if (MH_Initialize())
	{
		return 1;
	}

	if (MH_CreateHook(reinterpret_cast<void**>(p_present_target), &detour_present, reinterpret_cast<void**>(&p_present)) != MH_OK) {
		return 1;
	}

	while (!GetModuleHandle(L"flower_kernel.dll"))
	{
		Sleep(50);

	}

	mainModuleBase = (uintptr_t)GetModuleHandle(L"main.dll");
	flowerKernelModuleBase = (uintptr_t)GetModuleHandle(L"flower_kernel.dll");
	pitchAndYawDistant = (Vec2*)(mainModuleBase + 0xB66390);
	pitchAndYaw = (Vec2*)(mainModuleBase + 0xB6659C);
	fov = (float*)(mainModuleBase + 0xB663B0);

	uintptr_t addressOfFunction = (flowerKernelModuleBase + 0x1A9D0);

	CopyVec3 pVec3target = reinterpret_cast<CopyVec3>(addressOfFunction);
	
	if (MH_CreateHook(reinterpret_cast<void**>(addressOfFunction), &detourCopyVec3, reinterpret_cast<void**>(&pVec3)) != MH_OK) {
		return 1;
	}
		

	if (MH_EnableHook(p_present_target) != MH_OK) {
		return 1;
	}

	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	while (*(uintptr_t*)(mainModuleBase + 0xB74414) != 0x00000D00)
	{
	
	}




	const int sleepTime = 50;

	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000E00;
	//Sleep(sleepTime);
	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00001400;
	//Sleep(sleepTime);
	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00001500;
	//Sleep(sleepTime);
	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00001600;
	//Sleep(sleepTime);
	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00001E00;
	//Sleep(sleepTime);
	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00001F00;
	//Sleep(sleepTime);

	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00002000;
	//Sleep(sleepTime);

	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000002;
	//Sleep(sleepTime);
	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000003;
	//Sleep(sleepTime);

	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000103;
	//Sleep(sleepTime);

	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00010103;
	//Sleep(sleepTime+300);

	//*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00010104;
	//Sleep(sleepTime+300);

	*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000004;
	Sleep(sleepTime);
	*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000104;
	Sleep(sleepTime);
	////*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000204;
	////Sleep(sleepTime);
	////*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000304;
	////Sleep(sleepTime);
	////*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000604;
	////Sleep(sleepTime);

	////*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000005;
	////Sleep(sleepTime);
	////*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000105;
	////Sleep(sleepTime);

	////*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000007;
	////Sleep(sleepTime);
	////*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000107;
	////Sleep(sleepTime);

	//*(uintptr_t*)(mainModuleBase + 0xB65E75) = 0x000C00;

	//*(uintptr_t*)(mainModuleBase + 0xB6B2AF) = 0x02;

	int temp = 0x0;
	int lastTemp = 0x0;
	while (true) {
		temp = *(uintptr_t*)(mainModuleBase + 0xB74414);
		if (temp != lastTemp)
		{
			std::cout << std::hex << temp << std::endl;
			lastTemp = temp;
		}
	}

	//Cleanup
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK) {
		return 1;
	}
	if (MH_Uninitialize() != MH_OK) {
		return 1;
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (mainRenderTargetView) { mainRenderTargetView->Release(); mainRenderTargetView = NULL; }
	if (p_context) { p_context->Release(); p_context = NULL; }
	if (p_device) { p_device->Release(); p_device = NULL; }
	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(oWndProc));

	CreateThread(0, 0, EjectThread, 0, 0, 0);

	return 0;
}