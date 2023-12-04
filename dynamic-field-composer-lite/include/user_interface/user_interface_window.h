#pragma once


#include <memory>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#include "implot.h"
#include "implot_internal.h"


#include "./simulation/visualization.h"

enum CharSize : size_t
{
	CHAR_SIZE = 50
};

namespace dnf_composer
{
	namespace user_interface
	{

		class UserInterfaceWindow
		{
		public:
			UserInterfaceWindow() = default;
			virtual void render() = 0;
			virtual ~UserInterfaceWindow() = default;

			UserInterfaceWindow(const UserInterfaceWindow&) = delete;
			UserInterfaceWindow& operator=(const UserInterfaceWindow&) = delete;
			UserInterfaceWindow(UserInterfaceWindow&&) = delete;
			UserInterfaceWindow& operator=(UserInterfaceWindow&&) = delete;
		};
	}
}

