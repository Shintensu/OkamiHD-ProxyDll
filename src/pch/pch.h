// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <d3d11.h>

#include <Unknwn.h>
#include <dinput.h>

#include <stdio.h>
#include <tchar.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <cstdint>
#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "MinHook.h"

#endif //PCH_H