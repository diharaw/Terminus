#pragma once

#include <core/application.h>

namespace terminus
{
    class RuntimeApp : public Application
    {
    public:
        RuntimeApp();
        virtual ~RuntimeApp();
        void run() override;
        
    protected:
        bool initialize_app() override;
        void shutdown_app() override;
        
    private:
        void load_initial_scene();
    };
}
