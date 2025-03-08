#pragma once

#include <iostream>

#include "BaseModule.h"
namespace ImGuiModules
{
    class ENet : public ImGuiModules::BaseModule
    {
    private:
    public:
        char m_serverip[16] = { };
        bool m_ConnectENet = false;
    public:
        ENet()
        {

        }
        void Main()
        {
            if (!m_IsShown)
                return;

            ImGui::Begin("ENet Client", &m_IsShown);
            {
                if (ImGui::BeginTable("User Input", 2))
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Username:");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##Username", localPlayerUpdatePacket.username, 32);

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Server IP:");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::InputText("##Server IP", m_serverip, 16);

                    ImGui::EndTable();
                }

                /*ImGui::Text("Username:"); ImGui::SameLine(); ImGui::InputText("##Username", username, 32);
                ImGui::Text("Server IP:"); ImGui::SameLine(); ImGui::InputText("##Server IP", serverip, 16);*/

                if (!m_ConnectENet)
                {
                    if (!isConnected)
                    {
                        if (ImGui::Button("Connect"))
                        {
                            if (!strcmp(localPlayerUpdatePacket.username, ""))
                            {
                                std::cout << "Please enter a username!" << std::endl;
                            }
                            else if (!strcmp(m_serverip, ""))
                            {
                                std::cout << "Please enter an ip4 address!" << std::endl;
                            }
                            else
                            {
                                m_ConnectENet = true;
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
                            m_ConnectENet = false;
                        }
                    }
                }

                if (isConnected)
                {
                    if (playerUpdatePacketList)
                    {
                        if (ImGui::CollapsingHeader("Player List"))
                        {
                                for (int i = 0; i < playerListCount; i++)
                                {
                                    ImGui::Text(playerUpdatePacketList[i].username);
                                    ImGui::Text("X: %.3f Y: %.3f Z: %.3f", playerUpdatePacketList[i].playerPosition.vector.x, playerUpdatePacketList[i].playerPosition.vector.y, playerUpdatePacketList[i].playerPosition.vector.z);
                                    ImGui::Text("Pitch: %.3f Yaw: %.3f Roll: %.3f", playerUpdatePacketList[i].playerRotation.vector.x, playerUpdatePacketList[i].playerRotation.vector.y, playerUpdatePacketList[i].playerRotation.vector.z);
                                    ImGui::Text("Area ID: %X", playerUpdatePacketList[i].mapID);
                                    ImGui::Text("Movement Stage: %X", playerUpdatePacketList[i].playerMovementBitfield);
                                    ImGui::Separator();
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
                                    ImGui::Text("X: %.3f Y: %.3f Z: %.3f", playerPointerList[i]->coordinatePointer->vector.x, playerPointerList[i]->coordinatePointer->vector.y, playerPointerList[i]->coordinatePointer->vector.z);
                                    ImGui::Text("Pitch: %.3f Yaw: %.3f Roll: %.3f", playerPointerList[i]->rotation.vector.x, playerPointerList[i]->rotation.vector.y, playerPointerList[i]->rotation.vector.z);
                                    ImGui::Text("Animation Data Pointer: %p", playerPointerList[i]->mtb3CamPointer);
                                    ImGui::Text("Movement Stage: %X", playerPointerList[i]->movementBitfield);
                                    ImGui::Separator();
                                }
                            }
                        }
                    }
                }

                // TODO: Console/Logging of stdout in ImGui instead of external console
            }
            ImGui::End();
        }
    };
}