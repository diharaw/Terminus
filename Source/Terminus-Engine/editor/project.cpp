#include <editor/project.h>
#include <IO/filesystem.h>
#include <Core/context.h>

#include <prettywriter.h>

namespace terminus
{
	Project::Project()
	{

	}

	Project::~Project()
	{

	}

	void Project::create(const char* file_name, const char* path)
	{
		doc.SetObject();
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

		rapidjson::Value val(file_name, doc.GetAllocator());
		doc.AddMember("project_name", val, doc.GetAllocator());

		doc.Accept(writer);

		String scene_path = String(path);
		scene_path += "/";
		scene_path += String(file_name);
		scene_path += ".json";

		if (filesystem::write_begin(scene_path))
		{
			filesystem::write((void*)buffer.GetString(), buffer.GetSize(), 1, 0);
			filesystem::write_end();
		}
	}

	void Project::load(const char* file_name)
	{
		FileHandle handle = filesystem::read_file(String(file_name), true, true);
		doc.Parse(handle.buffer);

		rapidjson::Value& scenes = doc["scenes"];

		if (scenes.Size() > 0)
			context::get_scene_manager().load(String(scenes[0].GetString()));

		free(handle.buffer);
	}

	void Project::save()
	{

	}
}