#include "MeshFactory.h"

namespace Terminus { namespace Resource {

		MeshFactory::MeshFactory()
		{

		}

		MeshFactory::~MeshFactory()
		{

		}

		void MeshFactory::Initialize(Graphics::RenderDevice* device)
		{
			m_device = device;
		}

		Mesh* MeshFactory::Create(asset_common::MeshLoadData* _Data)
		{
			Graphics::IndexBuffer* indexBuffer = m_device->CreateIndexBuffer(&_Data->indices[0], sizeof(uint) * _Data->header.m_IndexCount, BufferUsageType::STATIC);
			Graphics::VertexBuffer* vertexBuffer;

			Mesh* mesh = new Mesh();

			if (_Data->IsSkeletal)
			{
				vertexBuffer = m_device->CreateVertexBuffer(&_Data->skeletalVertices[0], sizeof(SkeletalVertex) * _Data->header.m_VertexCount, BufferUsageType::STATIC);
				mesh->VertexArray = m_device->CreateVertexArray(vertexBuffer, indexBuffer, InputLayoutType::STANDARD_SKINNED_VERTEX);
			}
			else
			{
				vertexBuffer = m_device->CreateVertexBuffer(&_Data->skeletalVertices[0], sizeof(Vertex) * _Data->header.m_VertexCount, BufferUsageType::STATIC);
				mesh->VertexArray = m_device->CreateVertexArray(vertexBuffer, indexBuffer, InputLayoutType::STANDARD_VERTEX);
			}

			return mesh;
		}
} }
