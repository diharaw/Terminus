#include "MaterialFactory.h"

namespace Terminus { namespace Resource {

	MaterialFactory::MaterialFactory()
	{

	}

	MaterialFactory::~MaterialFactory()
	{

	}

	void MaterialFactory::Initialize(Graphics::RenderDevice* device)
	{
		m_device = device;
	}

	Graphics::Material* MaterialFactory::Create(asset_common::TextLoadData* _Data)
	{

	}

} }