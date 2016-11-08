#pragma once

#ifndef SHADERCACHE_H
#define SHADERCACHE_H

#include "AssetCache.h"
#include "AssetLoader.h"
#include "ShaderFactory.h"
#include <unordered_map>
#include "../Types.h"

namespace Terminus { namespace Resource {

	class ShaderCache : public IAssetCache<ShaderFactory>
	{
	private:
		std::unordered_map<std::string, Graphics::Shader*> m_ShaderMap;
		std::unordered_map<Graphics::Shader*, Graphics::ShaderProgram*> m_ShaderProgramMap;
		Graphics::RenderDevice* m_device;

	public:
		ShaderCache();
		~ShaderCache();
		void Initialize(Graphics::RenderDevice* device);

		Graphics::ShaderProgram* Load(const char* _vertexID,
									  const char* _pixelID,
									  const char* _geometryID = nullptr,
									  const char* _tessevalID = nullptr,
									  const char* _tesscontrolID = nullptr);
		void Unload(Graphics::ShaderProgram* program);
	};

} }

#endif
