#include <script/script_interface_impl.h>
#include <io/logger.h>
#include <core/context.h>

namespace terminus
{
	void ScriptInterfaceImpl::log_info(std::string text)
	{
		logger::log_info(text);
	}

	void ScriptInterfaceImpl::log_error(std::string text)
	{
		logger::log_error(text);
	}

	void ScriptInterfaceImpl::log_warning(std::string text)
	{
		logger::log_warning(text);
	}

	void ScriptInterfaceImpl::log_fatal(std::string text)
	{
		logger::log_fatal(text);
	}

	ListenerID ScriptInterfaceImpl::register_listener(EventType type, EventCallback callback)
	{
		return EventHandler::register_listener(type, callback);
	}

	void ScriptInterfaceImpl::unregister_listener(EventType type, ListenerID listener)
	{
		EventHandler::unregister_listener(type, listener);
	}

	void ScriptInterfaceImpl::load_scene(String scene)
	{
		context::get_scene_manager().load(scene);
	}

	void ScriptInterfaceImpl::preload_scene(String scene)
	{
		context::get_scene_manager().preload(scene);
	}

	void ScriptInterfaceImpl::set_active_scene(String scene)
	{
		context::get_scene_manager().set_active_scene(scene);
	}

	void ScriptInterfaceImpl::unload_scene(String scene)
	{
		context::get_scene_manager().unload(scene);
	}
}
