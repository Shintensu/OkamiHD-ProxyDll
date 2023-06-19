#pragma once

#include "BaseModule.h"
namespace ImGuiModules
{
    class Console : public ImGuiModules::BaseModule
    {
    public:
        Console()
        {

        };

        void Main()
        {
            if (!m_IsShown)
                return;

            ImGui::Begin("Overlay Console", &m_IsShown);
            ImGui::End();
        }
    };
}