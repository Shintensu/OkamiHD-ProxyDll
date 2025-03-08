#pragma once

#include "pch.h"

#include "flower_kernel/flower_kernel.h"

int MainThread();

extern const int SLEEPTIME;

extern FILE* fDummy;

extern bool isConnected;

extern unsigned int playerObjectCount;

extern NetworkPackets::PlayerUpdatePacket* playerUpdatePacketList;
extern size_t playerListCount;
extern pl00** playerPointerList;