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
                    ImGui::InputText("##Username", playerPacket.username, 32);

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
                            if (!strcmp(playerPacket.username, ""))
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
                    if (playerList && playerPacketList)
                    {
                        if (ImGui::CollapsingHeader("Player List"))
                        {
                            if (playerList[0])
                            {
                                for (int i = 0; i < playerListCount; i++)
                                {
                                    ImGui::Text(playerPacketList[i].username);
                                    ImGui::Text("X: %.3f Y: %.3f Z: %.3f", playerList[i]->coordinatePointer->vector.x, playerList[i]->coordinatePointer->vector.y, playerList[i]->coordinatePointer->vector.z);
                                    ImGui::Text("Pitch: %.3f Yaw: %.3f Roll: %.3f", playerList[i]->rotation.vector.x, playerList[i]->rotation.vector.y, playerList[i]->rotation.vector.z);
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
                                    ImGui::Text("X: %.3f Y: %.3f Z: %.3f", playerPointerList[i]->coordinatePointer->vector.x, playerPointerList[i]->coordinatePointer->vector.y, playerPointerList[i]->coordinatePointer->vector.z);
                                    ImGui::Text("Pitch: %.3f Yaw: %.3f Roll: %.3f", playerPointerList[i]->rotation.vector.x, playerPointerList[i]->rotation.vector.y, playerPointerList[i]->rotation.vector.z);
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
            ImGui::End();
        }
    };
}