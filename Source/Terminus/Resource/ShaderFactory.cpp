#include "ShaderFactory.h"

namespace Terminus { namespace Resource {

	ShaderFactory::ShaderFactory()
	{

	}

	ShaderFactory::~ShaderFactory()
	{

	}

	void ShaderFactory::Initialize(Terminus::Graphics::RenderDevice* device)
	{
		m_device = device;
	}

	Graphics::Shader* ShaderFactory::Create(asset_common::TextLoadData* _data)
	{
		Graphics::Shader* shader = m_device->CreateShader(_data->shader_type, _data->buffer);

		if (!shader)
			return nullptr;

		return shader;
	}

} }
