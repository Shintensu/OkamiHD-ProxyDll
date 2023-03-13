#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "GameStructs.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "UpdateVariables.h"

#include "BaseFunctionHook.h"

#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

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

    // Default Windows
    if (mainWindow.debugWindow.m_Show_demo)
        ImGui::ShowDemoWindow(&mainWindow.debugWindow.m_Show_demo);
    if (mainWindow.debugWindow.m_Show_metrics)
        ImGui::ShowMetricsWindow();
    if (mainWindow.debugWindow.m_Show_about)
        ImGui::ShowAboutWindow();
    if (mainWindow.debugWindow.m_Show_debug)
        ImGui::ShowDebugLogWindow();
    if (mainWindow.debugWindow.m_Show_style_editor)
        ImGui::ShowStyleEditor();
    if (mainWindow.debugWindow.m_Show_style_selector)
        ImGui::ShowStyleSelector("Style Selector");
    if (mainWindow.debugWindow.m_Show_font_selector)
        ImGui::ShowFontSelector("Font Selector");
    if (mainWindow.debugWindow.m_Show_user_guide)
        ImGui::ShowUserGuide();

    // Rendering
    ImGui::Render();
}