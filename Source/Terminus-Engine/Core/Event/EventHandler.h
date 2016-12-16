#pragma once

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Event.h"
#include "Delegate11.h"
#include <vector>
#include <map>

typedef Terminus::Delegate<void(Event*)> EventCallback;
typedef std::vector<EventCallback> CallbackList;
typedef std::map<EventType, CallbackList> EventCallbackMap;

#define EVENT_METHOD_DECLARATION(x) void x(Event* event)
#define EVENT_METHOD_DEFINITION(x, y) void x::y(Event* event)

namespace Terminus
{
    class EventHandler
    {
    private:
        static std::vector<Event*> m_EventQueue;
        static EventCallbackMap m_CallbackMap;
        
    public:
        EventHandler();
        ~EventHandler();
        static void QueueEvent(Event* _Event);
        static void FireEvent(Event* _Event);
        static void Update();
        static void RegisterListener(EventType _Type, EventCallback _Callback);
        
    };
    
}

#endif
