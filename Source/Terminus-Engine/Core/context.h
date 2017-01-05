#pragma once

#include <Core/macro.h>
#include <Graphics/renderer.h>
#include <Graphics/render_device.h>
#include <Platform/platform.h>
#include <IO/FileSystem.h>
#include <ECS/SceneManager.h>
#include <Resource/SceneCache.h>
#include <Resource/ShaderCache.h>
#include <Resource/TextureCache.h>
#include <Resource/MeshCache.h>
#include <Resource/MaterialCache.h>

namespace terminus
{
    struct Context
    {
        // Platform
        Platform	 _platform;
        //FileSystem   _filesystem;
        
        // Graphics Systems
        Renderer 	 _renderer;
        RenderDevice _render_device;
        
        // Gameplay Systems
        SceneManager _scene_manager;
        
        // Resource Caches
        SceneCache    _scene_cache;
        ShaderCache   _shader_cache;
        MeshCache     _mesh_cache;
        MaterialCache _material_cache;
        TextureCache  _texture_cache;
    };
    
    namespace Global
    {
        extern Context& get_context();
    }
    
    namespace context
    {
        T_FORCE_INLINE Platform&       get_platform() 			{ return Global::get_context()._platform; }
        //T_FORCE_INLINE FileSystem&   get_filesystem() 			{ return get_context()._filesystem; }
        T_FORCE_INLINE Renderer&       get_renderer() 			{ return Global::get_context()._renderer; }
        T_FORCE_INLINE RenderDevice&   get_render_device()      { return Global::get_context()._render_device; }
        T_FORCE_INLINE SceneManager&   get_scene_manager()      { return Global::get_context()._scene_manager; }
        T_FORCE_INLINE SceneCache&     get_scene_cache()        { return Global::get_context()._scene_cache; }
        T_FORCE_INLINE ShaderCache&    get_shader_cache()       { return Global::get_context()._shader_cache; }
        T_FORCE_INLINE MeshCache&      get_mesh_cache()         { return Global::get_context()._mesh_cache; }
        T_FORCE_INLINE MaterialCache&  get_material_cache()     { return Global::get_context()._material_cache; }
        T_FORCE_INLINE TextureCache&   get_texture_cache()      { return Global::get_context()._texture_cache; }
        
        T_FORCE_INLINE Platform*     get_platform_ptr() 		{ return &Global::get_context()._platform; }
        T_FORCE_INLINE Renderer*     get_renderer_ptr()         { return &Global::get_context()._renderer; }
        T_FORCE_INLINE RenderDevice* get_render_device_ptr()    { return &Global::get_context()._render_device; }
        T_FORCE_INLINE SceneManager* get_scene_manager_ptr()    { return &Global::get_context()._scene_manager; }
    }
}
