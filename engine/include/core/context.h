#pragma once

#include <core/macro.h>
#include <graphics/renderer.h>
#include <graphics/render_device.h>
#include <graphics/render_target_pool.h>
#include <resource/render_pass_cache.h>
#include <resource/rendering_path_cache.h>
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

namespace terminus
{    
    struct Context
    {
        // Engine Config
        ConfigFile*			_engine_config;
        
        // Platform
        Platform*			_platform;
        
        // Graphics Systems
        Renderer			_renderer;
        RenderDevice		_render_device;
        RenderTargetPool	_render_target_pool;
        FramebufferPool		_framebuffer_pool;
        
        // Gameplay Systems
        SceneManager		_scene_manager;
        
        // Physics Systems
        PhysicsEngineState  _physics_state;
        
        // Resource Caches
        SceneCache			_scene_cache;
        ShaderCache			_shader_cache;
        MeshCache			_mesh_cache;
        MaterialCache		_material_cache;
        TextureCache		_texture_cache;
        RenderPassCache		_render_pass_cache;
        RenderingPathCache  _rendering_path_cache;
        DynamicLibraryCache _dynamic_library_cache;
        LuaScriptFileCache  _lua_script_file_cache;
        
        // Thread Syncs
        Semaphore			_render_ready_sema;
        Semaphore			_main_ready_sema;
        Semaphore			_render_done_sema;
        Semaphore			_swap_done_sema;
        Semaphore			_render_exit_sema;
        Semaphore			_load_exit_sema;
        Semaphore			_load_wakeup_sema;
        
        // Threads
        RenderingThread		_rendering_thread;
        LoadingThread		_loading_thread;
        
		// Script
		ScriptEngine		_script_engine;
		ScriptInterfaceImpl _script_interface_impl;
        ImGuiBackend*       _imgui_backend;
        bool			    _shutdown;
    };
    
    namespace global
    {
        extern Context& get_context();
    }
    
    namespace context
    {
        T_FORCE_INLINE ConfigFile*			get_engine_config() 			{ return global::get_context()._engine_config; }
        T_FORCE_INLINE Platform*			get_platform() 					{ return global::get_context()._platform; }
        T_FORCE_INLINE ImGuiBackend*		get_imgui_backend() 			{ return global::get_context()._imgui_backend; }
        T_FORCE_INLINE Renderer&			get_renderer() 					{ return global::get_context()._renderer; }
        T_FORCE_INLINE RenderDevice&		get_render_device()				{ return global::get_context()._render_device; }
        T_FORCE_INLINE RenderTargetPool&	get_render_target_pool()		{ return global::get_context()._render_target_pool; }
        T_FORCE_INLINE FramebufferPool&		get_framebuffer_pool()			{ return global::get_context()._framebuffer_pool; }
        T_FORCE_INLINE PhysicsEngineState&	get_physics_state()             { return global::get_context()._physics_state; }
        T_FORCE_INLINE SceneManager&		get_scene_manager()				{ return global::get_context()._scene_manager; }
        T_FORCE_INLINE SceneCache&			get_scene_cache()				{ return global::get_context()._scene_cache; }
        T_FORCE_INLINE RenderPassCache&		get_render_pass_cache()			{ return global::get_context()._render_pass_cache; }
        T_FORCE_INLINE RenderingPathCache&	get_rendering_path_cache()		{ return global::get_context()._rendering_path_cache; }
        T_FORCE_INLINE DynamicLibraryCache& get_dynamic_library_cache()		{ return global::get_context()._dynamic_library_cache; }
        T_FORCE_INLINE LuaScriptFileCache&	get_lua_script_file_cache()		{ return global::get_context()._lua_script_file_cache; }
        T_FORCE_INLINE ShaderCache&			get_shader_cache()				{ return global::get_context()._shader_cache; }
        T_FORCE_INLINE MeshCache&			get_mesh_cache()				{ return global::get_context()._mesh_cache; }
        T_FORCE_INLINE MaterialCache&		get_material_cache()			{ return global::get_context()._material_cache; }
        T_FORCE_INLINE TextureCache&		get_texture_cache()				{ return global::get_context()._texture_cache; }
		T_FORCE_INLINE ScriptEngine&		get_script_engine()				{ return global::get_context()._script_engine; }
		T_FORCE_INLINE ScriptInterfaceImpl* get_script_interface_impl_ptr() { return &global::get_context()._script_interface_impl; }
        T_FORCE_INLINE Renderer*			get_renderer_ptr()				{ return &global::get_context()._renderer; }
        T_FORCE_INLINE RenderDevice*		get_render_device_ptr()			{ return &global::get_context()._render_device; }
        T_FORCE_INLINE SceneManager*		get_scene_manager_ptr()			{ return &global::get_context()._scene_manager; }
    }
}
