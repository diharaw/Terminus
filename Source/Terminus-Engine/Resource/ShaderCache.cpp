#include "../Core/Config.h"
#include "ShaderCache.h"
#include "../IO/FileSystem.h"
#include <iostream>
#include <Core/context.h>
#include <Resource/TextLoader.h>

namespace terminus
{
		ShaderCache::ShaderCache()
		{
            RegisterLoader<TextLoader>();
            filesystem::add_directory("assets/shader");
		}

		ShaderCache::~ShaderCache()
		{

		}

		void ShaderCache::Initialize()
		{
         
#if defined(TERMINUS_OPENGL)
            std::string extension = "glsl";
#else defined(TERMINUS_DIRECT3D11)
            std::string extension = "hlsl";
#endif
      
            AssetCommon::TextLoadData* data;
            String name;
            String vertex_template, pixel_template;
            
            if (m_LoaderMap.find(extension) == m_LoaderMap.end())
            {
                assert(false);
            }
            else
            {
                // Vertex Template
                name = "vs_template.";
                name += extension;
                
                data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(name));
                vertex_template = String(data->buffer);

                // Pixel Template
                name = "ps_template.";
                name += extension;
                
                data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(name));
                pixel_template = String(data->buffer);
            }
            
            
			m_Factory.Initialize(vertex_template, pixel_template);
		}

		ShaderProgram* ShaderCache::Load(const char* _vertexID,
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

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::VERTEX;

							vertex = m_Factory.Create(data);
						}
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

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::PIXEL;

							pixel = m_Factory.Create(data);
						}
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

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::GEOMETRY;

							geometry = m_Factory.Create(data);
						}
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

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::TESSELLATION_EVALUATION;

							tess_eval = m_Factory.Create(data);
						}
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

						if (m_LoaderMap.find(extension) == m_LoaderMap.end())
						{
							return nullptr;
						}
						else
						{
							AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(id));
							data->shader_type = ShaderType::TESSELLATION_CONTROL;

							tess_control = m_Factory.Create(data);
						}
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

		ShaderProgram* ShaderCache::Load(ShaderKey key)
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
                
                Shader* vertex = m_Factory.Create(defines, ShaderType::VERTEX);
                
                if(!vertex)
                    return nullptr;
                
                Shader* pixel = m_Factory.Create(defines, ShaderType::PIXEL);
                
                if(!pixel)
                    return nullptr;
                
                ShaderProgram* program = context::get_render_device().CreateShaderProgram(vertex, pixel, nullptr, nullptr, nullptr);
                
                if(!program)
                    return program;
                else
                    return nullptr;
            }
            else
                return m_ShaderProgramKeyMap[key._key];
		}

		void ShaderCache::Unload(ShaderProgram* program)
		{
			// TODO : erase from map
			context::get_render_device().DestoryShaderProgram(program);
		}

} // namespace terminus
