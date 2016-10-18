#include "MeshFactory.h"
#include "../Graphics/RenderBackend.h"

MeshFactory::MeshFactory()
{

}

MeshFactory::~MeshFactory()
{

}

Mesh* MeshFactory::Create(asset_common::MeshLoadData* _Data)
{
	IndexBuffer* indexBuffer = RenderBackend::CreateIndexBuffer(&_Data->indices[0], sizeof(uint) * _Data->header.m_IndexCount, USAGE_STATIC);
	VertexBuffer* vertexBuffer;

	Mesh* mesh = new Mesh();

	if (_Data->IsSkeletal) 
	{
		vertexBuffer = RenderBackend::CreateVertexBuffer(&_Data->skeletalVertices[0], sizeof(SkeletalVertex) * _Data->header.m_VertexCount, USAGE_STATIC);
		mesh->VAOHandle = RenderBackend::CreateVertexArray(vertexBuffer, indexBuffer, LAYOUT_STANDARD_SKINNED_VERTEX);
	}
	else
	{
		vertexBuffer = RenderBackend::CreateVertexBuffer(&_Data->skeletalVertices[0], sizeof(Vertex) * _Data->header.m_VertexCount, USAGE_STATIC);
		mesh->VAOHandle = RenderBackend::CreateVertexArray(vertexBuffer, indexBuffer, LAYOUT_STANDARD_VERTEX);
	}
		
	return mesh;
}