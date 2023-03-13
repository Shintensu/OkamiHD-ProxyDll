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


void detourCpadUpdate(uintptr_t* thisCpad)
{
	if (isReleased)
	{
		return;
	}
	return cpadHook->m_ppFunctionNew(thisCpad);
}