#include "RenderPassCache.h"
#include <iostream>
#include "../IO/FileSystem.h"

namespace Terminus { namespace Resource {

	RenderPassCache::RenderPassCache()
	{

	}

	RenderPassCache::~RenderPassCache()
	{

	}

	void RenderPassCache::Initialize(Graphics::RenderDevice* device)
	{
		m_Factory.Initialize(device);
	}

	Graphics::RenderPass* RenderPassCache::Load(String key)
	{
		if (m_RenderPassMap.find(key) == m_RenderPassMap.end())
		{
			std::cout << "Asset not in Cache. Loading Asset." << std::endl;
			std::string extension = FileSystem::get_file_extention(key);

			if (m_LoaderMap.find(extension) == m_LoaderMap.end())
			{
				return nullptr;
			}
			else
			{
				AssetCommon::TextLoadData* data = static_cast<AssetCommon::TextLoadData*>(m_LoaderMap[extension]->Load(key));
				Graphics::RenderPass* render_pass = m_Factory.Create(data);
				m_RenderPassMap[key] = render_pass;
				return render_pass;
			}
		}
		else
		{
			std::cout << "Asset already in cache, returning reference.." << std::endl;
			return m_RenderPassMap[key];
		}
	}

	void RenderPassCache::Unload(Graphics::RenderPass* renderPass)
	{
		T_SAFE_DELETE(renderPass);
	}

} }