#include "MaterialFactory.h"

namespace terminus { namespace Resource {

	MaterialFactory::MaterialFactory()
	{

	}

	MaterialFactory::~MaterialFactory()
	{

	}

	Graphics::Material* MaterialFactory::Create(AssetCommon::TextLoadData* data, TextureCache* textureCache)
	{
		JsonDocument doc;
		doc.Parse(data->buffer);

		Graphics::Material* material = new Graphics::Material();

		if (doc.HasMember("diffuse_map"))
		{
			String key = std::string(doc["diffuse_map"].GetString());
			material->texture_maps[Graphics::DIFFUSE] = (Graphics::Texture2D*)textureCache->Load(key);
		}
		else
		{
			assert(doc.HasMember("diffuse_value"));
			rapidjson::Value& val = doc["diffuse_value"];

			Vector4 color;
			color.r = val["r"].GetFloat();
			color.g = val["g"].GetFloat();
			color.b = val["b"].GetFloat();
			color.a = val["a"].GetFloat();

			material->diffuse_value = color;
		}

		if (doc.HasMember("normal_map"))
		{
			String key = std::string(doc["normal_map"].GetString());
			material->texture_maps[Graphics::NORMAL] = (Graphics::Texture2D*)textureCache->Load(key);
		}

		if (doc.HasMember("roughness_map"))
		{
			String key = std::string(doc["roughness_map"].GetString());
			material->texture_maps[Graphics::ROUGHNESS] = (Graphics::Texture2D*)textureCache->Load(key);
		}
		else
		{
			assert(doc.HasMember("roughness_value"));
			material->roughness_value = doc["roughness_value"].GetFloat();
		}

		if (doc.HasMember("metalness_map"))
		{
			String key = std::string(doc["metalness_map"].GetString());
			material->texture_maps[Graphics::METALNESS] = (Graphics::Texture2D*)textureCache->Load(key);
		}
		else
		{
			assert(doc.HasMember("metalness_value"));
			material->metalness_value = doc["metalness_value"].GetFloat();
		}

		if (doc.HasMember("displacement_map"))
		{
			String key = std::string(doc["displacement_map"].GetString());
			material->texture_maps[Graphics::DISPLACEMENT] = (Graphics::Texture2D*)textureCache->Load(key);
		}

		return material;
	}

} }
