#include "ShaderFactory.h"
#include <Core/context.h>

namespace terminus
{
	ShaderFactory::ShaderFactory()
	{
        // Load up template source
	}

	ShaderFactory::~ShaderFactory()
	{

	}

	void ShaderFactory::Initialize(String vertex_template, String pixel_template)
	{
        m_vertex_template = vertex_template;
        m_pixel_template = pixel_template;
	}

	Shader* ShaderFactory::Create(AssetCommon::TextLoadData* _data)
	{
		Shader* shader = context::get_render_device().CreateShader(_data->shader_type, _data->buffer);

		if (!shader)
			return nullptr;

		return shader;
	}
    
    Shader* ShaderFactory::Create(StringList& _defines, ShaderType _type)
    {
        String original_source;
        
        switch (_type)
        {
            case ShaderType::VERTEX:
                original_source = m_vertex_template;
                break;
            case ShaderType::GEOMETRY:
                break;
            case ShaderType::TESSELLATION_CONTROL:
                break;
            case ShaderType::TESSELLATION_EVALUATION:
                break;
            case ShaderType::PIXEL:
                original_source = m_pixel_template;
                break;
                
            default:
                assert(false);
                break;
        }
        
        String pre_processed_source = StringUtility::GenerateSource(original_source, _defines);
        
        Shader* shader = context::get_render_device().CreateShader(_type, pre_processed_source.c_str());
        
        if (!shader)
            return nullptr;
        
        return shader;
    }
}
