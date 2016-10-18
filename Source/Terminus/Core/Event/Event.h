#pragma once

#ifndef EVENT_H
#define EVENT_H

#include <string>

class Actor;
typedef unsigned long EventType;

class Event
{
public:
	virtual ~Event();
	virtual EventType GetType() = 0;
	virtual std::string GetName() = 0;
};

class Event_ActorCreated : public Event
{
private:
	static const EventType sk_Type;
	Actor* m_Actor;

public:
	Event_ActorCreated(Actor* _Actor);
	virtual EventType GetType();
	virtual std::string GetName();
	Actor* GetActor();
};

class Event_ActorDestroyed : public Event
{
private:
	static const EventType sk_Type;
	Actor* m_Actor;

public:
	Event_ActorDestroyed(Actor* _Actor);
	virtual EventType GetType();
	virtual std::string GetName();
	Actor* GetActor();
};

class Event_FullscreenToggle : public Event
{
private:
	static const EventType sk_Type;
	bool m_isFullscreen;

public:
	inline Event_FullscreenToggle(bool _isFullscreen) { m_isFullscreen = _isFullscreen; }
	inline virtual EventType GetType() { return sk_Type; }
	inline virtual std::string GetName() { return "Event - FullscreenToggle"; }
	inline bool isFullscreen() { return m_isFullscreen; }
};

class Event_WindowResolution : public Event
{
private:
	static const EventType sk_Type;
	int m_height;
	int m_width;

public:
	inline Event_WindowResolution(int _width, int _height) { m_width = _width; m_height = _height; }
	inline virtual EventType GetType() { return sk_Type; }
	inline virtual std::string GetName() { return "Event - WindowResolution"; }
	inline int getWidth() { return m_width; }
	inline int getHeight() { return m_height; }
};

#endif
