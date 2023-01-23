#include "pch.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "MinHook.h"

#include "MathStructs.h"

#include "main.h"

#include "Detours.h"
#include "Initialize.h"
#include "MinGuiMain.h"
#include "MainThread.h"


extern uintptr_t mainModuleBase;
extern math::Vec2* pitchAndYawDistant;

math::Vec3* cameraPostion;
math::Vec3* cameraFocus;
math::Vec3* ammyCoordinates;

float cameraMoveSpeed = 20.0f;
float cameraDistanceFromFocus = 100.0f;

int* cameraType;

math::Vec3 coordinateDelta(0.0f, 0.0f, 0.0f);
math::Vec3* pCoordinateDelta = &coordinateDelta;
math::Vec3 coordinateDeltaHalf(0.0f, 0.0f, 0.0f);
math::Vec3* pCoordinateDeltaHalf = &coordinateDeltaHalf;

// detour functions
math::Vec3* detourCopyVec3(math::Vec3* vector1, math::Vec3* vector2)
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
			float tempSpeed = cameraMoveSpeed;
			if (GetAsyncKeyState(VK_LSHIFT))
			{
				tempSpeed *= 3.0f;
			}
			if (*(uintptr_t*)(mainModuleBase + 0xB6B2D0))
			{
				cameraType = (int*)(mainModuleBase + 0xB664BC);
				*cameraType = 1;

				pCoordinateDelta->x = cosf(pitchAndYawDistant->y * -1) * tempSpeed;
				pCoordinateDelta->y = sinf(pitchAndYawDistant->x * -1) * tempSpeed;
				pCoordinateDelta->z = sinf(pitchAndYawDistant->y * -1) * tempSpeed;

				pCoordinateDeltaHalf->x = sinf(pitchAndYawDistant->y * 1) * tempSpeed;
				pCoordinateDeltaHalf->y = sinf(pitchAndYawDistant->x * 1) * tempSpeed;
				pCoordinateDeltaHalf->z = cosf(pitchAndYawDistant->y * 1) * tempSpeed;

				if (GetAsyncKeyState(0x57))
				{
					vector1->x += pCoordinateDelta->x;
					vector1->y += pCoordinateDelta->y;
					vector1->z += pCoordinateDelta->z;
				}
				if (GetAsyncKeyState(0x53))
				{
					vector1->x -= pCoordinateDelta->x;
					vector1->y -= pCoordinateDelta->y;
					vector1->z -= pCoordinateDelta->z;
				}
				if (GetAsyncKeyState(0x41))
				{
					vector1->x -= pCoordinateDeltaHalf->x;
					vector1->z -= pCoordinateDeltaHalf->z;
				}
				if (GetAsyncKeyState(0x44))
				{
					vector1->x += pCoordinateDeltaHalf->x;
					vector1->z += pCoordinateDeltaHalf->z;
				}

				cameraPostion = (math::Vec3*)(mainModuleBase + 0xB66380);
				cameraFocus = (math::Vec3*)(mainModuleBase + 0xB66370);

				pCoordinateDelta->x = cosf(pitchAndYawDistant->y * -1) * cameraDistanceFromFocus;
				pCoordinateDelta->y = sinf(pitchAndYawDistant->x * -1) * cameraDistanceFromFocus;
				pCoordinateDelta->z = sinf(pitchAndYawDistant->y * -1) * cameraDistanceFromFocus;

				cameraFocus->x = cameraPostion->x + pCoordinateDelta->x;
				cameraFocus->y = cameraPostion->y + pCoordinateDelta->y;
				cameraFocus->z = cameraPostion->z + pCoordinateDelta->z;

				return vector1;
			}
		}
		
		// original code
		vector1->x = vector2->x;
		vector1->y = vector2->y;
		vector1->z = vector2->z;
	}
	return vector1;
}