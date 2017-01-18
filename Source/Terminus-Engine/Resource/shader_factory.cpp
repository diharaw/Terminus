#include <Resource/scene_factory.h>
#include <IO/filesystem.h>
#include <Core/context.h>

namespace terminus
{
    namespace shader_factory
    {
        Shader* create(asset_common::TextLoadData* data)
        {
            Shader* shader = context::get_render_device().CreateShader(data->shader_type, data->buffer);
            
            if (!shader)
                return nullptr;
            
            return shader;
        }
        
        Shader* create(StringList& defines, ShaderType type)
        {
            String shader_name;
            String extension;
            
#if defined(TERMINUS_OPENGL)
            extension = ".glsl";
#else
            extension = ".hlsl";
#endif
            
            switch (type)
            {
                case ShaderType::VERTEX:
                    shader_name = "VS_Template";
                    break;
                case ShaderType::GEOMETRY:
                    shader_name = "GS_Template";
                    break;
                case ShaderType::TESSELLATION_CONTROL:
                    shader_name = "TCS_Template";
                    break;
                case ShaderType::TESSELLATION_EVALUATION:
                    shader_name = "TES_Template";
                    break;
                case ShaderType::PIXEL:
                    shader_name = "PS_Template";
                    break;
                    
                default:
                    assert(false);
                    break;
            }
            
            shader_name += extension;
            
            FileHandle handle = filesystem::read_file(shader_name);
            String shader_source = String(handle.buffer);
            
            String pre_processed_source = StringUtility::GenerateSource(shader_source, defines);
            
            Shader* shader = context::get_render_device().CreateShader(type, pre_processed_source.c_str());
            
            if (!shader)
                return nullptr;
            
            return shader;
        }
    }
}
