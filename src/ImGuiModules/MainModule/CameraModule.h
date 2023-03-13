#pragma once

#include "BaseModule.h"

class CameraModule : public BaseModule
{
private:
    bool m_CameraUpdates = false;
public:
    float m_CameraMoveSpeed = 20.0f;
    float m_CameraDistanceFromFocus = 100.0f;
public:
    CameraModule()
    {

    }

    void Main()
    {
        if (!m_IsShown)
            return;

        if (ImGui::CollapsingHeader("Camera"))
        {
            ImGui::InputFloat3("Camera Focus", ((float*)(mainModuleBase + 0xB66370)), "%.3f", 0);
            ImGui::InputFloat3("Camera Position", ((float*)(mainModuleBase + 0xB66380)), "%.3f", 0);
            ImGui::InputFloat2("Camera Pitch and Yaw", (float*)pitchAndYawDistant, "%.3f", 0);
            ImGui::InputFloat2("Camera Pitch and Yaw Momentum", (float*)pitchAndYaw, "%.3f", 0);
            ImGui::InputFloat("Camera FOV", fov, 0.0f, 0.0f, "%.3f", 0);
            ImGui::InputFloat("Camera Move Speed", &m_CameraMoveSpeed, 0.0f, 0.0f, "%.3f", 0);
            if (ImGui::Button("Toggle Freecam"))
            {

                if (!m_CameraUpdates)
                {
                    if (copyVec3Hook->EnableHook())
                        return;
                    m_CameraUpdates = true;
                }
                else if (m_CameraUpdates)
                {
                    if (copyVec3Hook->DisableHook())
                        return;
                    m_CameraUpdates = false;
                }
            }
        }
    }
};

