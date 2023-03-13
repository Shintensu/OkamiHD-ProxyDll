#pragma once

#include "BaseModule.h"

class DebugModule : public BaseModule
{
public:
    bool m_Show_about = false;
    bool m_Show_user_guide = false;
    bool m_Show_demo = false;
    bool m_Show_metrics = false;
    bool m_Show_debug = false;
    bool m_Show_style_editor = false;
    bool m_Show_style_selector = false;
    bool m_Show_font_selector = false;
public:
    DebugModule()
    {

    }
    void Main()
    {
        if (!m_IsShown)
            return;

        if (ImGui::CollapsingHeader("Default ImGui"))
        {
            ImGui::Checkbox("About", &m_Show_about);
            ImGui::Checkbox("User Guide", &m_Show_user_guide);
            ImGui::Checkbox("Demo Window", &m_Show_demo);

            ImGui::Checkbox("Metrics", &m_Show_metrics);
            ImGui::Checkbox("Debug", &m_Show_debug);

            ImGui::Checkbox("Style Editor", &m_Show_style_editor);
            ImGui::Checkbox("Style Selector", &m_Show_style_selector);

            ImGui::Checkbox("Font Selector", &m_Show_font_selector);
        }
    }
};