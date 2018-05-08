#pragma once

#include <core/terminus_macros.hpp>
#include <core/terminus_types.hpp>
#include <event/event_macros.hpp>
#include <event/event.hpp>
#include <stl/packed_array.hpp>
#include <concurrency/mutex.hpp>
#include <functional>

TE_BEGIN_TERMINUS_NAMESPACE

using EventCallback = std::function<void(Event*)>;
using CallbackID = uint32_t;

class EventManager
{
public:
	EventManager();
    ~EventManager();
    CallbackID register_callback(const uint16_t& type, EventCallback callback);
    void     unregister_callback(const uint16_t& type, const CallbackID& callback_id);
    Event*   allocate_event(const uint16_t& type);
    void     queue_event(Event* event);
    void     process_events();
    
private:
    Event*   pop_event();
    
private:
    Mutex				  m_mutex;
    uint32_t			  m_front;
    uint32_t			  m_back;
    uint32_t			  m_num_events;
    Event				  m_event_pool[TE_MAX_EVENTS];
    Event*				  m_event_queue[TE_MAX_EVENTS];
    PackedArray<EventCallback, TE_MAX_CALLBACKS> m_callback_maps[16];
};

TE_END_TERMINUS_NAMESPACE
