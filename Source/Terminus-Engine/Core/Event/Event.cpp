#include <Core/Event/event.h>
#include <ECS/scene_manager.h>

const EventType	TriggerEvent::sk_Type(0x6905873e);
const EventType	EntityEvent::sk_Type(0xc32fea56);
const EventType InputStateEvent::sk_Type(0x791dc446);
const EventType	InputAxisEvent::sk_Type(0x63fa800);
const EventType	InputActionEvent::sk_Type(0x235cd18d);
const EventType FileWatcherEvent::sk_Type(0x791dc446);
const EventType SceneLoadEvent::sk_Type(0x8bd7b76e);
const EventType ScenePreloadEvent::sk_Type(0x8bd7b36e);
