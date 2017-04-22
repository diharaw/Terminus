#pragma once

#include "editor_dock.h"

namespace terminus
{
	class SceneHeirachyDock : public EditorDock
	{
	public:
		SceneHeirachyDock();
		~SceneHeirachyDock();

	protected:
		void update_internal(double dt) override;
	};
}