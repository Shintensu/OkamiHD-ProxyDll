#include "UpdateVariables.h"

#include "Initialize.h"

pl00** playerObjectPtr = nullptr;
gameObj::PlayerPacket playerPacket;

void UpdateVariables()
{
    //getting pointer to player object
    playerObjectPtr = (pl00**)(mainModuleBase + 0xB6B2D0);

    // Update localPlayer
    if (playerObjectPtr[0])
    {
        playerPacket.mapID = *(int*)(mainModuleBase + 0xB6B2C8);
        memcpy(playerPacket.localPlayer, *playerObjectPtr, 0x15FC);
    }
}