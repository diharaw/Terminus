#include <event/event_manager.hpp>
#include <concurrency/scoped_lock.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

#define TE_EVENT_MASK (TE_MAX_EVENTS - 1u)

EventManager::EventManager()
{
    m_num_events = 0;
    m_front = 0;
    m_back = 0;
}

EventManager::~EventManager()
{
    
}

CallbackID EventManager::register_callback(const uint16_t& type, EventCallback callback)
{
    if(type < 16)
    {
        ID callback_id = m_callback_maps[type].add();
        m_callback_maps[type].lookup(callback_id) = callback;
        return callback_id;
    }
    else
        return 0;
}

void EventManager::unregister_callback(const uint16_t& type, const CallbackID& callback_id)
{
    if(type < 16)
    {
        auto& map = m_callback_maps[type];
        
        if(map.has(callback_id))
            map.remove(callback_id);
    }    
}

Event* EventManager::allocate_event(const uint16_t& type)
{
    uint32_t event_index = m_num_events++;
    Event* e = &m_event_pool[event_index & TE_EVENT_MASK];
    e->type = type;
    return e;
}

void EventManager::queue_event(Event* event)
{
    ScopedLock lock(m_mutex);
    
    m_event_queue[m_back & TE_EVENT_MASK] = event;
    ++m_back;
}

Event* EventManager::pop_event()
{
	ScopedLock lock(m_mutex);
    
    const uint32_t event_count = m_back - m_front;
    if (event_count <= 0)
        return nullptr;
    
    --m_back;
    return m_event_queue[m_back & TE_EVENT_MASK];
}

void EventManager::process_events()
{
    Event* e = pop_event();
    
    while(e)
    {
        auto& map = m_callback_maps[e->type];
        
        for(uint32_t i = 0; i < map.size(); i++)
            map._objects[i](e);
        
        e = pop_event();
    }
}

TE_END_TERMINUS_NAMESPACE
