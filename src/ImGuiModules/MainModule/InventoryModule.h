#pragma once

#include "BaseModule.h"

class InventoryModule : public BaseModule
{
private:

public:

	InventoryModule()
	{

	}

	void Main()
	{
		if (!m_IsShown)
			return;
		if (ImGui::CollapsingHeader("Inventory"))
		{
			if (ImGui::CollapsingHeader("Menuing"))
			{
				ImGui::Text("Item Base Pointer: %p", (uintptr_t*)(mainModuleBase + 0xB205C0));
				ImGui::InputInt("Item ID:", (int*)(mainModuleBase + 0x7A9814));
				ImGui::InputInt("Item ID 2:", (int*)(mainModuleBase + 0xB204C0)); 
				ImGui::InputInt("Item Name ID:", (int*)(mainModuleBase + 0x9C148E));
				ImGui::Text("Menu Type: %X", *(byte*)(mainModuleBase + 0xB1F400));
				ImGui::Text("Index Offset: %d", *(byte*)(mainModuleBase + 0xB1F401));
				ImGui::Text("Item Select Column: %d", *(byte*)(mainModuleBase + 0xB1F402));
				ImGui::Text("Item Select Row: %d", *(byte*)(mainModuleBase + 0xB1F403));
			}

			if (ImGui::CollapsingHeader("Items"))
			{
				ImGui::InputScalar("Holy Bone L:", ImGuiDataType_::ImGuiDataType_S16,(short*)(mainModuleBase + 0xB20638));
				ImGui::InputScalar("Exorcism Slip L:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20640));
				ImGui::InputScalar("Exorcism Slip M:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20642));
				ImGui::InputScalar("Exorcism Slip S:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20644));
				ImGui::InputScalar("(Re)Vengeance Slip:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20648));
				ImGui::InputScalar("Inkfinity Stone:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2064A));
				ImGui::InputScalar("Mermaid Coin:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2064C));
				ImGui::InputScalar("Golden Peach:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2064E));
				ImGui::InputScalar("Traveler's Charm:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20710));
				ImGui::InputScalar("Holy Bone M:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2074C));
				ImGui::InputScalar("Holy Bone S:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2074E));
				ImGui::InputScalar("Feedbag (Meat):", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20750));
				ImGui::InputScalar("Feedbag (Herbs):", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20752));
				ImGui::InputScalar("Feedbag (Seeds):", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20754));
				ImGui::InputScalar("Feedbag (Fish):", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20756));
				ImGui::InputScalar("Steel Fist Sake:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20762));
				ImGui::InputScalar("Steel Soul Sake:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20764));
				ImGui::InputScalar("Godly Charm:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20766));
				ImGui::InputScalar("Gold Dust:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2076C));
			}

			if (ImGui::CollapsingHeader("Karmic Transfoermers"))
			{
				ImGui::InputScalar("Karmic 1 - Canine Warrior Jin:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB206E6));
				ImGui::InputScalar("Karmic 3 - Plain Amaterasu:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20722));
				ImGui::InputScalar("Karmic 7 - Shiranui:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20724));
				ImGui::InputScalar("Karmic 8 - Shiranui Stone Statue:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20726));
				ImGui::InputScalar("Karmic 9 - Realistic:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20728));
				ImGui::InputScalar("Karmic Returner:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB207C0));
				ImGui::InputScalar("Karmic 2 - Canine Warrior Chi:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB207C2));
				ImGui::InputScalar("Karmic 6 - Canine Warrior Tei:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB207C4));
				ImGui::InputScalar("Karmic 5 - Canine Warrior Rei:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB207C6));
				ImGui::InputScalar("Karmic 4 - Canine Warrior Shin:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB207CE));
			}

			if (ImGui::CollapsingHeader("Equipment"))
			{
				if (ImGui::CollapsingHeader("Divine Instruments"))
				{
					ImGui::InputScalar("Divine Retribution:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20650));
					ImGui::InputScalar("Snarling Beast:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20652));
					ImGui::InputScalar("Infinity Judge:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20654));
					ImGui::InputScalar("Trinity Mirror:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20656));
					ImGui::InputScalar("Solar Flare:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20658));

					ImGui::InputScalar("Devout Beads:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2065A));
					ImGui::InputScalar("Life Beads:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2065C));
					ImGui::InputScalar("Exorcism Beads:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2065E));
					ImGui::InputScalar("Ressurection Beads:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20660));
					ImGui::InputScalar("Tundra Beads:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20662));

					ImGui::InputScalar("Tsumugari:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20664));
					ImGui::InputScalar("Seven Strike:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20666));
					ImGui::InputScalar("Blade of Kusanagi:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20668));
					ImGui::InputScalar("Eighth Wonder:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2066A));
					ImGui::InputScalar("Thunder Edge:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2066C));
				}
				if (ImGui::CollapsingHeader("Holy Artifacts"))
				{
					ImGui::InputScalar("Astral Pouch:", ImGuiDataType_::ImGuiDataType_S8, (byte*)(mainModuleBase + 0xB2063C));
					ImGui::InputScalar("Peace Bell:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20646));
					ImGui::InputScalar("Golden Lucky Cat:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2075A));
					ImGui::InputScalar("Thief's Glove:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2075C));
					ImGui::InputScalar("Wood Mat:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2075E));
					ImGui::InputScalar("Golden Ink Pot:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20760));
					ImGui::InputScalar("Water Tablet:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB20768));
					ImGui::InputScalar("Fire Tablet:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB2076A));
					ImGui::InputScalar("String of Beads:", ImGuiDataType_::ImGuiDataType_S16, (short*)(mainModuleBase + 0xB207CA));
				}			
			}
		}
	}
};