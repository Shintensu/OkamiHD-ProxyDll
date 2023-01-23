#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>
#include <cstdint>

#include "util.h"
#include "dinput8Proxy.h"
#include "chainLoad.h"
#include "iniConfig.h"

#include "main.h"

//entry point
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    // switch case for managing attaching/detaching
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // load chalice config file
        loadINIConfig();
        //The one real Chainload // chainloads either directly into dinput8.dll or another dinput8.dll proxy if specified in the ini file
        din8DLL = load_real_dinput8();
        chainDLL = chainLoad(chainLoadDLL);
        if (chainDLL != NULL && GetProcAddress(chainDLL, "DirectInput8Create")) {
            DirectInput8Create_fn = (DirectInput8Create_TYPE)GetProcAddress(chainDLL, "DirectInput8Create");
        }
        else {
            DirectInput8Create_fn = (DirectInput8Create_TYPE)GetProcAddress(din8DLL, "DirectInput8Create");
        }

        // create threada
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, &hinstDLL, 0, NULL);

        break;
    case DLL_PROCESS_DETACH:
        // cleanup
        FreeLibrary(hinstDLL);
        break;
    }
    return TRUE;
}