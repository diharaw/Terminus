#include "ShaderFactory.h"
#include "../Graphics/RenderBackend.h"

ShaderFactory::ShaderFactory()
{
    
}

ShaderFactory::~ShaderFactory()
{
    
}

ResourceHandle ShaderFactory::Create(asset_common::TextLoadData* _data)
{
    switch (_data->shader_type)
    {
        case SHADER_VERTEX:
            return RenderBackend::CreateVertexShader(_data->buffer);
            break;
            
        case SHADER_GEOMETRY:
            return RenderBackend::CreateGeometryShader(_data->buffer);
            break;
            
        case SHADER_TESSELLATION_CONTROL:
            return RenderBackend::CreateControlShader(_data->buffer);
            break;
            
        case SHADER_TESSELLATION_EVALUATION:
            return RenderBackend::CreateEvaluationShader(_data->buffer);
            break;
        case SHADER_PIXEL:
            return RenderBackend::CreatePixelShader(_data->buffer);
            break;
            
        default:
            return USHRT_MAX;
            break;
    }
}
