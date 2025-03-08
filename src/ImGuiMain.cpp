#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "NetworkPackets.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "UpdateVariables.h"

#include "BaseFunctionHook.h"

#include "MainModule.h"

MainModule mainWindow;

// ImGui main function, since this is ran directly in the games main thread, do not pause the execution at any point as it will crash the game
void ImGuiMain()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Main Menu
    mainWindow.Show();
    mainWindow.Main();

    // Rendering
    ImGui::Render();
}