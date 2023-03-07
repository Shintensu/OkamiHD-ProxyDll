#pragma once

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

extern CopyVec3 pVec3;
extern PlayerMove pPlayerMove;
extern PlayerGetInput pPlayerGetInput;
extern PlayerConstructor pPlayerConstructor;
extern ObjectConstructor pObjectConstructor;
extern cModelAnimationUpdate pcModelAnimationUpdate;

extern uintptr_t copyVec3Address;
extern uintptr_t playerMoveAddress;
extern uintptr_t playerGetInputAddress;
extern uintptr_t playerConstructorAddress;
extern uintptr_t objectConstructorAdress;
extern uintptr_t cModelAnimationUpdateAddress;

extern CopyVec3 pVec3target;
extern PlayerMove pPlayerMovetarget;
extern PlayerGetInput pPlayerGetInputtarget;
extern PlayerConstructor pPlayerConstructortarget;
extern ObjectConstructor pObjectConstructorTarget;
extern cModelAnimationUpdate pcModelAnimationUpdateTarget;

extern float* fov;
extern wk::math::cVec* pitchAndYawDistant;
extern wk::math::cVec* pitchAndYaw;
