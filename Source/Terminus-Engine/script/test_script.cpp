#include "test_script.h"
#include <iostream>

TestScript::TestScript()
{
    SERIALIZABLE(is_first);
    SERIALIZABLE(some_int);
    SERIALIZABLE(some_float);
    
    some_int = 0;
    some_float = 0.0f;
    is_first = true;
}

TestScript::~TestScript()
{

}

void TestScript::setup_environment(terminus::ScriptInterface* script_interface)
{
	_script_interface = script_interface;
	_script_interface->log_info("Registering Callbacks...");

	EventCallback callback;
	callback.Bind<TestScript, &TestScript::OnSceneLoad>(this);
	listener_id = _script_interface->register_listener(0x8bd7b76e, callback);
}

void TestScript::initialize()
{
    
}

void TestScript::OnSceneLoad(Event* event)
{
	_script_interface->log_info("The CppScript is notified of the scene load completion event!");
}

void TestScript::update(double dt)
{
    if(is_first)
    {
        _script_interface->log_info("hot reload");
        is_first = false;
    }
}

void TestScript::shutdown()
{
	_script_interface->log_info("Unregistering Callbacks...");
	_script_interface->unregister_listener(0x8bd7b36e, listener_id);
}
