#pragma once

#include <cstdint>

namespace gameObj
{
	struct PlayerPacket
	{
		char username[32] = {};
		int mapID = 0;
		char localPlayer[0x15FC] = {};
	};
}