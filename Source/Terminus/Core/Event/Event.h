#pragma once

#ifndef EVENT_H
#define EVENT_H

#include <string>

class Actor;
class Entity;
typedef unsigned long EventType;

class Event
{
public:
    virtual ~Event() {}
	virtual EventType GetType() = 0;
	virtual std::string GetName() = 0;
};

enum EntityEventType
{
    ENTITY_CREATED = 0,
    ENTITY_DESTROYED
};

class EntityEvent : public Event
{
private:
    static const EventType sk_Type;
    Entity*                m_entity;
    EntityEventType        m_event_type;
    
public:
    EntityEvent(Entity* _entity) : m_entity(_entity) {}
    virtual ~EntityEvent() {}
    inline virtual EventType GetType()               { return sk_Type; }
    inline virtual std::string GetName()             { return "Entity Event"; }
    inline Entity* GetEntity()                       { return m_entity; }
    inline EntityEventType GetEntityEventType()      { return m_event_type; }
};


enum TriggerEventType
{
    TRIGGER_ENTERED = 0,
    TRIGGER_EXITED
};

class TriggerEvent : public Event
{
public:
    static const EventType sk_Type;
    
private:
    TriggerEventType       m_event_type;
    
public:
    TriggerEvent(TriggerEventType _type) : m_event_type(_type) {}
    virtual ~TriggerEvent() {}
    inline virtual EventType GetType()                         { return sk_Type; }
    inline virtual std::string GetName()                       { return "Entity Event"; }
    inline TriggerEventType GetTriggerEventType()              { return m_event_type; }
};


class InputStateEvent : public Event
{
public:
    static const EventType sk_Type;
    
private:
    std::string            m_state;
    
public:
    InputStateEvent(std::string _state) : m_state(_state) {}
    virtual ~InputStateEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input State Event"; }
    inline std::string GetState()                         { return m_state; }
};

class InputActionEvent : public Event
{
public:
    static const EventType sk_Type;
    
private:
    std::string            m_action;
    int                    m_value;
    
public:
    InputActionEvent(std::string _action, int _value) : m_action(_action), m_value(_value) {}
    virtual ~InputActionEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input Action Event"; }
    inline std::string GetAction()                        { return m_action; }
    inline int GetValue()                                 { return m_value; }
};

class InputAxisEvent : public Event
{
public:
    static const EventType sk_Type;
    
private:
    std::string            m_axis;
    double                 m_value;
    double                 m_delta;
    
public:
    InputAxisEvent(std::string _axis, double _value, double _delta) : m_axis(_axis), m_value(_value), m_delta(_delta) {}
    virtual ~InputAxisEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input State Event"; }
    inline std::string GetAxis()                          { return m_axis; }
    inline double GetValue()                              { return m_value; }
    inline double GetDelta()                              { return m_delta; }
};

enum FileWatcherEventType
{
    FILE_ADDED = 0,
    FILE_UPDATED
};

class FileWatcherEvent : public Event
{
private:
    static const EventType sk_Type;
    std::string            m_filename;
    FileWatcherEventType   m_event_type;
    
public:
    FileWatcherEvent(std::string _filename, FileWatcherEventType _type) : m_filename(_filename), m_event_type(_type) {}
    virtual ~FileWatcherEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input State Event"; }
    inline std::string GetFilename()                      { return m_filename; }
    inline FileWatcherEventType GetEventType()            { return m_event_type; }
};

#endif
