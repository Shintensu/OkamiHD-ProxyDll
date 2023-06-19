#pragma once

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "Initialize.h"
#include "UpdateVariables.h"
#include "MainThread.h"

namespace ImGuiModules
{
	class BaseModule
	{
	public:
		bool m_IsShown = false;

	public:
		virtual void Main() = 0;

		void Show()
		{
			m_IsShown = true;
		}
		void Hide()
		{
			m_IsShown = false;
		}
	};
}
