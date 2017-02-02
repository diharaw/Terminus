#pragma once

#include "cpp_script.h"
#include <Core/Event/event_handler.h>

class TERMINUS_API TestScript : public terminus::CppScript
{
private:
    bool is_first;
    int some_int;
    float some_float;
    ListenerID listener_id;
    
public:
    TestScript();
    ~TestScript();
    virtual void initialize() override;
    virtual void update(double dt) override;
    virtual void shutdown() override;
    void OnSceneLoad(Event* event);
};

DECLARE_FACTORY_FUNC(TestScript, terminus::CppScript);
