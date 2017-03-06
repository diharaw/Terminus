#include <resource/rendering_path_factory.h>
#include <resource/render_pass_cache.h>
#include <resource/text_loader.h>
#include <core/context.h>
#include <core/types.h>

namespace terminus
{
    namespace rendering_path_factory
    {
        RenderingPath* create(String rendering_path)
        {
            asset_common::TextLoadData* load_data = text_loader::load(rendering_path);
            RenderingPath* rp = new RenderingPath();
            
            JsonDocument doc;
            doc.Parse(load_data->buffer);
            
            if (doc.HasMember("name"))
                rp->_name = doc["name"].GetString();
            
            rapidjson::Value& render_passes = doc["render_passes"];
            
            rp->_num_render_passes = 0;
            RenderPassCache& cache = context::get_render_pass_cache();
            
            for (rapidjson::SizeType i = 0; i < render_passes.Size(); i++)
            {
                String render_pass_name = String(render_passes[i].GetString());
                rp->_render_passes[rp->_num_render_passes] = cache.load(render_pass_name);
                rp->_num_render_passes++;
            }
            
            return rp;
        }
    }
}
