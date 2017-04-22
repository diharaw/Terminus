#pragma once

#include "editor_dock.h"

namespace terminus
{
	class ProfilerDock : public EditorDock
	{
	public:
		ProfilerDock();
		~ProfilerDock();

	protected:
		void update_internal(double dt) override;
	};
}