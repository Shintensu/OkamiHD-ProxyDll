#pragma once

#include "MainModule/CameraModule.h"
#include "MainModule/PlayerModule.h"
#include "MainModule/ENetModule.h"
#include "MainModule/DebugModule.h"
#include "MainModule/InventoryModule.h"

class MainModule : public BaseModule
{
private:
    bool m_Show_camera_window = false;
    bool m_Show_hack_window = false;
    bool m_Show_debug = false;
public:
    CameraModule cameraWindow;
    PlayerModule playerWindow;
    ENetModule enetWindow;
    DebugModule debugWindow;
    InventoryModule inventoryWindow;
public:
    MainModule()
    {

    };

    void Main()
    {
        if (!m_IsShown)
            return;

        ImGui::Begin("OkamiHD ImGui Overlay - Main Menu");
        //ImGui::SetWindowPos(ImVec2(1.0f, 1.0f));
        

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                ImGui::MenuItem("File");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Windows"))
            {
                ImGui::MenuItem("Camera Hacks", NULL, &m_Show_camera_window);
                ImGui::MenuItem("Player Hacks", NULL, &m_Show_hack_window);
                ImGui::MenuItem("Default ImGui Debug Windows", NULL, &m_Show_debug);
                ImGui::EndMenu();
            }
            //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
            if (ImGui::BeginMenu("Tools"))
            {
                ImGui::MenuItem("Test");
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
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
        cameraWindow.Show();
        cameraWindow.Main();

        // Hack Window
        playerWindow.Show();
        playerWindow.Main();

        //Inventory
        inventoryWindow.Show();
        inventoryWindow.Main();

        // ENet Client
        enetWindow.Show();
        enetWindow.Main();

        // Debug Window
        debugWindow.Show();
        debugWindow.Main();

        ImGui::Checkbox("Use Encryption?", (bool*)(mainModuleBase + 0x7E6B44));

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }
};