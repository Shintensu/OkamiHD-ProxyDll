#pragma once

#include "MainModule/Camera.h"
#include "MainModule/Player.h"
#include "MainModule/ENet.h"
#include "MainModule/Inventory.h"

#include "MainModule/Console.h"
#include "MainModule/HeapViewer.h"

class MainModule : public ImGuiModules::BaseModule
{
private:
    bool m_Show_demo = false;
public:
    ImGuiModules::Camera cameraWindow;
    ImGuiModules::Player playerWindow;
    ImGuiModules::ENet enetWindow;
    ImGuiModules::Inventory inventoryWindow;
    ImGuiModules::Console consoleWindow;
    ImGuiModules::HeapViewer heapViewerWindow;
public:
    MainModule()
    {

    };

    void Main()
    {
        if (!m_IsShown)
            return;

        ImGui::Begin("OkamiHD ImGui Overlay - Main Menu", &m_IsShown);
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
                ImGui::MenuItem("Camera", NULL, &cameraWindow.m_IsShown);
                ImGui::MenuItem("Player", NULL, &playerWindow.m_IsShown);
                ImGui::MenuItem("ENet Client", NULL, &enetWindow.m_IsShown);
                ImGui::MenuItem("Inventory", NULL, &inventoryWindow.m_IsShown);
                ImGui::MenuItem("Default ImGui Demo Window", NULL, &m_Show_demo);
                ImGui::MenuItem("Console", NULL, &consoleWindow.m_IsShown);
                ImGui::MenuItem("Heap Viewer", NULL, &heapViewerWindow.m_IsShown);
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

        ImGui::Checkbox("Use Encryption?", (bool*)(mainModuleBase + 0x7E6B44));

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();

        if (cameraWindow.m_IsShown)
            cameraWindow.Main();

        if (playerWindow.m_IsShown)
            playerWindow.Main();

        if (inventoryWindow.m_IsShown)
            inventoryWindow.Main();

        if (enetWindow.m_IsShown)
            enetWindow.Main();

        if (consoleWindow.m_IsShown)
            consoleWindow.Main();

        if (heapViewerWindow.m_IsShown)
            heapViewerWindow.Main();

        // ImGui Demo Window
        if (m_Show_demo)
            ImGui::ShowDemoWindow(&m_Show_demo);
    }
};