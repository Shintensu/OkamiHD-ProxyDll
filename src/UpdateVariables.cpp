#include "UpdateVariables.h"

#include "Initialize.h"

pl00** playerObjectPtr = nullptr;
NetworkPackets::PlayerUpdatePacket localPlayerUpdatePacket;

void UpdateVariables()
{
    //getting pointer to player object
    playerObjectPtr = (pl00**)(mainModuleBase + 0xB6B2D0);

    // Update localPlayer
    if (playerObjectPtr[0])
    {
        // Commented until NetworkPackets.h is working as intended
        // 
        // localPlayerUpdatePacket.mapID = *(int*)(mainModuleBase + 0xB6B2C8);
        // localPlayerUpdatePacket.playerPosition = *(*playerObjectPtr)->coordinatePointer;
        // localPlayerUpdatePacket.playerRotation = (*playerObjectPtr)->rotation;
        // localPlayerUpdatePacket.playerMovementBitfield = (*playerObjectPtr)->movementBitfield;
    }
}