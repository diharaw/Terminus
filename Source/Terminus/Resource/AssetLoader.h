#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <string>
#include <vector>

#define REGISTER_EXTENSION(x) m_Extensions.push_back(x);

class IAssetLoader
{
public:
	std::vector<std::string> m_Extensions;

public:
	IAssetLoader() {}
	virtual ~IAssetLoader() {}
	virtual void* Load(std::string _id) = 0;
};

#endif

