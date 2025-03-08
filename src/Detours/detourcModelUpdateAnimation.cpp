#include "pch.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "ImGuiMain.h"
#include "MainThread.h"

#include "BaseFunctionHook.h"

#include "flower_kernel/flower_kernel.h"
#include "main/main.h"


void detourcModelUpdateAnimation(wk::math::cMatrix* thisModel, wk::math::cMatrix* transMatrix)
{
	if (playerPointerList && *playerPointerList)
	{
		for (int i = 0; i < (int)playerObjectCount - 1; i++)
		{
			for (int j = 0; j < 48; j++)
			{
				if (&playerPointerList[i]->matrixArray[j] == thisModel)
					return;
			}
		}
	}
	cModelAnimationUpdateHook->m_ppFunctionNew(thisModel, transMatrix);
}