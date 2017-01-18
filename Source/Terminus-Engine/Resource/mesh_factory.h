#pragma once

#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <Resource/mesh.h>
#include <Graphics/render_device.h>
#include <global.h>
#include <string>

namespace terminus
{
    struct CreateMeshTaskData
    {
        Mesh* mesh;
        VertexBuffer* vertexBuffer;
        IndexBuffer* indexBuffer;
        InputLayoutType layoutType;
        InputLayout* layout;
        void* index_buffer_data;
        void* vertex_buffer_data;
        uint index_buffer_size;
        uint vertex_buffer_size;
        BufferUsageType usageType;
    };
    
	namespace mesh_factory
	{
		extern Mesh* create(asset_common::MeshLoadData* _Data);
        extern TASK_METHOD_DECLARATION(create_mesh_task);
	};
}

#endif
