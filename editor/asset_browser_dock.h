#pragma once

#include "editor_dock.h"

namespace terminus
{
	class AssetBrowserDock : public EditorDock
	{
	public:
		AssetBrowserDock();
		~AssetBrowserDock();

	protected:
		void update_internal(double dt) override;
	};
}