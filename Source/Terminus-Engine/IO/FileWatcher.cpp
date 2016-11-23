#include "FileWatcher.h"
#include "FileSystem.h"
#include <iostream>

namespace Terminus { namespace IO {

	class UpdateListener : public FW::FileWatchListener
	{
	public:
		UpdateListener() {}
		void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
			FW::Action action)
		{
			// Fire event
			if (action == FW::Actions::Add)
			{
				std::cout << "File " << filename << " Added" << std::endl;
			}
			else if (action == FW::Actions::Modified)
			{
				std::cout << "File " << filename << " Modified" << std::endl;
			}
		}
	};

	FileWatcher::FileWatcher()
	{
		m_TicksSinceLastUpdate = 1;
		m_TicksPerUpdate = 1;
	}

	FileWatcher::~FileWatcher()
	{
	}

	void FileWatcher::AddDirectory(std::string _directory)
	{
#ifdef __APPLE__
		std::string cwd = FileSystem::get_current_working_directory();
		cwd += "/";
		std::string dir = cwd + _directory;
		m_Watcher.addWatch(dir, new UpdateListener(), true);
#else
		m_Watcher.addWatch(_directory, new UpdateListener(), true);
#endif
	}

	void FileWatcher::Update()
	{
		if (m_TicksSinceLastUpdate == m_TicksPerUpdate)
		{
			m_Watcher.update();
			m_TicksSinceLastUpdate = 1;
		}
		else
			m_TicksSinceLastUpdate++;
	}

	void FileWatcher::SetTicksPerUpdate(uint8 _ticks)
	{
		m_TicksPerUpdate = _ticks;
	}

	uint8 FileWatcher::GetTicksPerUpdate()
	{
		return m_TicksPerUpdate;
	}

} }