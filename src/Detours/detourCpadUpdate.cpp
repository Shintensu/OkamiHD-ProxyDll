#include "pch.h"

#include "GameStructs.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "BaseFunctionHook.h"

#include "flower_kernel/flower_kernel.h"
#include "main/main.h"


void detourCpadUpdate(uintptr_t* thisCpad)
{
	if (isReleased)
	{
		return;
	}
	return cpadHook->m_ppFunctionNew(thisCpad);
}