#include "pch.h"

#include "GameStructs.h"

#include "main.h"

#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"
#include "UpdateVariables.h"
#include "Detours.h"

#include "BaseFunctionHook.h"

// Globals
HINSTANCE dll_handle;
uintptr_t mainModuleBase;
uintptr_t flowerKernelModuleBase;

int errorCode;

bool isReleased;
WNDPROC oWndProc;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// WndProc override, calls back to the games og WndProc
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

	// pass calls to ImGui
	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	// get ImGui IO device
	ImGuiIO& io = ImGui::GetIO();

	// release the mouse from regular gameplay when the end key is pressed
	if (GetAsyncKeyState(VK_END))
	{
		//EnableMouseInPointer(TRUE);
		ClipCursor(NULL);
		ShowCursor(TRUE);
		isReleased = true;
	}
	// stop calls to the original function if ImGui is hovered over and keep the cursor released
	if (io.WantCaptureMouse && isReleased)
	{
		ClipCursor(NULL);
		ShowCursor(TRUE);
		return true;
	}
	// hide the cursor when clicking back into the game (while not hovering over ImGui windows)
	else if (uMsg == WM_LBUTTONDOWN)
	{
		while (ShowCursor(FALSE) > -1);
		isReleased = false;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

// getting a pointer to the games present function
present p_present_target;

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
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_levels, 2, D3D11_SDK_VERSION, &sd, &swap_chain, &device, nullptr, nullptr) == S_OK)
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


// present function override, used to draw ImGui
bool init = false;
HWND window = NULL;
ID3D11Device* p_device = NULL;
ID3D11DeviceContext* p_context = NULL;
ID3D11RenderTargetView* mainRenderTargetView = NULL;
long __stdcall detour_present(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags) 
{
	if (!init) 
	{
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

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(p_device, p_context);
			init = true;
		}
		else
			return presentHook->m_ppFunctionNew(p_swap_chain, sync_interval, flags);
	}

	UpdateVariables();
	ImGuiMain();

	p_context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return presentHook->m_ppFunctionNew(p_swap_chain, sync_interval, flags);
}

DWORD __stdcall EjectThread(LPVOID lpParameter) 
{
	Sleep(100);
	FreeLibraryAndExitThread(dll_handle, 0);
	Sleep(100);
	return 0;
}

// "main" loop
int WINAPI main(HINSTANCE hinstDLL)
{
	dll_handle = hinstDLL;

	// initialize minhhook, detours/overrides and some important values
	errorCode = Initialize();
	// main thread that runs seperately from the game, not to be confused with the injected ImGuiMain function that runs in the actual thread of the game and should thus never contain any hard stops in code execution
	errorCode = MainThread();

	{
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
	}

	return errorCode;
}