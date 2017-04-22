#pragma once

#include "editor_dock.h"

namespace terminus
{
	class LoggerDock : public EditorDock
	{
	public:
		LoggerDock();
		~LoggerDock();

	protected:
		void update_internal(double dt) override;
	};
}