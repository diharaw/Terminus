#include "CameraComponent.h"
#include "MeshComponent.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"

namespace Terminus { namespace ECS {
    
    const ComponentID TransformComponent::_id = 0;
    const ComponentID CameraComponent::_id = 1;
    const ComponentID ScriptComponent::_id = 2;
    const ComponentID MeshComponent::_id = 3;
    
} }
