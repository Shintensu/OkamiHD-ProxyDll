#pragma once
#include <Windows.h>
#include <d3d11.h>

extern int errorCode;

extern uintptr_t mainModuleBase;
extern uintptr_t flowerKernelModuleBase;

extern bool isReleased;

extern HINSTANCE dll_handle;
extern WNDPROC oWndProc;
extern LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);
extern present p_present_target;
extern bool get_present_pointer();

extern bool init;
extern HWND window;
extern ID3D11Device* p_device;
extern ID3D11DeviceContext* p_context;
extern ID3D11RenderTargetView* mainRenderTargetView;
extern long __stdcall detour_present(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags);

extern DWORD __stdcall EjectThread(LPVOID lpParameter);

extern int WINAPI main(HINSTANCE hinstDLL);