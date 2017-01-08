#pragma once

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <Graphics/render_common.h>
#include <Graphics/render_device.h>
#include <Resource/asset_common.h>
#include <Resource/shader_key.h>
#include <Utility/string_utility.h>

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
