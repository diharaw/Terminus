#pragma once

#include <core/types.h>

namespace terminus
{
	class EditorDock
	{
	protected:
		bool _visible;

	public:
		EditorDock();
		virtual ~EditorDock();
		void update(double dt);
		Vector2 size();
		void set_title(StringBuffer32 title);

	protected:
		virtual void update_internal(double dt) = 0;

	protected:
		Vector2		   _rect;
		StringBuffer32 _title;
	};
}
