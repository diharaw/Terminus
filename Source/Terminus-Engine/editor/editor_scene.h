#pragma once

#include <editor/editor_entity.h>

#define MAX_EDITOR_ENTITIES 1024

namespace terminus
{
    class Scene;
    
    struct EditorScene
    {
        PackedArray<EditorEntity, MAX_EDITOR_ENTITIES> _entities;
        StringBuffer32 _name;
        
        EditorScene();
        
        ~EditorScene();
        
        void load_from_file(StringBuffer32 scene);
        
        EditorEntity& create_entity(StringBuffer32 name);
        
        void remove_entity(EditorEntity& entity);
        
    };
}
