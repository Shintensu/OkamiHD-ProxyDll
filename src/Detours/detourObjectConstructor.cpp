#include "pch.h"

#include "GameStructs.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "BaseFunctionHook.h"

#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

cObjBase* detourObjectConstructor(unsigned long long objectType, unsigned long long objectSize)
{
	cObjBase* objectPointer;
	objectPointer = objectConstructorHook->m_ppFunctionNew(objectType, objectSize);
	if (objectType == 0x100)
	{
		playerPointerList[playerObjectCount] = (pl00*)objectPointer;
		playerObjectCount++;
	}
	return objectPointer;
}