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

int* cameraType;

wk::math::cVec* cameraPostion;
wk::math::cVec* cameraFocus;
wk::math::cVec* ammyCoordinates;

wk::math::cVec coordinateDelta(0.0f, 0.0f, 0.0f, 0.0f);
wk::math::cVec* pCoordinateDelta = &coordinateDelta;
wk::math::cVec coordinateDeltaHalf(0.0f, 0.0f, 0.0f, 0.0f);
wk::math::cVec* pCoordinateDeltaHalf = &coordinateDeltaHalf;

wk::math::cVec* detourCopyVec3(wk::math::cVec* vector1, wk::math::cVec* vector2)
{
	if (vector1 != vector2)
	{
		if ((uintptr_t)vector1 == (uintptr_t)(mainModuleBase + 0xB66370))
		{
			// make camera focus update code do nothing
			return vector1;
		}

		if ((uintptr_t)vector1 == (uintptr_t)(mainModuleBase + 0xB66380))
		{
			// override camera position update code with freecam code
			float tempSpeed = mainWindow.cameraWindow.m_CameraMoveSpeed;
			float tempFDistance = mainWindow.cameraWindow.m_CameraDistanceFromFocus;
			if (GetAsyncKeyState(VK_LSHIFT))
			{
				tempSpeed *= 3.0f;
			}
			if (*(uintptr_t*)(mainModuleBase + 0xB6B2D0))
			{
				/*cameraType = (int*)(mainModuleBase + 0xB664BC);
				*cameraType = 1;*/

				pCoordinateDelta->vector.x = cosf(pitchAndYawDistant->vector.y * -1) * tempSpeed;
				pCoordinateDelta->vector.y = sinf(pitchAndYawDistant->vector.x * -1) * tempSpeed;
				pCoordinateDelta->vector.z = sinf(pitchAndYawDistant->vector.y * -1) * tempSpeed;

				pCoordinateDeltaHalf->vector.x = sinf(pitchAndYawDistant->vector.y * 1) * tempSpeed;
				pCoordinateDeltaHalf->vector.y = sinf(pitchAndYawDistant->vector.x * 1) * tempSpeed;
				pCoordinateDeltaHalf->vector.z = cosf(pitchAndYawDistant->vector.y * 1) * tempSpeed;

				if (GetAsyncKeyState(0x57))
				{
					vector1->vector.x += pCoordinateDelta->vector.x;
					vector1->vector.y += pCoordinateDelta->vector.y;
					vector1->vector.z += pCoordinateDelta->vector.z;
				}
				if (GetAsyncKeyState(0x53))
				{
					vector1->vector.x -= pCoordinateDelta->vector.x;
					vector1->vector.y -= pCoordinateDelta->vector.y;
					vector1->vector.z -= pCoordinateDelta->vector.z;
				}
				if (GetAsyncKeyState(0x41))
				{
					vector1->vector.x -= pCoordinateDeltaHalf->vector.x;
					vector1->vector.z -= pCoordinateDeltaHalf->vector.z;
				}
				if (GetAsyncKeyState(0x44))
				{
					vector1->vector.x += pCoordinateDeltaHalf->vector.x;
					vector1->vector.z += pCoordinateDeltaHalf->vector.z;
				}

				cameraPostion = (wk::math::cVec*)(mainModuleBase + 0xB66380);
				cameraFocus = (wk::math::cVec*)(mainModuleBase + 0xB66370);

				pCoordinateDelta->vector.x = cosf(pitchAndYawDistant->vector.y * -1) * tempFDistance;
				pCoordinateDelta->vector.y = sinf(pitchAndYawDistant->vector.x * -1) * tempFDistance;
				pCoordinateDelta->vector.z = sinf(pitchAndYawDistant->vector.y * -1) * tempFDistance;

				cameraFocus->vector.x = cameraPostion->vector.x + pCoordinateDelta->vector.x;
				cameraFocus->vector.y = cameraPostion->vector.y + pCoordinateDelta->vector.y;
				cameraFocus->vector.z = cameraPostion->vector.z + pCoordinateDelta->vector.z;

				return vector1;
			}
		}

		// original code
		vector1->vector.x = vector2->vector.x;
		vector1->vector.y = vector2->vector.y;
		vector1->vector.z = vector2->vector.z;
	}
	return vector1;
}