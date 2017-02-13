#pragma once

#include <imgui.h>

namespace terminus
{
	class IEditorWindow
	{
	public:
		IEditorWindow(){}
		virtual ~IEditorWindow(){}
		virtual void update(double dt) = 0;
	};
}