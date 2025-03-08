#include "pch.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "BaseFunctionHook.h"

#include "flower_kernel/flower_kernel.h"
#include "main/main.h"

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