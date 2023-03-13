#pragma once

#include "wk.h"
#include "cParts/cModel/cObj/cObjBase/pl/pl00.h"

wk::math::cVec* detourCopyVec3(wk::math::cVec* vector1, wk::math::cVec* vector2);

long long detourPlayerGetInput(cObjBase* objectPointer);

void detourPlayerConstructor();

cObjBase* detourObjectConstructor(unsigned long long objectType, unsigned long long objectSize);

void detourcModelUpdateAnimation(wk::math::cMatrix* thisModel, wk::math::cMatrix* transMatrix);

void detourCpadUpdate(uintptr_t* thisCpad);