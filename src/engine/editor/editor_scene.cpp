#include <editor/editor_scene.h>

namespace terminus
{
    EditorScene::EditorScene()
    {
        
    }
    
    EditorScene::~EditorScene()
    {
        
    }
    
    void EditorScene::load_from_file(StringBuffer32 scene)
    {
        
    }
    
    EditorEntity& EditorScene::create_entity(StringBuffer32 name)
    {
        ID entity_id = _entities.add();
        EditorEntity& entity = _entities.lookup(entity_id);
        entity._entity_id = entity_id;
        entity._name = name;
        
        return entity;
    }
    
    void EditorScene::remove_entity(EditorEntity& entity)
    {
        for(int i = 0; i < MAX_COMPONENT_DESC; i++)
            T_SAFE_DELETE(entity._components[i]);
        
        _entities.remove(entity._entity_id);
    }
}
