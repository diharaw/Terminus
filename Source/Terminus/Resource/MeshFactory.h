#pragma once

#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "Mesh.h"
#include "../Graphics/RenderDevice.h"
#include <string>

namespace Terminus { namespace Resource {

	class MeshFactory
	{
	private:
		Graphics::RenderDevice* m_device;

	public:
		MeshFactory();
		~MeshFactory();
		void Initialize(Graphics::RenderDevice* device);
		Mesh* Create(asset_common::MeshLoadData* _Data);
	};

} }

#endif