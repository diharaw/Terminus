#pragma once

#include <event/include/event_manager.hpp>
#include <stl/include/hash_map.hpp>
#include <stl/include/packed_array.hpp>
#include <mutex>

TE_BEGIN_TERMINUS_NAMESPACE

class EventManagerImpl : public EventManager
{
public:
    EventManagerImpl();
    ~EventManagerImpl();
    uint32_t register_callback(uint16_t type, EventCallback callback) override;
    void     unregister_callback(uint16_t type, uint32_t callback_id) override;
    Event*   allocate_event(uint16_t type)                            override;
    void     queue_event(Event* event)                                override;
    void     process_events()                                         override;
    
private:
    Event*   pop_event();
    
private:
    std::mutex            m_mutex;
    uint32_t			  m_front;
    uint32_t			  m_back;
    uint32_t			  m_num_events;
    Event				  m_event_pool[TE_MAX_EVENTS];
    Event*				  m_event_queue[TE_MAX_EVENTS];
    PackedArray<EventCallback, TE_MAX_CALLBACKS> m_callback_maps[16];
};

TE_END_TERMINUS_NAMESPACE
