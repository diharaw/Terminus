#pragma once

#include <json.hpp>

namespace terminus
{
	class Project
	{
	private:
		nlohmann::json _project_json;

	public:
		Project(const char* json);
		~Project();
	};
}