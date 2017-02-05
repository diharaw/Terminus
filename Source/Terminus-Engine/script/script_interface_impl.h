#pragma once

#include <script/script_interface.h>

namespace terminus
{
	class ScriptInterfaceImpl : public ScriptInterface
	{
	public:
		ScriptInterfaceImpl() {}
		~ScriptInterfaceImpl() {}

		// logger
		virtual void log_info(std::string text) override;
		virtual void log_error(std::string text) override;
		virtual void log_warning(std::string text) override;
		virtual void log_fatal(std::string text) override;
		virtual ListenerID register_listener(EventType type, EventCallback callback) override;
		virtual void unregister_listener(EventType type, ListenerID listener) override;
		virtual void load_scene(String scene) override;
		virtual void preload_scene(String scene) override;
		virtual void set_active_scene(String scene) override;
		virtual void unload_scene(String scene) override;
	};
}