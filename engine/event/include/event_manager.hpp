#pragma once

#include <core/include/terminus_macros.hpp>
#include <core/include/terminus_types.hpp>
#include <event/include/event_macros.hpp>
#include <event/include/event.hpp>
#include <functional>

TE_BEGIN_TERMINUS_NAMESPACE

typedef std::function<void(Event*)> EventCallback;

class EventManager
{
public:
    virtual uint32_t register_callback(uint16_t type, EventCallback callback) = 0;
    virtual void     unregister_callback(uint16_t type, uint32_t callback_id) = 0;
    virtual Event*   allocate_event(uint16_t type)                            = 0;
    virtual void     queue_event(Event* event)                                = 0;
    virtual void     process_events()                                         = 0;
};

TE_END_TERMINUS_NAMESPACE
