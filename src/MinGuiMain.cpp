#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "MinHook.h"

#include "MathStructs.h"

#include "Detours.h"
#include "Initialize.h"
#include "MinGuiMain.h"
#include "MainThread.h"


#include "mem.h"
#include "proc.h"
#include "detourHook.h"


void MainMenu();
void CameraWindow();
void HackWindow();
void DebugWindow();

extern uintptr_t mainModuleBase;
extern uintptr_t flowerKernelModuleBase;

extern math::Vec2* pitchAndYawDistant;
extern math::Vec2* pitchAndYaw;

extern float* fov;

extern float cameraMoveSpeed;

extern int temp;
extern int lastTemp;


uintptr_t* playerObjectPtr = nullptr;

bool open = true;

bool bHealth = false, bInk = false, bmovementCheat = false;

bool show_debug = false;

bool show_demo_window = false;
bool show_metrics_window = false;
bool show_about_window = false;
bool show_debug_window = false;
bool show_style_editor_window = false;
bool show_style_selector_window = false;
bool show_font_selector_window = false;
bool show_user_guide_window = false;

bool show_camera_window = true;
bool show_hack_window = true;

ImVec4 clear_color = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
float teleport[3];

bool cameraUpdates = false;
bool freeCam;

// ImGui main function, since this is ran directly in the games main thread, do not pause the execution at any point as it will crash the game
void ImGuiMain()
{
    //getting pointer to player object
    playerObjectPtr = (uintptr_t*)(mainModuleBase + 0xB6B2D0);

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Menu
    MainMenu();

    // Camera Window
    CameraWindow();

    // Hack Window
    HackWindow();

    // Debug Window
    DebugWindow();

    // Rendering
    ImGui::Render();
}

// the main menu of the overlay
void MainMenu()
{
    ImGui::Begin("OkamiHD ImGui Overlay - Main Menu");
    ImGui::Checkbox("Camera Hacks", &show_camera_window);
    ImGui::Checkbox("Player Hacks", &show_hack_window);
    ImGui::Checkbox("Default ImGui Debug Windows", &show_debug);      // Edit bools storing our window open/close state
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

// a window containing a bunch of useful stats about the camera, as well as a freecam hack
void CameraWindow()
{
    if (show_camera_window)
    {
        ImGui::Begin("Camera", &show_camera_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::InputFloat3("Camera Focus", ((float*)(mainModuleBase + 0xB66370)), "%.3f", 0);
        ImGui::InputFloat3("Camera Position", ((float*)(mainModuleBase + 0xB66380)), "%.3f", 0);
        ImGui::InputFloat2("Camera Pitch and Yaw", (float*)pitchAndYawDistant, "%.3f", 0);
        ImGui::InputFloat2("Camera Pitch and Yaw Momentum", (float*)pitchAndYaw, "%.3f", 0);
        ImGui::InputFloat("Camera FOV", fov, 0.0f, 0.0f, "%.3f", 0);
        ImGui::InputFloat("Camera Move Speed", &cameraMoveSpeed, 0.0f, 0.0f, "%.3f", 0);
        if (ImGui::Button("Toggle Freecam"))
        {

            if (!cameraUpdates)
            {
                if (MH_EnableHook(reinterpret_cast<void**>(flowerKernelModuleBase + 0x1A9D0)) != MH_OK)
                {
                    cameraUpdates = true;
                }
            }
            else if (cameraUpdates)
            {
                if (MH_DisableHook(reinterpret_cast<void**>(flowerKernelModuleBase + 0x1A9D0)) != MH_OK)
                {
                    cameraUpdates = false;
                }
            }
        }
        ImGui::End();
    }
}

// a window containing a bunch of player object information and hacks
void HackWindow()
{
    if (show_hack_window)
    {
        ImGui::Begin("Hacks", &show_hack_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        if (*playerObjectPtr)
        {
            ImGui::Checkbox("Health", &bHealth);
            ImGui::Checkbox("Ink", &bInk);

            if (ImGui::Checkbox("Movement Cheats", &bmovementCheat))
            {
                if (bmovementCheat)
                {
                    //changes double jump to be on
                    *(uintptr_t*)(mainModuleBase + 0xB6B2B9) |= 1 << 2;

                    ////Ignore Jump Barriers
                    //mem::Nop((BYTE*)mainModuleBase + 0x46024B, 7);

                    //set max dash speed to very high
                    *(float*)(mainModuleBase + 0x6AF03C) = 13.0f;
                }
                else
                {
                    //compares double Jump ability against Dojo skill entry
                    uintptr_t clearUpper = *(uintptr_t*)(mainModuleBase + 0xB4DFA2) << sizeof(uintptr_t) - 1;
                    *(uintptr_t*)(mainModuleBase + 0xB6B2B9) &= clearUpper >> sizeof(uintptr_t) - 3;

                    ////restore original Jump code
                    //mem::Patch((BYTE*)(mainModuleBase + 0x3AB596), (BYTE*)"\xF3\x0F\x5C\xC8\xF3\x0F\x58\x8E\x4C\x0E\x00\x00", 12);
                    ////restore original Jump Barrier code
                    //mem::Patch((BYTE*)(mainModuleBase + 0x46024B), (BYTE*)"\x41\x89\x86\x14\x10\x00\x00", 7);

                    //restore original dash Speed
                    *(float*)(mainModuleBase + 0x6AF03C) = 6.900000095f;
                }
            }
            ImGui::InputFloat3("Player Position", (float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x0 }), "%.3f", 0);
            ImGui::InputFloat3("Teleport Destination", teleport, "%.3f", 0);
            if (ImGui::Button("Teleport"))
            {
                *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x0 }) = teleport[0];
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x4 }) = teleport[1];
                *(float*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xA8, 0x8 }) = teleport[2];
                *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
            }
            if (ImGui::Button("No Clip"))
                *(uintptr_t*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;

            //continous writes
            if (bHealth)
            {
                INT16* healthAddress = (INT16*)(mainModuleBase + 0xB4DF90);
                if (healthAddress)
                {
                    *healthAddress = 1337;
                }
            }

            if (bInk)
            {
                INT16* inkAddress = (INT16*)(mainModuleBase + 0xB205D9);
                if (inkAddress)
                {
                    *inkAddress = 1337;
                }
            }

            if (bmovementCheat)
            {
                //number of jumps and air tackles used
                *(BYTE*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xDE0, 0x1145 }) = 0;
                //number of air tackles used
                *(BYTE*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xDE0, 0x1146 }) = 0;
                //Dash Meter
                *(WORD*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0x1174 }) = 351;
            }
        }
        else
        {
            ImGui::Text("No player object found!");
        }
        ImGui::End();
    }
}

// a window containing various checkboxes to load default ImGui windows
void DebugWindow()
{
    if (show_debug)
    {
        ImGui::Begin("Debug Windows", &show_debug);

        ImGui::Checkbox("About", &show_about_window);
        ImGui::Checkbox("User Guide", &show_user_guide_window);
        ImGui::Checkbox("Demo Window", &show_demo_window);

        ImGui::Checkbox("Metrics", &show_metrics_window);
        ImGui::Checkbox("Debug", &show_debug_window);

        ImGui::Checkbox("Style Editor", &show_style_editor_window); 
        ImGui::Checkbox("Style Selector", &show_font_selector_window); 
        ImGui::End();
    }

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    if (show_metrics_window)
        ImGui::ShowMetricsWindow();
    if (show_about_window)
        ImGui::ShowAboutWindow();
    if (show_debug_window)
        ImGui::ShowDebugLogWindow();
    if (show_style_editor_window)
        ImGui::ShowStyleEditor();
    if (show_style_selector_window)
        ImGui::ShowStyleSelector("Style Editor");
    if (show_font_selector_window)
        ImGui::ShowFontSelector("Style Selector");
    if (show_user_guide_window)
        ImGui::ShowUserGuide();
}