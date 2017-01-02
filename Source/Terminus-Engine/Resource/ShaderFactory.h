#pragma once

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "../Graphics/RenderDevice.h"
#include "AssetCommon.h"
#include "ShaderKey.h"
#include "../Utility/StringUtility.h"

namespace Terminus { namespace Resource {

	class ShaderFactory
	{
	private:
		Graphics::RenderDevice* m_device;
        String m_vertex_template;
        String m_pixel_template;

	public:
		ShaderFactory();
		~ShaderFactory();
		void Initialize(Graphics::RenderDevice* device, String vertex_template, String pixel_template);

		Graphics::Shader* Create(AssetCommon::TextLoadData* _data);
        Graphics::Shader* Create(StringList& _defines, ShaderType _type);
	};

} }

#endif
