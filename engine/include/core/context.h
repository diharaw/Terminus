#pragma once

#include <core/macro.h>
#include <graphics/renderer.h>
#include <graphics/render_device.h>
#include <graphics/render_target_pool.h>
#include <resource/render_pass_cache.h>
#include <resource/rendering_path_cache.h>
#include <resource/pipeline_state_object_cache.h>
#include <graphics/framebuffer_pool.h>
#include <platform/platform.h>
#include <io/filesystem.h>
#include <thread/semaphore.h>
#include <thread/thread_pool.h>
#include <gameplay/scene_manager.h>
#include <resource/scene_cache.h>
#include <resource/shader_cache.h>
#include <resource/texture_cache.h>
#include <resource/mesh_cache.h>
#include <resource/material_cache.h>
#include <resource/dynamic_library_cache.h>
#include <resource/lua_script_file_cache.h>
#include <core/rendering_thread.h>
#include <core/loading_thread.h>
#include <core/config_file.h>
#include <script/script_engine.h>
#include <script/script_interface_impl.h>
#include <physics/physics_engine.h>
#include <graphics/imgui_backend.h>
#include <core/frame_packet_pool.h>

#define MAX_ENGINE_OBJECTS 30

TERMINUS_BEGIN_NAMESPACE

struct Context
{
    // Engine Config
    ConfigFile*				 _engine_config;
    						 
    // Platform				 
    Platform*				 _platform;
    						 
    // Graphics Systems		 
    Renderer				 _renderer;
    RenderDevice			 _render_device;
    RenderTargetPool		 _render_target_pool;
    FramebufferPool			 _framebuffer_pool;
    						 
    // Gameplay Systems		 
    SceneManager			 _scene_manager;
    						 
    // Resource Caches		 
    SceneCache				 _scene_cache;
    ShaderCache				 _shader_cache;
    MeshCache				 _mesh_cache;
    MaterialCache			 _material_cache;
    TextureCache			 _texture_cache;
    RenderPassCache			 _render_pass_cache;
    RenderingPathCache		 _rendering_path_cache;
    DynamicLibraryCache		 _dynamic_library_cache;
    LuaScriptFileCache		 _lua_script_file_cache;
    PipelineStateObjectCache _pso_cache;
	FramePacketPool			 _frame_packet_pool;
    
	PhysicsEngine			 _physics_engine;

    // Threads
    RenderingThread			 _rendering_thread;
    LoadingThread			 _loading_thread;
    						 
	// Script				 
	ScriptEngine			 _script_engine;
	ScriptInterfaceImpl		 _script_interface_impl;
    ImGuiBackend*			 _imgui_backend;
    bool					 _shutdown;
};

namespace global
{
    extern Context& get_context();
}

class ContextTypeID
{
private:
    static uint16_t counter;
    
public:
    template<typename T>
    static uint16_t get()
    {
        static uint16_t id = counter++;
        return id;
    }
};

namespace context
{
    static void* _map[MAX_ENGINE_OBJECTS];
    
    template<typename T>
    static void register_object(T& object)
    {
        _map[ContextTypeID::get<T>()] = &object;
    }
    
    template<typename BaseClass, typename T>
    static void register_object_base(T& object)
    {
        _map[ContextTypeID::get<BaseClass>()] = &object;
    }
    
    template<typename T>
    static void unregister_object()
    {
        _map[ContextTypeID::get<T>()] = nullptr;
    }
    
    template<typename T>
    static T* get_object()
    {
        return (T*)_map[ContextTypeID::get<T>()];
    }
    
    TE_FORCE_INLINE ConfigFile*			get_engine_config() 			{ return global::get_context()._engine_config; }
    TE_FORCE_INLINE Platform*			get_platform() 					{ return global::get_context()._platform; }
    TE_FORCE_INLINE ImGuiBackend*		get_imgui_backend() 			{ return global::get_context()._imgui_backend; }
    TE_FORCE_INLINE Renderer&			get_renderer() 					{ return global::get_context()._renderer; }
    TE_FORCE_INLINE RenderDevice&		get_render_device()				{ return global::get_context()._render_device; }
    TE_FORCE_INLINE RenderTargetPool&	get_render_target_pool()		{ return global::get_context()._render_target_pool; }
    TE_FORCE_INLINE FramebufferPool&		get_framebuffer_pool()			{ return global::get_context()._framebuffer_pool; }
    TE_FORCE_INLINE PhysicsEngine&		get_physics_engine()            { return global::get_context()._physics_engine; }
    TE_FORCE_INLINE SceneManager&		get_scene_manager()				{ return global::get_context()._scene_manager; }
    TE_FORCE_INLINE SceneCache&			get_scene_cache()				{ return global::get_context()._scene_cache; }
    TE_FORCE_INLINE RenderPassCache&		get_render_pass_cache()			{ return global::get_context()._render_pass_cache; }
    TE_FORCE_INLINE RenderingPathCache&	get_rendering_path_cache()		{ return global::get_context()._rendering_path_cache; }
    TE_FORCE_INLINE DynamicLibraryCache& get_dynamic_library_cache()		{ return global::get_context()._dynamic_library_cache; }
    TE_FORCE_INLINE LuaScriptFileCache&	get_lua_script_file_cache()		{ return global::get_context()._lua_script_file_cache; }
    TE_FORCE_INLINE PipelineStateObjectCache&	get_pipeline_state_object_cache()		{ return global::get_context()._pso_cache; }
    TE_FORCE_INLINE ShaderCache&			get_shader_cache()				{ return global::get_context()._shader_cache; }
    TE_FORCE_INLINE MeshCache&			get_mesh_cache()				{ return global::get_context()._mesh_cache; }
    TE_FORCE_INLINE MaterialCache&		get_material_cache()			{ return global::get_context()._material_cache; }
    TE_FORCE_INLINE TextureCache&		get_texture_cache()				{ return global::get_context()._texture_cache; }
	TE_FORCE_INLINE ScriptEngine&		get_script_engine()				{ return global::get_context()._script_engine; }
	TE_FORCE_INLINE ScriptInterfaceImpl* get_script_interface_impl_ptr() { return &global::get_context()._script_interface_impl; }
    TE_FORCE_INLINE Renderer*			get_renderer_ptr()				{ return &global::get_context()._renderer; }
    TE_FORCE_INLINE RenderDevice*		get_render_device_ptr()			{ return &global::get_context()._render_device; }
    TE_FORCE_INLINE SceneManager*		get_scene_manager_ptr()			{ return &global::get_context()._scene_manager; }
}

TERMINUS_END_NAMESPACE
