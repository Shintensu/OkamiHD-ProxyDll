#define WIN32_LEAN_AND_MEAN
#include "pch.h"

#include <enet/enet.h>

#include "NetworkPackets.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "UpdateVariables.h"

#include "BaseFunctionHook.h"

#include "flower_kernel/flower_kernel.h"
#include "main/main.h"

const int SLEEPTIME = 50;

FILE* fDummy;

bool isConnected = false;

unsigned int playerObjectCount = 0;

NetworkPackets::PlayerUpdatePacket* playerUpdatePacketList = nullptr;
size_t playerListCount = 0;
pl00** playerPointerList = nullptr;

// Declaring some loop relevant variables
ENetAddress address;
ENetEvent event;
ENetPeer* peer = nullptr;

void SendPlayerUpdate(ENetPeer* peer, const NetworkPackets::PlayerUpdatePacket* data)
{
	/* Create a reliable packet of size 7 containing "packet\0" */
	ENetPacket* packet = enet_packet_create(data,
		sizeof(NetworkPackets::PlayerUpdatePacket),
		ENET_PACKET_FLAG_RELIABLE);
	/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */
	enet_peer_send(peer, 1, packet);

	// MapID 4 byte int
	// position and rotation 4 floats, 16 bytes
	// animation data 4 byte int
}

void SendText(ENetPeer* peer, const char* data)
{
	/* Create a reliable packet of size 7 containing "packet\0" */
	ENetPacket* packet = enet_packet_create(data,
		strlen(data) + 1,
		ENET_PACKET_FLAG_RELIABLE);
	/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */
	enet_peer_send(peer, 0, packet);
}

int ENetThread()
{
	// Alloc Console for debugging
	AllocConsole();

	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	// Initializing ENet
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
		return EXIT_FAILURE;
	}

	// Creating a Client
	ENetHost* client;
	client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		3 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (client == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	// the actual thread loop
	while (true)
	{
		// ENet client Loop
		if (isConnected)
		{
			enet_host_service(client, &event, 0);
		}
		
		// Disconnect
		if (!mainWindow.enetWindow.m_ConnectENet && isConnected)
		{
			enet_peer_disconnect(peer, 0);
			isConnected = false;
			while (enet_host_service(client, &event, 3000) > 0)
			{
				switch (event.type)
				{
				case ENET_EVENT_TYPE_RECEIVE:
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					puts("Disconnection succeeded.");

					free(playerPointerList);
					playerPointerList = nullptr;
					free(playerUpdatePacketList);
					playerUpdatePacketList = nullptr;
					playerObjectCount = 0;

					//if (playerMoveHook->DisableHook())
					//	return EXIT_FAILURE;
					if (playerGetInputHook->DisableHook())
						return EXIT_FAILURE;
					if (playerConstructorHook->DisableHook())
						return EXIT_FAILURE;
					if (objectConstructorHook->DisableHook())
						return EXIT_FAILURE;
					//if (cModelAnimationUpdateHook->DisableHook())
					//	return EXIT_FAILURE;

					// reload stage while maintaining position
					*(wk::math::cVec*)(mainModuleBase + 0xB65E64) = *playerObjectPtr[0]->coordinatePointer;
					*(float*)(mainModuleBase + 0xB65E70) = playerObjectPtr[0]->rotation.vector.y;

					*(uintptr_t*)(mainModuleBase + 0xB6B2AF) = 2;
				}
			}
		}

		if (isConnected)
		{
			// SendText(peer, "Hello World!");

			/* Wait up to 10 milliseconds for an event. */
			while (enet_host_service(client, &event, 10) > 0)
			{
				std::cout << "Event occuring" << std::endl;
				switch (event.type)
				{
				case ENET_EVENT_TYPE_RECEIVE:
					if (event.channelID == 2)
					{
						SendPlayerUpdate(peer, &localPlayerUpdatePacket);

						if (!playerPointerList)
						{
							playerListCount = (event.packet->dataLength / sizeof(NetworkPackets::PlayerUpdatePacket));
							playerPointerList = (pl00**)calloc(playerListCount, sizeof(pl00*));
							playerUpdatePacketList = (NetworkPackets::PlayerUpdatePacket*)calloc(playerListCount, sizeof(NetworkPackets::PlayerUpdatePacket));
						}

						if (playerPointerList)
						{
							memcpy(playerUpdatePacketList, (NetworkPackets::PlayerUpdatePacket*)event.packet->data, event.packet->dataLength);
						}
					}
					else
					{
						printf("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
							event.packet->dataLength,
							event.packet->data,
							event.peer->address.host,
							event.peer->address.port,
							event.channelID);
					}
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);
					break;
				}
			}
		}

		// Connect/Reconnect
		if (mainWindow.enetWindow.m_ConnectENet && !isConnected)
		{
			/* Set address to localhost. */
			enet_address_set_host(&address, mainWindow.enetWindow.m_serverip);
			address.port = 54310; // 54310

			/* Initiate the connection, allocating the two channels 0 and 1. */
			peer = enet_host_connect(client, &address, 3, 0);
			if (peer == NULL)
			{
				fprintf(stderr,
					"No available peers for initiating an ENet connection.\n");
				exit(EXIT_FAILURE);
			}
			/* Wait up to 5 seconds for the connection attempt to succeed. */
			if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
			{
				puts("Connection to Server succeeded.");
				isConnected = true;


				//if (playerMoveHook->EnableHook())
				//	return EXIT_FAILURE;
				if (playerGetInputHook->EnableHook())
					return EXIT_FAILURE;
				if (playerConstructorHook->EnableHook())
					return EXIT_FAILURE;
				if (objectConstructorHook->EnableHook())
					return EXIT_FAILURE;
				//if (cModelAnimationUpdateHook->EnableHook())
				//	return EXIT_FAILURE;

				// reload stage while maintaining position
				*(wk::math::cVec*)(mainModuleBase + 0xB65E64) = *playerObjectPtr[0]->coordinatePointer;
				*(float*)(mainModuleBase + 0xB65E70) = playerObjectPtr[0]->rotation.vector.y;

				*(uintptr_t*)(mainModuleBase + 0xB6B2AF) = 2;

				*(uintptr_t*)(mainModuleBase + 0xB6B2AF) = 2;
			}
			else
			{
				/* Either the 5 seconds are up or a disconnect event was */
				/* received. Reset the peer in the event the 5 seconds   */
				/* had run out without any significant event.            */
				enet_peer_reset(peer);
				puts("Connection to Server failed.");
				mainWindow.enetWindow.m_ConnectENet = !mainWindow.enetWindow.m_ConnectENet;
			}
		}
	}
	// Cleanup
	enet_host_destroy(client);
	enet_deinitialize();

	//if (playerMoveHook->DisableHook())
	//	return EXIT_FAILURE;
	if (playerGetInputHook->DisableHook())
		return EXIT_FAILURE;
	if (playerConstructorHook->DisableHook())
		return EXIT_FAILURE;
	if (objectConstructorHook->DisableHook())
		return EXIT_FAILURE;
	//if (cModelAnimationUpdateHook->DisableHook())
	//	return EXIT_FAILURE;
}

int MainThread()
{
	std::thread enetThread(ENetThread);

	// pausing main thread until the company logos start to roll
	while (*(uintptr_t*)(mainModuleBase + 0xB74414) != 0x00000D00)
	{

	}
	// skipping directly to the title screen
	* (uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000004;
	Sleep(SLEEPTIME);
	*(uintptr_t*)(mainModuleBase + 0xB74414) = 0x00000104;
	Sleep(SLEEPTIME);
	//*(uintptr_t*)(mainModuleBase + 0xB65E75) = 0x000C00;
	//*(uintptr_t*)(mainModuleBase + 0xB6B2AF) = 0x02;

	//testing.playerPosition = testing2.playerPosition;
	
	while (true)
	{
		// I really should have documented what this address was, whoops. All I remember is that I did this to reset the 'PlayerObjectCount' after a load
		// Probably is the boolean that indicates whether the loading is active or not?

		if (*(bool*)(mainModuleBase + 0xB6B2AF) == 0x1)
		{
			playerObjectCount = 0;
		}

		if (isConnected && playerPointerList)
		{
			if (playerPointerList[0] && *playerObjectPtr && playerPointerList[0]->mtb3CamPointer)
			{
				bool j = false;
				for (int i = 0; i < playerListCount - 1; i++)
				{
					if (strlen(playerUpdatePacketList[i].username) > 3)
					{ 
						std::string tempName(playerUpdatePacketList[i].username);
						std::string tempName2(localPlayerUpdatePacket.username);
						
						if (strcmp(playerUpdatePacketList[i].username, localPlayerUpdatePacket.username) == 0)
						{
							j = false;
						}

						if (strlen(playerUpdatePacketList[i + j].username) < 3 || playerUpdatePacketList[i + j].mapID != localPlayerUpdatePacket.mapID)
						{
							*playerPointerList[i]->coordinatePointer = wk::math::cVec(0.0f, 1000.0f, 0.0f, 1.0f);
							playerPointerList[i]->rotation = wk::math::cVec(0.0f, 0.0f, 0.0f, 0.0f);
							break;
						}
						// Commented until NetworkPackets.h is working as intended
						// 
						// *playerPointerList[i]->coordinatePointer = playerUpdatePacketList[i + j].playerPosition;
						// playerPointerList[i]->coordinatePointer->vector.x = playerUpdatePacketList[i + j].playerPosition.vector.x + 50.0f;
						// playerPointerList[i]->rotation = playerUpdatePacketList[i + j].playerRotation;
						// playerPointerList[i]->movementBitfield = playerUpdatePacketList[i + j].playerMovementBitfield;

					}
				}
			}
		}
	}
}

void modelCreation()
{
	std::vector<cModel> otherPlayers;

	// TO DO: Create easy to modify attribute system: Damage, health etc in a config file
	// TO DO: Find out what parameters are used to create specific models and how to arbitrarily spawn them
}