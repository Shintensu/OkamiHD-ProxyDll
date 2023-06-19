#pragma once

#include "BaseFunctionHook.h"

typedef void(__cdecl* PlayerMovementUpdate)(cObjBase* objectPointer);

// WIP, testing a different approach for the detours to have them all in one file. Will definitely need to define the Detour as static if I want to keep it inside of the class

class PlayerMovementUpdateHook : public BaseFunctionHook<PlayerMovementUpdate>
{
public:
	//PlayerMovementUpdate m_pFunction;
	//LPVOID m_pDetourPointer;
	//PlayerMovementUpdate m_ppFunctionNew;

	PlayerMovementUpdateHook()
	{
		m_pFunction = reinterpret_cast<PlayerMovementUpdate>(mainModuleBase + 0x3AF020);
		void(__cdecl PlayerMovementUpdateHook:: * detPtr)(cObjBase * objectPointer) = &PlayerMovementUpdateHook::Detour;
		m_pDetourPointer = (void*&)detPtr;
	}

	void __cdecl Detour(cObjBase* objectPointer)
	{
		if (*playerObjectPtr == objectPointer)
		{
			m_ppFunctionNew(objectPointer);
			return;
		}
		else
		{
			return;
		}
	}

	//int CreateHook()
	//{
	//	if (MH_CreateHook(m_pFunction, m_pDetourPointer, reinterpret_cast<void**>(&m_ppFunctionNew)) != MH_OK)
	//	{
	//		return 1;
	//	}
	//	return 0;
	//}
	//int EnableHook()
	//{
	//	if (MH_EnableHook(m_pFunction) != MH_OK)
	//	{
	//		return 1;
	//	}
	//	return 0;
	//}
	//int DisableHook()
	//{
	//	if (MH_DisableHook(m_pFunction) != MH_OK)
	//	{
	//		return 1;
	//	}
	//	return 0;
	//}
};