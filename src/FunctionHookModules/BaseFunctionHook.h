#pragma once

#include "MinHook.h"

#include "main.h"
#include "UpdateVariables.h"
#include "MainThread.h"

#include "flower_kernel/flower_kernel.h"
#include "main/main.h"

template<typename T>
class BaseFunctionHook
{
public:
	T      m_pFunction;
	LPVOID m_pDetourPointer;
	T      m_ppFunctionNew;

	BaseFunctionHook()
		:m_pFunction(nullptr), m_pDetourPointer(nullptr), m_ppFunctionNew(nullptr)
	{

	}
	BaseFunctionHook(T functionPointer, LPVOID detourPointer)
		:m_pFunction(functionPointer), m_pDetourPointer(detourPointer), m_ppFunctionNew(nullptr)
	{

	}

	int CreateHook()
	{
		if (MH_CreateHook(m_pFunction, m_pDetourPointer, reinterpret_cast<void**>(&m_ppFunctionNew)) != MH_OK)
		{
			return 1;
		}
		return 0;
	}
	int EnableHook()
	{
		if (MH_EnableHook(m_pFunction) != MH_OK)
		{
			return 1;
		}
		return 0;
	}
	int DisableHook()
	{
		if (MH_DisableHook(m_pFunction) != MH_OK)
		{
			return 1;
		}
		return 0;
	}
};
