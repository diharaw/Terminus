#include <ECS/camera_component.h>
#include <ECS/mesh_component.h>
#include <ECS/script_component.h>
#include <ECS/transform_component.h>

namespace terminus
{
    const ComponentID TransformComponent::_id = 0;
    const ComponentID CameraComponent::_id = 1;
    const ComponentID ScriptComponent::_id = 2;
    const ComponentID MeshComponent::_id = 3;
} // namespace terminus
