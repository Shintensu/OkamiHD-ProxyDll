#include "pch.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "BaseFunctionHook.h"

#include "flower_kernel/flower_kernel.h"
#include "main/main.h"

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