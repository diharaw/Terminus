#include "SceneFactory.h"

namespace Terminus { namespace Resource {

	SceneFactory::SceneFactory()
	{

	}

	SceneFactory::~SceneFactory()
	{

	}

	ECS::Scene* SceneFactory::Create(AssetCommon::TextLoadData* _data)
	{
		JsonDocument doc;
		doc.Parse(_data->buffer);
	}

} }