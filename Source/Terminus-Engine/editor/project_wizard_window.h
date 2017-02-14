#pragma once

#include <editor/ieditor_window.h>
#include <types.h>

namespace terminus
{
    class ProjectWizardWindow : public IEditorWindow
    {
    public:
        ProjectWizardWindow(std::string title, Vector2 rel_size, Vector2 size, Vector2 rel_pos, Vector2 pos, int flags);
        virtual ~ProjectWizardWindow();
        virtual void window_contents(double dt) override;
        virtual void initialize() override;
        
    private:
        StringBuffer32  _project_name;
        StringBuffer128 _save_path;
    };
}
