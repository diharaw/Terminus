#pragma once

#include "editor_dock.h"

namespace terminus
{
	class SceneViewportDock : public EditorDock
	{
	public:
		SceneViewportDock();
		~SceneViewportDock();

	protected:
		void update_internal(double dt) override;
	};
}
