#pragma once

#include "BaseFunctionHook.h"

typedef void(__cdecl* PlayerMovementUpdate)(cObjBase* objectPointer);

class PlayerMovementUpdateHook : public BaseFunctionHook<PlayerMovementUpdate>
{
public:
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
};