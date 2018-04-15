#pragma once

#include <core/include/terminus_macros.hpp>
#include <core/include/terminus_types.hpp>
#include <event/include/event_macros.hpp>
#include <event/include/event.hpp>
#include <functional>

TE_BEGIN_TERMINUS_NAMESPACE

using EventCallback = std::function<void(Event*)>;
using CallbackID = uint32_t;

class EventManager
{
public:
    virtual CallbackID register_callback(const uint16_t& type, EventCallback callback)			= 0;
    virtual void	   unregister_callback(const uint16_t& type, const CallbackID& callback_id) = 0;
    virtual Event*	   allocate_event(const uint16_t& type)										= 0;
    virtual void	   queue_event(Event* event)												= 0;
    virtual void	   process_events()															= 0;
};

TE_END_TERMINUS_NAMESPACE
