#define WIN32_LEAN_AND_MEAN
#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "MinHook.h"

#include "MathStructs.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "MinGuiMain.h"
#include "MainThread.h"

extern uintptr_t mainModuleBase;

int temp = 0x0;
int lastTemp = 0x0;

const int sleepTime = 50;

void MainThread()
{
	// pausing the thread until the company logos are starting to roll
	while (*(uintptr_t*)(mainModuleBase + 0xB74414) != 0x00000D00)
	{

	}

	// this mostly commented code is/was for testing the title screen loading sequence

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


	// skipping directly to the title screen
	* (uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000004;
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

	// the actual thread loop
	while (true) {
		// this is a remnant from testing, but actually is useful for preventing the demo movies from playing
		temp = *(uintptr_t*)(mainModuleBase + 0xB74414);
		if (temp != lastTemp)
		{
			lastTemp = temp;
		}
	}
}