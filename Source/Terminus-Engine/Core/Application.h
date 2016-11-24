#pragma once

#ifndef  APPLICATION_H
#define APPLICATION_H

#include "Graphics/Config.h"
#include "Platform/PlatformBackend.h"
#include "GUI/ImGuiBackend.h"
#include "Graphics/CommandList.h"
#include "Memory/PoolAllocator.h"
#include "Memory/StackAllocator.h"
#include "Global.h"
#include "IO/FileSystem.h"
#include "IO/FileWatcher.h"
#include "Input/Input.h"
#include "Input/InputContext.h"
#include "Core/Event/EventHandler.h"
#include "Resource/ShaderCache.h"
#include "Resource/MeshCache.h"
#include "Resource/AssimpMeshLoader.h"
#include "Resource/TSMLoader.h"
#include "Resource/TextLoader.h"
#include "Resource/StbImageLoader.h"
#include "Resource/TextureCache.h"
#include "Resource/AssetCommon.h"
#include "Graphics/RenderDevice.h"
#include "Math/MathUtility.h"
#include "Utility/MeshImporter.h"
#include "ECS/World.h"

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