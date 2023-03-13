#pragma once

#include "BaseFunctionHook.h"
#include "main.dll/PlayerMovementUpdateHook.h"

#include "main.h"
#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

int Initialize();

typedef float* (__cdecl* CopyVec3)(float* vector1, float* vector2);
typedef long long(__cdecl* PlayerGetInput)(cObjBase* objectPointer);
typedef void(__cdecl* PlayerConstructor)();
typedef cObjBase*(__cdecl* ObjectConstructor)(unsigned long long objectType, unsigned long long objectSize);
typedef void(__cdecl* cModelAnimationUpdate)(wk::math::cMatrix* thisModel, wk::math::cMatrix* transMatrix);
typedef void(__cdecl* cpadUpdate)(uintptr_t* thisCpad);

extern BaseFunctionHook<present>* presentHook;

extern BaseFunctionHook<CopyVec3>* copyVec3Hook;
extern PlayerMovementUpdateHook* playerMoveHook;
extern BaseFunctionHook<PlayerGetInput>* playerGetInputHook;
extern BaseFunctionHook<PlayerConstructor>* playerConstructorHook;
extern BaseFunctionHook<ObjectConstructor>* objectConstructorHook;
extern BaseFunctionHook<cModelAnimationUpdate>* cModelAnimationUpdateHook;
extern BaseFunctionHook<cpadUpdate>* cpadHook;

extern float* fov;
extern wk::math::cVec* pitchAndYawDistant;
extern wk::math::cVec* pitchAndYaw;
