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

void detourPlayerConstructor()
{
	playerObjectCount = 0;
	playerConstructorHook->m_ppFunctionNew();
	for (int i = 0; i < (int)playerListCount - 1; i++)
	{
		playerConstructorHook->m_ppFunctionNew();
	}
	return;
}