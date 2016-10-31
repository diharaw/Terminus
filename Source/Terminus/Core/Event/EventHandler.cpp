#include "EventHandler.h"
#include <iostream>

namespace Terminus
{
    std::vector<Event*> EventHandler::m_EventQueue;
    EventCallbackMap EventHandler::m_CallbackMap;
    
    EventHandler::EventHandler()
    {
        
    }
    
    EventHandler::~EventHandler()
    {
        
    }
    
    void EventHandler::QueueEvent(Event* _Event)
    {
        m_EventQueue.push_back(_Event);
    }
    
    void EventHandler::FireEvent(Event* _Event)
    {
        for (int i = 0; i < m_CallbackMap[_Event->GetType()].size(); i++)
            m_CallbackMap[_Event->GetType()][i](_Event);
    }
    
    void EventHandler::Update()
    {
        std::vector<Event*>::iterator it = m_EventQueue.begin();
        while (it != m_EventQueue.end())
        {
            Event* event = *it;
            
            EventCallbackMap::iterator callbackItr = m_CallbackMap.find(event->GetType());
            
            if(callbackItr != m_CallbackMap.end())
            {
                for (auto callback : callbackItr->second)
                    callback(event);
            }
            
            it = m_EventQueue.erase(it);
        }
    }
    
    void EventHandler::RegisterListener(EventType _Type, EventCallback _Callback)
    {
        m_CallbackMap[_Type].push_back(_Callback);
    }
}
