#include "test_script.h"
#include <iostream>
#include <ECS/scene.h>
#include <ECS/scene_manager.h>
#include <IO/logger.h>
#include <Windows.h>

TestScript::TestScript()
{
    SERIALIZABLE(is_first);
    SERIALIZABLE(some_int);
    SERIALIZABLE(some_float);
    
    some_int = 0;
    some_float = 0.0f;
    is_first = true;
    logger::log("Registering Callbacks...", "test", 0, LogLevel::INFO);
    EventCallback callback;
    callback.Bind<TestScript, &TestScript::OnSceneLoad>(this);
    listener_id = terminus::EventHandler::register_listener(SceneLoadEvent::sk_Type, callback);

	OutputDebugString("Initialize!\n");
}

TestScript::~TestScript()
{

}

void TestScript::initialize()
{
    
}

void TestScript::OnSceneLoad(Event* event)
{
    SceneLoadEvent* event_data = (SceneLoadEvent*)event;
	OutputDebugString("The CppScript is notified of the scene load completion event!\n");
    logger::log("The CppScript is notified of the scene load completion event!", "test", 0, LogLevel::INFO);
}

void TestScript::update(double dt)
{
    //OutputDebugString("hot reload!");
    logger::log("hot reload", "test", 0, LogLevel::INFO);

    if(is_first)
    {
        T_LOG_INFO("C++ Hot Reload!!");
        some_int = 666;
        some_float = 234.42f;
        is_first = false;
    }
}

void TestScript::shutdown()
{
    T_LOG_INFO(std::to_string(some_int));
    T_LOG_INFO(std::to_string(some_float));
    T_LOG_INFO("Unregistering listeners....");
    terminus::EventHandler::unregister_listener(SceneLoadEvent::sk_Type, listener_id);
}
