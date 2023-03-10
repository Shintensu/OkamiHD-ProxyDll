#pragma once

#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

int Initialize();

extern uintptr_t mainModuleBase;
extern uintptr_t flowerKernelModuleBase;

typedef float* (__cdecl* CopyVec3)(float* vector1, float* vector2);
extern CopyVec3 pVec3;
extern uintptr_t copyVec3Address;
extern CopyVec3 pVec3target;

typedef void(__cdecl* PlayerMove)(cObjBase* objectPointer);
extern PlayerMove pPlayerMove;
extern uintptr_t playerMoveAddress;
extern PlayerMove pPlayerMovetarget;

typedef long long(__cdecl* PlayerGetInput)(cObjBase* objectPointer);
extern PlayerGetInput pPlayerGetInput;
extern uintptr_t playerGetInputAddress;
extern PlayerGetInput pPlayerGetInputtarget;

typedef void(__cdecl* PlayerConstructor)();
extern PlayerConstructor pPlayerConstructor;
extern uintptr_t playerConstructorAddress;
extern PlayerConstructor pPlayerConstructortarget;

typedef cObjBase*(__cdecl* ObjectConstructor)(unsigned long long objectType, unsigned long long objectSize);
extern ObjectConstructor pObjectConstructor;
extern uintptr_t objectConstructorAdress;
extern ObjectConstructor pObjectConstructorTarget;

typedef void(__cdecl* cModelAnimationUpdate)(wk::math::cMatrix* thisModel, wk::math::cMatrix* transMatrix);
extern cModelAnimationUpdate pcModelAnimationUpdate;
extern uintptr_t cModelAnimationUpdateAddress;
extern cModelAnimationUpdate pcModelAnimationUpdateTarget;

typedef void(__cdecl* cpadUpdate)(uintptr_t* thisCpad);
extern cpadUpdate pcpadUpdate;
extern uintptr_t cpadUpdateAddress;
extern cpadUpdate pCpadUpdateTarget;

extern float* fov;
extern wk::math::cVec* pitchAndYawDistant;
extern wk::math::cVec* pitchAndYaw;
