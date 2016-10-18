#pragma once

#ifndef ASSETCACHE_H
#define ASSETCACHE_H

#include "../Memory/StackAllocator.h"
#include "../GlobalMemory.h"
#include "AssetLoader.h"
#include <string>
#include <unordered_map>

// Forward Declaration
class  IAssetFactory;
class  IAssetLoader;
struct IAsset;

template<class FactoryType>
class IAssetCache
{
protected:
    Terminus::Allocator* m_Allocator;
	FactoryType			 m_Factory;
	std::vector<IAssetLoader*>		 m_LoaderList;
	std::unordered_map<std::string, IAssetLoader*> m_LoaderMap;
    
public:
    IAssetCache(Terminus::Allocator* _allocator = nullptr) : m_Allocator(_allocator) {}
    ~IAssetCache() { }
    
	template<class LoaderType>
	void RegisterLoader()
	{
		IAssetLoader* loader = new LoaderType();
		m_LoaderList.push_back(loader);

		for (std::string ext : loader->m_Extensions)
			m_LoaderMap[ext] = loader;
	}
};

#endif