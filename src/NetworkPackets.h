#pragma once

#include "flower_kernel/flower_kernel.h"

namespace NetworkPackets
{
	struct ConnectPacket
	{
		char username[32] = {};
	};

	struct PlayerUpdatePacket
	{
		// For some reason wk::math::cVec has to be declared as the first thing in the struct, possibly because it is a class. If not first in the struct, exception will be thrown that "module not found"
		wk::math::cVec playerPosition;
		wk::math::cVec playerRotation;
		char username[32] = {};
		int mapID = 0;
		unsigned int playerMovementBitfield = 0;
	};
}