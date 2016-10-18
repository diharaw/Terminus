#include "Event.h"


const EventType	Event_ActorCreated::sk_Type(0x6905873e);
const EventType	Event_ActorDestroyed::sk_Type(0xc32fea56);
const EventType Event_FullscreenToggle::sk_Type(0x791dc446);
//const EventType	Event_Actor_Picked::sk_EventType(0x63fa800);
//const EventType	Event_Actor_Collision::sk_EventType(0x235cd18d);
//const EventType Event_Actor_Move::sk_EventType(0x791dc446);

Event::~Event()
{
}

Event_ActorCreated::Event_ActorCreated(Actor* _Actor)
{
	m_Actor = _Actor;
}

EventType Event_ActorCreated::GetType()
{
	return sk_Type;
}

std::string Event_ActorCreated::GetName()
{
	return "Event - Actor Created";
}

Actor* Event_ActorCreated::GetActor()
{
	return m_Actor;
}

Event_ActorDestroyed::Event_ActorDestroyed(Actor* _Actor)
{
	m_Actor = _Actor;
}

EventType Event_ActorDestroyed::GetType()
{
	return sk_Type;
}

std::string Event_ActorDestroyed::GetName()
{
	return "Event - Actor Destroyed";
}

Actor* Event_ActorDestroyed::GetActor()
{
	return m_Actor;
}
