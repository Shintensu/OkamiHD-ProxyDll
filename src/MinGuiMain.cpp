#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "GameStructs.h"

#include "Detours.h"
#include "Initialize.h"
#include "MinGuiMain.h"
#include "MainThread.h"

#include "FunctionHook.h"

#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

void MainMenu();
void CameraWindow();
void HackWindow();
void ENetClientWindow();
void DebugWindow();

pl00** playerObjectPtr = nullptr;

gameObj::PlayerPacket playerPacket;

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
wk::math::cVec teleport(0.0f, 0.0f, 0.0f, 0.0f);

float cameraMoveSpeed = 20.0f;
float cameraDistanceFromFocus = 100.0f;

bool cameraUpdates = false;
bool freeCam;

bool teleportBool = false;
bool noclip = false;

bool connectENet = false;

char username[32];
char serverip[16];


// ImGui main function, since this is ran directly in the games main thread, do not pause the execution at any point as it will crash the game
void ImGuiMain()
{
    //getting pointer to player object
    playerObjectPtr = (pl00**)(mainModuleBase + 0xB6B2D0);
    
    // Update localPlayer
    if (playerObjectPtr[0])
    {
        strcpy_s(playerPacket.username, username);
        playerPacket.mapID = *(int*)(mainModuleBase + 0xB6B2C8);
        memcpy(playerPacket.localPlayer, *playerObjectPtr, 0x15FC);
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Menu
    MainMenu();

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

    // Rendering
    ImGui::Render();
}



// the main menu of the overlay
void MainMenu()
{
    ImGui::Begin("OkamiHD ImGui Overlay - Main Menu");

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            ImGui::MenuItem("File");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows"))
        {
            ImGui::MenuItem("Camera Hacks", NULL, &show_camera_window);
            ImGui::MenuItem("Player Hacks", NULL, &show_hack_window);
            ImGui::MenuItem("Default ImGui Debug Windows", NULL, &show_debug);
            ImGui::EndMenu();
        }
        //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Test");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("Okami Internal Overlay | ImGui Version: (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("ABOUT THIS OVERLAY:");
        ImGui::BulletText("This is an all in one overlay for debugging, analyzing or just plain messing with the game.");
        ImGui::BulletText("It also contains a client to connect to Okami Servers.");
        ImGui::BulletText("Everything is provided as is, no guarantees.");
        ImGui::Separator();

        ImGui::Text("USER GUIDE:");
        ImGui::ShowUserGuide();
    }

    // Camera Window
    CameraWindow();

    // Hack Window
    HackWindow();

    // ENet Client
    ENetClientWindow();

    // Debug Window
    DebugWindow();  

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

// a window containing a bunch of useful stats about the camera, as well as a freecam hack
void CameraWindow()
{
    if (ImGui::CollapsingHeader("Camera"))
    {
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
                if (copyVec3Hook->EnableHook())
                    return;
                cameraUpdates = true;
            }
            else if (cameraUpdates)
            {
                if (copyVec3Hook->DisableHook())
                    return;
                cameraUpdates = false;
            }
        }
    }
}

// a window containing a bunch of player object information and hacks
void HackWindow()
{
    if (ImGui::CollapsingHeader("Player Hacks"))
    {
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
            ImGui::InputFloat4("Player Position",(float*)playerObjectPtr[0]->coordinatePointer, "%.3f", 0);
            ImGui::InputFloat4("Teleport Destination", (float*)&teleport, "%.3f", 0);
            if (teleportBool)
            {
                *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                teleportBool = false;
            }
            if (ImGui::Button("Teleport"))
            {
                if (!noclip)
                {
                    *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                    teleportBool = true;
                }
                *playerObjectPtr[0]->coordinatePointer = teleport;
            }

            if (ImGui::Button("No Clip"))
            {
                *(uintptr_t*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                noclip = !noclip;
            }


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
                ////number of jumps and air tackles used
                //*(BYTE*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xDE0, 0x1145 }) = 0;
                ////number of air tackles used
                //*(BYTE*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0xDE0, 0x1146 }) = 0;
                ////Dash Meter
                //*(WORD*)mem::FindDMAAddy((uintptr_t)(playerObjectPtr), { 0x1174 }) = 351;
            }
        }
        else
        {
            ImGui::Text("No player object found!");
        }
    }
}

void ENetClientWindow()
{
    if (ImGui::CollapsingHeader("ENet Client"))
    {
        if (ImGui::BeginTable("User Input", 2))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Username:");
            ImGui::TableSetColumnIndex(1);
            ImGui::InputText("##Username", username, 32);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Server IP:"); 
            ImGui::TableSetColumnIndex(1);
            ImGui::InputText("##Server IP", serverip, 16);

            ImGui::EndTable();
        }

        /*ImGui::Text("Username:"); ImGui::SameLine(); ImGui::InputText("##Username", username, 32);
        ImGui::Text("Server IP:"); ImGui::SameLine(); ImGui::InputText("##Server IP", serverip, 16);*/

        if (!connectENet)
        {
            if (!isConnected)
            {
                if (ImGui::Button("Connect"))
                {
                    if (!strcmp(username, ""))
                    {
                        std::cout << "Please enter a username!" << std::endl;
                    }
                    else if (!strcmp(serverip, ""))
                    {
                        std::cout << "Please enter an ip4 address!" << std::endl;
                    }
                    else
                    {
                        connectENet = true;
                    }
                }
            }
        }
        else
        {
            if (isConnected)
            {
                if (ImGui::Button("Disconnect"))
                {
                    connectENet = false;
                }
            }
        }
        
        if (isConnected)
        {
            if (playerList && playerPacketList)
            {
                if (ImGui::CollapsingHeader("Player List"))
                {
                    if (playerList[0])
                    {
                        for (int i = 0; i < playerListCount; i++)
                        {
                            ImGui::Text(playerPacketList[i].username);
                            ImGui::Text("X: %.3f Y: %.3f Z: %.3f", playerList[i]->coordinatePointer->identity.x, playerList[i]->coordinatePointer->identity.y, playerList[i]->coordinatePointer->identity.z);
                            ImGui::Text("Pitch: %.3f Yaw: %.3f Roll: %.3f", playerList[i]->rotation.identity.x, playerList[i]->rotation.identity.y, playerList[i]->rotation.identity.z);
                            ImGui::Text("Area ID: %X", playerPacketList[i].mapID);
                            ImGui::Text("Animation ID: %p", playerList[i]->mtb3CamPointer);
                            ImGui::Text("Movement Stage: %X", playerList[i]->movementStage);
                            ImGui::Separator();
                        }
                    }
                }
            }
            if (playerPointerList)
            {
                if (ImGui::CollapsingHeader("pl00 Entity List"))
                {
                    if (playerPointerList[0])
                    {
                        for (int i = 0; i < playerListCount; i++)
                        {
                            ImGui::Text("Entity Pointer: %p", playerPointerList[i]);
                            ImGui::Text("X: %.3f Y: %.3f Z: %.3f", playerPointerList[i]->coordinatePointer->identity.x, playerPointerList[i]->coordinatePointer->identity.y, playerPointerList[i]->coordinatePointer->identity.z);
                            ImGui::Text("Pitch: %.3f Yaw: %.3f Roll: %.3f", playerPointerList[i]->rotation.identity.x , playerPointerList[i]->rotation.identity.y, playerPointerList[i]->rotation.identity.z);
                            ImGui::Text("Animation Data Pointer: %p", playerPointerList[i]->mtb3CamPointer);
                            ImGui::Text("Movement Stage: %X", playerPointerList[i]->movementStage);
                            ImGui::Separator();
                        }
                    }
                }
            }
        }

        // TODO: Console/Logging of stdout in ImGui instead of external console
    }
}

// a window containing various checkboxes to load default ImGui windows
void DebugWindow()
{
    if (ImGui::CollapsingHeader("Default ImGui"))
    {
        ImGui::Checkbox("About", &show_about_window);
        ImGui::Checkbox("User Guide", &show_user_guide_window);
        ImGui::Checkbox("Demo Window", &show_demo_window);

        ImGui::Checkbox("Metrics", &show_metrics_window);
        ImGui::Checkbox("Debug", &show_debug_window);

        ImGui::Checkbox("Style Editor", &show_style_editor_window);
        ImGui::Checkbox("Style Selector", &show_font_selector_window);
    }
}