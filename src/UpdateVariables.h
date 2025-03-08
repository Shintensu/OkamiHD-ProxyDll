#pragma once

#include "flower_kernel/flower_kernel.h"
#include "main/main.h"

#include "NetworkPackets.h"

extern pl00** playerObjectPtr;
extern NetworkPackets::PlayerUpdatePacket localPlayerUpdatePacket;

void UpdateVariables();
