#pragma once

#include <event/include/event_manager.hpp>
#include <stl/include/hash_map.hpp>
#include <stl/include/packed_array.hpp>
#include <concurrency/include/mutex.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class EventManagerImpl : public EventManager
{
public:
	EventManagerImpl();
    ~EventManagerImpl();
    CallbackID register_callback(const uint16_t& type, EventCallback callback)		  override;
    void     unregister_callback(const uint16_t& type, const CallbackID& callback_id) override;
    Event*   allocate_event(const uint16_t& type)									  override;
    void     queue_event(Event* event)												  override;
    void     process_events()														  override;
    
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
