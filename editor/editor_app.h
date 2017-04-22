#pragma once

#include <core/application.h>

namespace terminus
{
    class EditorApp : public Application
    {
    public:
        EditorApp();
        virtual ~EditorApp();
        void run() override;
        
    protected:
        bool initialize_app() override;
        void shutdown_app() override;
    };
}
