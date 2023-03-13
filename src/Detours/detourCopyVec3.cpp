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

				pCoordinateDelta->identity.x = cosf(pitchAndYawDistant->identity.y * -1) * tempSpeed;
				pCoordinateDelta->identity.y = sinf(pitchAndYawDistant->identity.x * -1) * tempSpeed;
				pCoordinateDelta->identity.z = sinf(pitchAndYawDistant->identity.y * -1) * tempSpeed;

				pCoordinateDeltaHalf->identity.x = sinf(pitchAndYawDistant->identity.y * 1) * tempSpeed;
				pCoordinateDeltaHalf->identity.y = sinf(pitchAndYawDistant->identity.x * 1) * tempSpeed;
				pCoordinateDeltaHalf->identity.z = cosf(pitchAndYawDistant->identity.y * 1) * tempSpeed;

				if (GetAsyncKeyState(0x57))
				{
					vector1->identity.x += pCoordinateDelta->identity.x;
					vector1->identity.y += pCoordinateDelta->identity.y;
					vector1->identity.z += pCoordinateDelta->identity.z;
				}
				if (GetAsyncKeyState(0x53))
				{
					vector1->identity.x -= pCoordinateDelta->identity.x;
					vector1->identity.y -= pCoordinateDelta->identity.y;
					vector1->identity.z -= pCoordinateDelta->identity.z;
				}
				if (GetAsyncKeyState(0x41))
				{
					vector1->identity.x -= pCoordinateDeltaHalf->identity.x;
					vector1->identity.z -= pCoordinateDeltaHalf->identity.z;
				}
				if (GetAsyncKeyState(0x44))
				{
					vector1->identity.x += pCoordinateDeltaHalf->identity.x;
					vector1->identity.z += pCoordinateDeltaHalf->identity.z;
				}

				cameraPostion = (wk::math::cVec*)(mainModuleBase + 0xB66380);
				cameraFocus = (wk::math::cVec*)(mainModuleBase + 0xB66370);

				pCoordinateDelta->identity.x = cosf(pitchAndYawDistant->identity.y * -1) * tempFDistance;
				pCoordinateDelta->identity.y = sinf(pitchAndYawDistant->identity.x * -1) * tempFDistance;
				pCoordinateDelta->identity.z = sinf(pitchAndYawDistant->identity.y * -1) * tempFDistance;

				cameraFocus->identity.x = cameraPostion->identity.x + pCoordinateDelta->identity.x;
				cameraFocus->identity.y = cameraPostion->identity.y + pCoordinateDelta->identity.y;
				cameraFocus->identity.z = cameraPostion->identity.z + pCoordinateDelta->identity.z;

				return vector1;
			}
		}

		// original code
		vector1->identity.x = vector2->identity.x;
		vector1->identity.y = vector2->identity.y;
		vector1->identity.z = vector2->identity.z;
	}
	return vector1;
}