#pragma once

int MainThread();

extern const int SLEEPTIME;

extern FILE* fDummy;

extern bool isConnected;

extern unsigned int playerObjectCount;

extern gameObj::PlayerPacket* playerPacketList;
extern size_t playerListCount;
extern pl00** playerPointerList;
extern pl00** playerList;