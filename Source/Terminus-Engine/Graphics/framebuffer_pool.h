#pragma once

#include <Graphics/render_device.h>
#include <types.h>

#include <unordered_map>

namespace terminus
{
    class FramebufferPool
    {
    private:
        std::unordered_map<String, Framebuffer*> _fb_map;
        
    public:
        FramebufferPool();
        ~FramebufferPool();
        
        Framebuffer* create(String name);
        Framebuffer* lookup(String name);
    };
}
