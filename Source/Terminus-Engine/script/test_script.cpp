#include "test_script.h"
#include <iostream>
#include <ECS/scene.h>
#include <IO/logger.h>

TestScript::TestScript()
{
    is_first = true;
}

TestScript::~TestScript()
{
    
}

void TestScript::initialize()
{
    T_LOG_INFO("Hello from Cpp Script!");
}

void TestScript::update(double dt)
{
    if(is_first)
    {
        T_LOG_INFO("C++ Hot Reload!!");
        is_first = false;
    }
}

void TestScript::shutdown()
{
    T_LOG_INFO("This is not a drill!");
}

void TestScript::serialize(void* mem)
{
    
}

void TestScript::deserialize(void* data)
{
    
}

size_t TestScript::get_size()
{
    return sizeof(TestScript);
}
