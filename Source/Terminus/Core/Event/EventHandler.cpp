#include "EventHandler.h"

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
        for (int i = 0; i < m_EventQueue.size(); i++)
        {
            EventType currentType = m_EventQueue[i]->GetType();
            
            for (int j = 0; j < m_CallbackMap[currentType].size(); j++)
                m_CallbackMap[currentType][j](m_EventQueue[i]);
        }
    }
    
    void EventHandler::RegisterListener(EventType _Type, EventCallback _Callback)
    {
        m_CallbackMap[_Type].push_back(_Callback);
    }
}
