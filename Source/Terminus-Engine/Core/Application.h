#pragma once

#ifndef  APPLICATION_H
#define APPLICATION_H

#include "Terminus.h"

#define TERMINUS_DECLARE_MAIN(x)			\
int main()															\
{																			\
	Terminus::Application* app = new x();		\
	if(app->Initialize())										\
	{																		\
		app->Run();												\
	}																		\
	app->Shutdown();											\
	T_SAFE_DELETE(app)									\
}																			\

namespace Terminus {

	class Application
	{
	private:
		// Entity-Component-System
		ECS::World 			 			   m_world;
		//ECS::TransformSystem 	   m_transform_system;
		//ECS::ScriptSystem    		   m_script_system;
		//ECS::RenderSystem	 	   m_render_system;

		// Graphics
		Graphics::RenderDevice  m_render_device;
		//Graphics::Renderer	   	   m_renderer;

		// Resource Caches
		Resource::MeshCache	   m_mesh_cache;
		Resource::ShaderCache   m_shader_cache;
		Resource::TextureCache   m_texture_cache;

		// Resource Factories
		Resource::MeshFactory    m_mesh_factory;
		Resource::ShaderFactory m_shader_factory;
		Resource::TextureFactory m_texture_factory;
												   
	public:
		Application();
		~Application();
		bool Initialize();
		void Run();
		void Shutdown();
	};

}

#endif