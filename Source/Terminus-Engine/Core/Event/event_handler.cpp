#include <Core/Event/event_handler.h>
#include <iostream>

namespace terminus
{
    std::vector<Event*> EventHandler::_event_queue;
    ListenerMap EventHandler::_listener_map;
    int EventHandler::_last_listener_id = 0;
    
    EventHandler::EventHandler()
    {
        
    }
    
    EventHandler::~EventHandler()
    {
        
    }
    
    void EventHandler::queue_event(Event* event)
    {
        _event_queue.push_back(event);
    }
    
    void EventHandler::fire_event(Event* event)
    {
        for (int i = 0; i < _listener_map[event->GetType()]._num_objects; i++)
            _listener_map[event->GetType()]._objects[i]._callback.Invoke(event);
    }
    
    void EventHandler::update()
    {
        std::vector<Event*>::iterator it = _event_queue.begin();
        while (it != _event_queue.end())
        {
            Event* event = *it;
            
            ListenerMap::iterator listenerItr = _listener_map.find(event->GetType());
            
            if(listenerItr != _listener_map.end())
            {
                for (int i = 0; i < listenerItr->second.size(); i++)
                    listenerItr->second._objects[i]._callback.Invoke(event);
            }
            
            delete event;
            it = _event_queue.erase(it);
        }
    }
    
    ListenerID EventHandler::register_listener(EventType type, EventCallback callback)
    {
        ListenerID listener_id = _listener_map[type].add();
        EventListener& listener = _listener_map[type].lookup(listener_id);
        
        listener._listener_id = listener_id;
        listener._callback = callback;
    
        return listener_id;
    }
    
    void EventHandler::unregister_listener(EventType type, ListenerID listener_id)
    {
        ListenerArray& listener_arry = _listener_map[type];
        listener_arry.remove(listener_id);
    }

} // namespace terminus
