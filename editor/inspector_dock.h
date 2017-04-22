#pragma once

#include "editor_dock.h"

namespace terminus
{
	class InspectorDock : public EditorDock
	{
	public:
		InspectorDock();
		~InspectorDock();

	protected:
		void update_internal(double dt) override;
	};
}