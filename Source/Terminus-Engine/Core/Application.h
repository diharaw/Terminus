#pragma once

#ifndef  APPLICATION_H
#define APPLICATION_H

#include "Core/Config.h"
#include "Platform/PlatformBackend.h"
#include "GUI/ImGuiBackend.h"
#include "Graphics/Renderer.h"
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
#include "Resource/MaterialCache.h"
#include "Resource/TextLoader.h"
#include "Resource/StbImageLoader.h"
#include "Resource/TextureCache.h"
#include "Resource/AssetCommon.h"
#include "Resource/SceneCache.h"
#include "Graphics/RenderDevice.h"
#include "ECS/SceneManager.h"
#include "Math/MathUtility.h"
#include "Utility/MeshImporter.h"
#include "ECS/Scene.h"
#include "ECS/TransformSystem.h"

#if defined(TERMINUS_PLATFORM_WIN32)
#define TERMINUS_DECLARE_MAIN(x)							\
int WINAPI WinMain(HINSTANCE hinstance,			    \
									 HINSTANCE hprevinstance,			\
								     LPSTR lpcmdline,							\
									 int ncmdshow)								\
{																							\
	Terminus::Global::Initialize();											\
	Terminus::Application* app = T_NEW x();					\
	if(app->Initialize())														\
	{																						\
		app->Run();																\
	}																						\
	app->Shutdown();															\
	Terminus::Global::Shutdown();										\
	return 0;																			\
}																							\

#else

#define TERMINUS_DECLARE_MAIN(x)			\
int main()															\
{																			\
	Terminus::Global::Initialize();							\
	Terminus::Application* app = T_NEW x();	\
	if(app->Initialize())										\
	{																		\
		app->Run();												\
	}																		\
	app->Shutdown();											\
	Terminus::Global::Shutdown();						\
	return 0;															\
}																			\

#endif


namespace Terminus {

	class Application
	{
	private:
		// Entity-Component-System
		//ECS::Sc 			 			   m_world;
		ECS::SceneManager		    m_scene_manager;

		// Graphics
		Graphics::RenderDevice   m_render_device;

		// Resource Caches
		Resource::MeshCache	    m_mesh_cache;
		Resource::ShaderCache    m_shader_cache;
		Resource::TextureCache    m_texture_cache;
		Resource::SceneCache	    m_scene_cache ;
		Resource::MaterialCache  m_material_cache;

		// Resource Factories
		Resource::MeshFactory     m_mesh_factory;
		Resource::ShaderFactory  m_shader_factory;
		Resource::TextureFactory  m_texture_factory;

		ThreadPool*					   m_main_thread_pool;
        ThreadPool*                    m_rendering_thread_pool;
        
	public:
		Application();
		~Application();
		bool Initialize();
		void Run();
		void Shutdown();

	private:
		void InitializeInput();
		void InitializeResources();
		void InitializeGraphics();
		void InitializePhysics();
		void InitializeAudio();
		void InitializeECS();
		void InitializeScript();
        
        void SubmitRendering();
        void ShutdownGraphics();
        
        TASK_METHOD_DECLARATION(GraphicsInitializeTask);
        TASK_METHOD_DECLARATION(GraphicsShutdownTask);
        TASK_METHOD_DECLARATION(RenderingTask);
	};

}

#endif
