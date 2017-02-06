#pragma once

#include <string>
#include <types.h>
#include <Core/Event/event_handler.h>

namespace terminus
{
	class ScriptInterface
	{
	public:
		ScriptInterface() {}
		virtual ~ScriptInterface() {}

		// logger
		virtual void log_info(std::string text) = 0;
		virtual void log_error(std::string text) = 0;
		virtual void log_warning(std::string text) = 0;
		virtual void log_fatal(std::string text) = 0;

		// event handler
		virtual ListenerID register_listener(EventType type, EventCallback callback) = 0;
		virtual void unregister_listener(EventType type, ListenerID listener) = 0;

		// scene manager
		virtual void load_scene(String scene) = 0;
		virtual void preload_scene(String scene) = 0;
		virtual void set_active_scene(String scene) = 0;
		virtual void unload_scene(String scene) = 0;
	};
}
