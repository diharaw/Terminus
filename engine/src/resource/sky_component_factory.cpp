#include <resource/sky_component_factory.h>
#include <gameplay/component_types.h>
#include <core/context.h>

namespace terminus
{
    namespace sky_component_factory
    {
        void create(JsonValue& json, Entity& entity, Scene* scene)
        {
            SkyComponent& component = scene->attach_sky_component(entity);
            TextureCache& cache = context::get_texture_cache();
            
            component.cubemap = json["cubemap"].GetString();
            component.texture = cache.load_cubemap(std::string(component.cubemap.c_str()));
        }
    }
}
