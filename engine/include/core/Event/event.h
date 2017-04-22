#pragma once

#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <core/types.h>

class Actor;
class Entity;
typedef unsigned long EventType;

struct Event
{
    virtual ~Event() {}
	virtual EventType GetType() = 0;
	virtual std::string GetName() = 0;
};

enum EntityEventType
{
    ENTITY_CREATED = 0,
    ENTITY_DESTROYED
};

struct EntityEvent : public Event
{
    static const EventType sk_Type;
    Entity*                m_entity;
    EntityEventType        m_event_type;
    
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

struct TriggerEvent : public Event
{
    static const EventType sk_Type;
    TriggerEventType       m_event_type;
    
    TriggerEvent(TriggerEventType _type) : m_event_type(_type) {}
    virtual ~TriggerEvent() {}
    inline virtual EventType GetType()                         { return sk_Type; }
    inline virtual std::string GetName()                       { return "Entity Event"; }
    inline TriggerEventType GetTriggerEventType()              { return m_event_type; }
};


struct InputActionEvent : public Event
{
    static const EventType sk_Type;
    uint64_t            m_action;
    
    InputActionEvent(uint64_t _action) : m_action(_action) {}
    InputActionEvent(HashResult _action) : m_action(_action.hash) {}
    virtual ~InputActionEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input Action Event"; }
    inline uint64_t GetAction()                         { return m_action; }
    inline HashResult get_action_hash()
    {
        HashResult result;
        result.hash = m_action;
        return result;
    }
};

struct InputStateEvent : public Event
{
    static const EventType sk_Type;
    uint64_t            m_state;
    int                 m_value;
    
    InputStateEvent(uint64_t _state, int _value) : m_state(_state), m_value(_value) {}
    InputStateEvent(HashResult _state, int _value) : m_state(_state.hash), m_value(_value) {}
    virtual ~InputStateEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input State Event"; }
    inline uint64_t GetState()                        { return m_state; }
    inline int GetValue()                                 { return m_value; }
    inline HashResult get_state_hash()
    {
        HashResult result;
        result.hash = m_state;
        return result;
    }
};

struct InputAxisEvent : public Event
{
    static const EventType sk_Type;
    uint64_t            m_axis;
    double                 m_value;
    double                 m_delta;
    
    InputAxisEvent(uint64_t _axis, double _value, double _delta) : m_axis(_axis), m_value(_value), m_delta(_delta) {}
    InputAxisEvent(HashResult _axis, double _value, double _delta) : m_axis(_axis.hash), m_value(_value), m_delta(_delta) {}
    virtual ~InputAxisEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input State Event"; }
    inline uint64_t GetAxis()                          { return m_axis; }
    inline double GetValue()                              { return m_value; }
    inline double GetDelta()                              { return m_delta; }
    inline HashResult get_axis_hash()
    {
        HashResult result;
        result.hash = m_axis;
        return result;
    }
};

enum FileWatcherEventType
{
    FILE_ADDED = 0,
    FILE_UPDATED
};

struct FileWatcherEvent : public Event
{
    static const EventType sk_Type;
    std::string            m_filename;
    FileWatcherEventType   m_event_type;
    
    FileWatcherEvent(std::string _filename, FileWatcherEventType _type) : m_filename(_filename), m_event_type(_type) {}
    virtual ~FileWatcherEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Input State Event"; }
    inline std::string GetFilename()                      { return m_filename; }
    inline FileWatcherEventType GetEventType()            { return m_event_type; }
};

struct LuaScriptUpdatedEvent : public Event
{
    static const EventType sk_Type;
    std::string            _script;
    
    LuaScriptUpdatedEvent(std::string script) : _script(script) {}
    virtual ~LuaScriptUpdatedEvent() {}
    inline virtual EventType GetType()                    { return sk_Type; }
    inline virtual std::string GetName()                  { return "Lua Script Updated Event"; }
    inline std::string get_script_name()                  { return _script; }
};

struct WindowRectEvent : public Event
{
	static const EventType sk_Type;
	Vector2				   _size;

	WindowRectEvent(Vector2 size) : _size(size) {}
	virtual ~WindowRectEvent() {}
	inline virtual EventType GetType() { return sk_Type; }
	inline virtual std::string GetName() { return "Window Rect Resize Event"; }
};

struct DrawableRectEvent : public Event
{
	static const EventType sk_Type;
	Vector2				   _size;

	DrawableRectEvent(Vector2 size) : _size(size) {}
	virtual ~DrawableRectEvent() {}
	inline virtual EventType GetType() { return sk_Type; }
	inline virtual std::string GetName() { return "Drawable Rect Resize Event"; }
};

#endif
