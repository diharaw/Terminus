#pragma once

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include "../Graphics/RenderCommon.h"
#include "../Graphics/render_device.h"
#include "AssetCommon.h"
#include "ShaderKey.h"
#include "../Utility/StringUtility.h"

namespace terminus
{
	class ShaderFactory
	{
	private:
        String m_vertex_template;
        String m_pixel_template;

	public:
		ShaderFactory();
		~ShaderFactory();
		void Initialize(String vertex_template, String pixel_template);

		Shader* Create(AssetCommon::TextLoadData* _data);
        Shader* Create(StringList& _defines, ShaderType _type);
	};
}

#endif
