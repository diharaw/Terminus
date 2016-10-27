#pragma once

#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "Event.h"
#include "FastDelegate.h"
#include <vector>
#include <map>

typedef fastdelegate::FastDelegate1<Event*> EventCallback;
typedef std::vector<EventCallback> CallbackList;
typedef std::map<EventType, CallbackList> EventCallbackMap;

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
