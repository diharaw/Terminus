#pragma once

#include <core/include/terminus_macros.hpp>
#include <core/include/terminus_types.hpp>
#include <platform/include/platform_macros.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct WindowCreateDesc
{
    uint32_t   w;
    uint32_t   h;
    int32_t	   x;
    int32_t	   y;
    char	   title[100];
    uint32_t   flags;
};

class IWindow
{
public:
    IWindow() {}
    virtual ~IWindow() {}
    virtual bool initialize(WindowCreateDesc& desc) = 0;
    virtual void shutdown() = 0;
    virtual void hide() = 0;
    virtual void show() = 0;
    virtual uint32_t id() = 0;
    virtual void hide_cursor() = 0;
    virtual void show_cursor() = 0;
    virtual void* native_handle() = 0;
    virtual void* handle() = 0;
    virtual void resize(uint32_t w, uint32_t h) = 0;
    virtual void make_windowed() = 0;
    virtual void make_fullscreen() = 0;
    virtual void remove_border() = 0;
    virtual void add_border() = 0;
    virtual const char* title() = 0;
    virtual void set_title(const char* title) = 0;
    virtual bool shown() = 0;
};

TE_END_TERMINUS_NAMESPACE
