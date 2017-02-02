#include "test_script.h"
#include <iostream>
#include <ECS/scene.h>
#include <ECS/scene_manager.h>
#include <IO/logger.h>

TestScript::TestScript()
{
    SERIALIZABLE(is_first);
    SERIALIZABLE(some_int);
    SERIALIZABLE(some_float);
    
    some_int = 0;
    some_float = 0.0f;
    is_first = true;
    T_LOG_INFO("Registering Callbacks...");
    EventCallback callback;
    callback.Bind<TestScript, &TestScript::OnSceneLoad>(this);
    listener_id = terminus::EventHandler::register_listener(SceneLoadEvent::sk_Type, callback);
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
    T_LOG_INFO("The CppScript is notified of the scene load completion event!");
}

void TestScript::update(double dt)
{
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
