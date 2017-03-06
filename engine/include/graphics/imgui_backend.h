#pragma once

#include <imgui.h>

namespace terminus
{
    class ImGuiBackend
    {
    public:
        ImGuiBackend() {}
        virtual ~ImGuiBackend() {}
        virtual void initialize() = 0;
        virtual void new_frame() = 0;
        virtual void shutdown() = 0;
        virtual void render() = 0;
    };
}
