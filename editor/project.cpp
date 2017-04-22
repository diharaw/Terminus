#include "project.h"

namespace terminus
{
	Project::Project(const char* json)
	{
		_project_json.parse(json);
	}

	Project::~Project()
	{

	}
}