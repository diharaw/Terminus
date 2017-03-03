#ifndef ShaderKey_h
#define ShaderKey_h

#include <Resource/asset_common.h>
#include <Graphics/draw_item.h>

namespace terminus
{
    struct ShaderKey
    {
        // |            |        |
        // | 3-bits     | 1-bit  | 1-bit
        // | Renderable | Albedo |
        
        uint64_t _key;
        
        ShaderKey()
        {
            _key = 0;
        }
        
        ShaderKey(RenderableType type, bool albedo, bool normal, bool metalness, bool roughness, bool parallax, uint16 pass_id)
        {
            encode_mesh_type(type);
            encode_albedo(albedo);
            encode_normal(normal);
            encode_metalness(metalness);
            encode_roughness(roughness);
            encode_parallax_occlusion(parallax);
            encode_renderpass_id(pass_id);
        }
        
        inline void encode_renderpass_id(uint16 pass_id)
        {
            uint64 temp = pass_id;
            _key |= temp << 55;
        }
        
        inline void encode_parallax_occlusion(bool option)
        {
            uint64 temp = option;
            _key |= temp << 56;
        }
        
        inline void encode_metalness(bool option)
        {
            uint64 temp = option;
            _key |= temp << 57;
        }
        
        inline void encode_roughness(bool option)
        {
            uint64 temp = option;
            _key |= temp << 58;
        }
        
        inline void encode_normal(bool option)
        {
            uint64 temp = option;
            _key |= temp << 59;
        }
        
        inline void encode_albedo(bool option)
        {
            uint64 temp = option;
            _key |= temp << 60;
        }
        
        inline void encode_mesh_type(RenderableType type)
        {
            uint64 temp = 0;

            switch(type)
            {
                case RenderableType::StaticMesh:
                {
                    temp = 0;
                    break;
                }
                    
                case RenderableType::SkeletalMesh:
                {
                    temp = 1;
                    break;
                }
                    
                case RenderableType::Terrain:
                {
                    temp = 2;
                    break;
                }
                    
                case RenderableType::Ocean:
                {
                    temp = 3;
                    break;
                }
                    
                case RenderableType::Particle:
                {
                    temp = 4;
                    break;
                }
                    
                case RenderableType::Skybox:
                {
                    temp = 5;
                    break;
                }
                case RenderableType::Quad:
                {
                    temp = 6;
                    break;
                }
                default:
                {
                    return;
                }
            }
            
            
            _key |= temp << 61;
        }
        
        inline int	DecodeMeshType()
        {
            return (_key >> 61);
        }
        
        inline int	DecodeAlbedo()
        {
            return (_key >> 60) & 1;
        }
        
        inline int	DecodeNormal()
        {
            return (_key >> 59) & 1;
        }
        
        inline int	DecodeRoughness()
        {
            return (_key >> 58) & 1;
        }
        
        inline int	DecodeMetalness()
        {
            return (_key >> 57) & 1;
        }
        
        inline int	DecodeParallaxOcclusion()
        {
            return (_key >> 56) & 1;
        }
        
    };
}

#endif
