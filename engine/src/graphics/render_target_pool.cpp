#include <graphics/render_target_pool.h>
#include <core/context.h>

namespace terminus
{
    struct CreateRenderTargetTaskData
    {
        Texture2D**   _render_target;
        uint16        _height;
        uint16        _width;
        TextureFormat _format;
    };
    
    void create_rendertarget_task(void* data)
    {
        CreateRenderTargetTaskData* task_data = (CreateRenderTargetTaskData*)data;
        RenderDevice& device = context::get_render_device();
        *task_data->_render_target = device.CreateTexture2D(task_data->_width, task_data->_height, nullptr, task_data->_format, true);
        
        T_LOG_INFO("Created render target");
    }
    
    RenderTargetPool::RenderTargetPool()
    {
        
    }
    
    RenderTargetPool::~RenderTargetPool()
    {
        
    }
    
    Texture* RenderTargetPool::create(String name, uint16 width, uint16 height, TextureFormat format)
    {
        if(_rt_map.find(name) == _rt_map.end())
        {
            Texture2D* render_target;
            
            Task texture_task;
            
            CreateRenderTargetTaskData* texture_task_data = task_data<CreateRenderTargetTaskData>(texture_task);
            texture_task_data->_render_target = &render_target;
            texture_task_data->_height = height;
            texture_task_data->_width = width;
            texture_task_data->_format = format;
            texture_task._function.Bind<&create_rendertarget_task>();
            submit_gpu_upload_task(texture_task);
            
            _rt_map[name] = render_target;
            return render_target;
        }
        else
            return _rt_map[name];
    }
    
    Texture* RenderTargetPool::lookup(String name)
    {
        if(_rt_map.find(name) == _rt_map.end())
            return nullptr;
        else
            return _rt_map[name];
    }
}
