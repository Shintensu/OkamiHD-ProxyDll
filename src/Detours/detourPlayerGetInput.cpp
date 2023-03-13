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

long long detourPlayerGetInput(cObjBase* objectPointer)
{
	if (*playerObjectPtr == objectPointer)
	{
		return playerGetInputHook->m_ppFunctionNew(objectPointer);
	}
	else
	{
		return 1;
	}
}