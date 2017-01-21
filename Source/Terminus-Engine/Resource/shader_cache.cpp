#include <Core/config.h>
#include <Core/context.h>
#include <Resource/shader_cache.h>
#include <Resource/text_loader.h>
#include <IO/filesystem.h>

#include <iostream>

namespace terminus
{

		ShaderCache::ShaderCache()
		{
            filesystem::add_directory("assets/shader");
		}

		ShaderCache::~ShaderCache()
		{

		}



		ShaderProgram* ShaderCache::load(const char* _vertexID,
												   const char* _pixelID,
												   const char* _geometryID,
												   const char* _tessevalID,
												   const char* _tesscontrolID)
		{
			Shader* vertex;
			Shader* pixel;
			Shader* geometry;
			Shader* tess_eval;
			Shader* tess_control;

#if defined(TERMINUS_OPENGL)
			std::string extension = ".glsl";
#else defined(TERMINUS_DIRECT3D11)
			std::string extension = ".hlsl";
#endif

			// load vertex shader
			{
				if (_vertexID != nullptr)
				{
					std::string filename = std::string(_vertexID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = filesystem::get_file_extention(id);

                        asset_common::TextLoadData* data = text_loader::load(id);
                        data->shader_type = ShaderType::VERTEX;
                        
                        vertex = shader_factory::create(data);
					}
					else
						vertex = m_ShaderMap[id];
				}
				else
					return nullptr;
			}

			// load pixel shader
			{
				if (_vertexID != nullptr)
				{
					std::string filename = std::string(_pixelID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = filesystem::get_file_extention(id);

                        asset_common::TextLoadData* data = text_loader::load(id);
                        data->shader_type = ShaderType::PIXEL;
                        
                        pixel = shader_factory::create(data);
					}
					else
						pixel = m_ShaderMap[id];
				}
				else
					return nullptr;
			}

			// load geometry shader
			{
				if (_geometryID != nullptr)
				{
					std::string filename = std::string(_geometryID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = filesystem::get_file_extention(id);

                        asset_common::TextLoadData* data = text_loader::load(id);
                        data->shader_type = ShaderType::GEOMETRY;
                        
                        geometry = shader_factory::create(data);
					}
					else
						geometry = m_ShaderMap[id];
				}
				else
					geometry = nullptr;
			}

			// load tessellation evaluation shader
			{
				if (_tessevalID != nullptr)
				{
					std::string filename = std::string(_tessevalID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = filesystem::get_file_extention(id);

                        asset_common::TextLoadData* data = text_loader::load(id);
                        data->shader_type = ShaderType::TESSELLATION_EVALUATION;
                        
                        tess_eval = shader_factory::create(data);
					}
					else
						tess_eval = m_ShaderMap[id];
				}
				else
					tess_eval = nullptr;
			}

			// load tessellation control shader
			{
				if (_tesscontrolID != nullptr)
				{
					std::string filename = std::string(_tesscontrolID);
					std::string id = filename + extension;

					if (m_ShaderMap.find(id) == m_ShaderMap.end())
					{
						std::string extension = filesystem::get_file_extention(id);

                        asset_common::TextLoadData* data = text_loader::load(id);
                        data->shader_type = ShaderType::TESSELLATION_CONTROL;
                        
                        tess_control = shader_factory::create(data);
					}
					else
						tess_control = m_ShaderMap[id];
				}
				else
					tess_control = nullptr;
			}
			
			ShaderProgram* program = context::get_render_device().CreateShaderProgram(vertex, pixel, geometry, tess_control, tess_eval);

			if (program)
			{
				m_ShaderProgramMap[vertex] = program;
				m_ShaderProgramMap[pixel] = program;

				if (geometry)
					m_ShaderProgramMap[geometry] = program;
				if (tess_control)
					m_ShaderProgramMap[tess_control] = program;
				if (tess_eval)
					m_ShaderProgramMap[tess_eval] = program;

				return program;
			}
			else
				return nullptr;
		}

		ShaderProgram* ShaderCache::load(ShaderKey key)
		{
            if (m_ShaderProgramKeyMap.find(key._key) == m_ShaderProgramKeyMap.end())
            {
                StringList defines;
                
                if(key.DecodeAlbedo())
                {
                    defines.push_back(SHADER_DEF_DIFFUSE_MAP);
                }
                if(key.DecodeNormal())
                {
                    defines.push_back(SHADER_DEF_NORMAL_MAP);
                }
                if(key.DecodeMetalness())
                {
                    defines.push_back(SHADER_DEF_METALNESS_MAP);
                }
                if(key.DecodeRoughness())
                {
                    defines.push_back(SHADER_DEF_ROUGHNESS_MAP);
                }
                if(key.DecodeMeshType() == 1)
                {
                    defines.push_back(SHADER_DEF_SKELETAL_MESH);
                }
                if(key.DecodeParallaxOcclusion())
                {
                    defines.push_back(SHADER_DEF_PARALLAX_OCCLUSION);
                }
                
            #if defined(TERMINUS_PLATFORM_MACOS)
                defines.push_back(SHADER_DEF_APPLE);
            #endif
                
                // THE FOLLOWING BLOCK SHOULD BE SENT TO THE RENDERING THREAD
                
                Shader* vertex = shader_factory::create(defines, ShaderType::VERTEX);
                
                if(!vertex)
                    return nullptr;
                
                Shader* pixel = shader_factory::create(defines, ShaderType::PIXEL);
                
                if(!pixel)
                    return nullptr;
                
                ShaderProgram* program = context::get_render_device().CreateShaderProgram(vertex, pixel, nullptr, nullptr, nullptr);
                
                // TODO(dihara): THE ABOVE BLOCK SHOULD BE SENT TO THE RENDERING THREAD
                
                if(!program)
                    return program;
                else
                    return nullptr;
            }
            else
                return m_ShaderProgramKeyMap[key._key];
		}
    
    ShaderProgram* ShaderCache::load(RenderableType type, RenderPass* render_pass, Material* material)
    {
        ShaderKey  key;
        StringList defines;
        
        if(material->texture_maps[DIFFUSE])
        {
            defines.push_back(SHADER_DEF_DIFFUSE_MAP);
            key.encode_albedo(true);
        }
        else
            key.encode_albedo(false);
        
        if(material->texture_maps[NORMAL])
        {
            defines.push_back(SHADER_DEF_NORMAL_MAP);
            key.encode_normal(true);
        }
        else
            key.encode_normal(false);
        
        if(material->texture_maps[METALNESS])
        {
            defines.push_back(SHADER_DEF_METALNESS_MAP);
            key.encode_metalness(true);
        }
        else
            key.encode_metalness(false);
        
        if(material->texture_maps[ROUGHNESS])
        {
            defines.push_back(SHADER_DEF_ROUGHNESS_MAP);
            key.encode_roughness(true);
        }
        else
            key.encode_roughness(false);
        
        if(material->texture_maps[DISPLACEMENT])
        {
            defines.push_back(SHADER_DEF_PARALLAX_OCCLUSION);
            key.encode_parallax_occlusion(true);
        }
        else
            key.encode_parallax_occlusion(false);
        
        key.encode_mesh_type(type);
        key.encode_renderpass_id(render_pass->pass_id);
        
        if(type == RenderableType::SkeletalMesh)
            defines.push_back(SHADER_DEF_SKELETAL_MESH);
        else if(type == RenderableType::Quad)
            defines.push_back(SHADER_DEF_QUAD_MESH);
        
        if(material->alpha_discard)
            defines.push_back(SHADER_DEF_ALPHA_DISCARD);
        
        if(m_ShaderProgramKeyMap.find(key._key) == m_ShaderProgramKeyMap.end())
        {
            Shader* vs = shader_factory::create(ShaderType::VERTEX, render_pass->vs_template, defines);
            Shader* ps = shader_factory::create(ShaderType::PIXEL, render_pass->ps_template, defines);
            
            ShaderProgram* program = shader_factory::create_program(vs, ps, nullptr, nullptr, nullptr);
            m_ShaderProgramKeyMap[key._key] = program;
            
            return program;
        }
        else
            return m_ShaderProgramKeyMap[key._key];        
    }

    void ShaderCache::unload(ShaderProgram* program)
    {
        // TODO : erase from map
        context::get_render_device().DestoryShaderProgram(program);
    }

} // namespace terminus
