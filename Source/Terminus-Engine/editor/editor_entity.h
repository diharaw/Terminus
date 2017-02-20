#pragma once

#include <editor/component_desc.h>
#include <container/packed_array.h>
#include <types.h>

#define MAX_COMPONENT_DESC 20

namespace terminus
{
    struct EditorEntity
    {
        ComponentDesc* _components[MAX_COMPONENT_DESC];
        StringBuffer32 _name;
        ID             _entity_id;
        
        EditorEntity(StringBuffer32 name)
        {
            for(int i = 0 ; i < MAX_COMPONENT_DESC; i++)
                _components[i] = nullptr;
        }
        
        inline bool has_component(EditorComponentType type)
        {
            return (_components[type] != nullptr);
        }
        
        inline ComponentDesc* get_component_desc(EditorComponentType type)
        {
            return _components[type];
        }
        
        inline void add_component_desc(EditorComponentType type, ComponentDesc* desc)
        {
            _components[type] = desc;
        }
        
        inline void remove_component_desc(EditorComponentType type)
        {
            _components[type] = nullptr;
        }
    };
    
}
