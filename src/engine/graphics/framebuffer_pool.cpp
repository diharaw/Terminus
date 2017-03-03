#include <Graphics/framebuffer_pool.h>
#include <Core/context.h>

namespace terminus
{
    struct CreateFramebufferTaskData
    {
        Framebuffer** _framebuffer;
    };
    
    void create_framebuffer_task(void* data)
    {
        CreateFramebufferTaskData* task_data = (CreateFramebufferTaskData*)data;
        RenderDevice& device = context::get_render_device();
        *task_data->_framebuffer = device.CreateFramebuffer();
        
        T_LOG_INFO("Created framebuffer");
    }
    
    FramebufferPool::FramebufferPool()
    {
        
    }
    
    FramebufferPool::~FramebufferPool()
    {
        
    }
    
    Framebuffer* FramebufferPool::create(String name)
    {
        if(_fb_map.find(name) == _fb_map.end())
        {
            Framebuffer* framebuffer;
            
            Task fbo_task;
            
            CreateFramebufferTaskData* fbo_task_data = task_data<CreateFramebufferTaskData>(fbo_task);
            fbo_task_data->_framebuffer = &framebuffer;
            fbo_task._function.Bind<&create_framebuffer_task>();
            
            submit_gpu_upload_task(fbo_task);
            
            _fb_map[name] = framebuffer;
            return framebuffer;
        }
        else
            return _fb_map[name];
    }
    
    Framebuffer* FramebufferPool::lookup(String name)
    {
        if(_fb_map.find(name) == _fb_map.end())
            return nullptr;
        else
            return _fb_map[name];
    }
}
