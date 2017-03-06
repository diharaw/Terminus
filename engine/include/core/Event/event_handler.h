#pragma once

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <core/Event/event.h>
#include <container/packed_array.h>
#include <Delegate11.h>
#include <vector>
#include <map>

#define MAX_EVENT_LISTENERS 24

using EventCallback = terminus::Delegate<void(Event*)>;

struct EventListener
{
    int           _listener_id;
    EventCallback _callback;
};

using ListenerID   = ID;
using ListenerArray = PackedArray<EventListener, MAX_EVENT_LISTENERS>;
using ListenerMap  = std::map<EventType, ListenerArray>;

#define EVENT_METHOD_DECLARATION(x) void x(Event* event)
#define EVENT_METHOD_DEFINITION(x, y) void x::y(Event* event)

namespace terminus
{
    class EventHandler
    {
    private:
        static std::vector<Event*> _event_queue;
        static ListenerMap _listener_map;
        static int _last_listener_id;
        
    public:
        EventHandler();
        ~EventHandler();
        static void queue_event(Event* event);
        static void fire_event(Event* event);
        static void update();
        static ListenerID register_listener(EventType type, EventCallback callback);
        static void unregister_listener(EventType type, ListenerID listener_id);
        
    };
    
}

#endif
