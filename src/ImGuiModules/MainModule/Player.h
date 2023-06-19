#pragma once

#include "BaseModule.h"

namespace ImGuiModules
{
    class Player : public ImGuiModules::BaseModule
    {
    private:
        bool m_BHealth = false;
        bool m_BInk = false;
        bool m_BmovementCheats = false;
        bool m_Bteleport = false;
        bool m_Bnoclip = false;
        wk::math::cVec m_Teleport = { 0.0f, 0.0f, 0.0f, 0.0f };
    public:
        Player()
        {

        }
        void Main()
        {
            if (!m_IsShown)
                return;

            ImGui::Begin("Player", &m_IsShown);
            {
                if (*playerObjectPtr)
                {
                    ImGui::Checkbox("Health", &m_BHealth);
                    ImGui::Checkbox("Ink", &m_BInk);

                    if (ImGui::Checkbox("Movement Cheats", &m_BmovementCheats))
                    {
                        if (m_BmovementCheats)
                        {
                            //changes double jump to be on
                            *(uintptr_t*)(mainModuleBase + 0xB6B2B9) |= 1 << 2;

                            ////Ignore Jump Barriers
                            //mem::Nop((BYTE*)mainModuleBase + 0x46024B, 7);

                            //set max dash speed to very high
                            //*(float*)(mainModuleBase + 0x6AF03C) = 13.0f;
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
                            //*(float*)(mainModuleBase + 0x6AF03C) = 6.900000095f;
                        }
                    }

                    ImGui::InputFloat4("Player Position", (float*)playerObjectPtr[0]->coordinatePointer, "%.3f", 0);
                    ImGui::InputFloat4("Teleport Destination", (float*)&m_Teleport, "%.3f", 0);
                    if (m_Bteleport)
                    {
                        *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                        m_Bteleport = false;
                    }
                    if (ImGui::Button("Teleport"))
                    {
                        if (!m_Bnoclip)
                        {
                            *(BYTE*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                            m_Bteleport = true;
                        }
                        *playerObjectPtr[0]->coordinatePointer = m_Teleport;
                    }

                    if (ImGui::Button("No Clip"))
                    {
                        *(uintptr_t*)(mainModuleBase + 0xB6B2BB) ^= 1 << 1;
                        m_Bnoclip = !m_Bnoclip;
                    }


                    //continous writes
                    if (m_BHealth)
                    {
                        INT16* healthAddress = (INT16*)(mainModuleBase + 0xB4DF90);
                        if (healthAddress)
                        {
                            *healthAddress = 1337;
                        }
                    }

                    if (m_BInk)
                    {
                        INT16* inkAddress = (INT16*)(mainModuleBase + 0xB205D9);
                        if (inkAddress)
                        {
                            *inkAddress = 1337;
                        }
                    }

                    if (m_BmovementCheats)
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
            ImGui::End();
        }
    };
}