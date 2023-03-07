#pragma once

#include <cstdint>

#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

namespace gameObj
{
	struct PlayerPacket
	{
		char username[32] = {};
		int mapID = 0;
		char localPlayer[0x15FC] = {};
	};
}