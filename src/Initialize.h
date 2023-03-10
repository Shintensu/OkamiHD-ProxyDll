#pragma once

#include "FunctionHook.h"

#include "main.h"
#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

int Initialize();

extern uintptr_t mainModuleBase;
extern uintptr_t flowerKernelModuleBase;

typedef float* (__cdecl* CopyVec3)(float* vector1, float* vector2);
typedef void(__cdecl* PlayerMove)(cObjBase* objectPointer);
typedef long long(__cdecl* PlayerGetInput)(cObjBase* objectPointer);
typedef void(__cdecl* PlayerConstructor)();
typedef cObjBase*(__cdecl* ObjectConstructor)(unsigned long long objectType, unsigned long long objectSize);
typedef void(__cdecl* cModelAnimationUpdate)(wk::math::cMatrix* thisModel, wk::math::cMatrix* transMatrix);
typedef void(__cdecl* cpadUpdate)(uintptr_t* thisCpad);

extern FunctionHook<present>* presentHook;

extern FunctionHook<CopyVec3>* copyVec3Hook;
extern FunctionHook<PlayerMove>* playerMoveHook;
extern FunctionHook<PlayerGetInput>* playerGetInputHook;
extern FunctionHook<PlayerConstructor>* playerConstructorHook;
extern FunctionHook<ObjectConstructor>* objectConstructorHook;
extern FunctionHook<cModelAnimationUpdate>* cModelAnimationUpdateHook;
extern FunctionHook<cpadUpdate>* cpadHook;

extern float* fov;
extern wk::math::cVec* pitchAndYawDistant;
extern wk::math::cVec* pitchAndYaw;
