#pragma once

#include <core/types.h>
#include <string>
#include <include/FileWatcher/FileWatcher.h>

namespace terminus
{
	class FileWatcher
	{
	private:
		uint8				    m_TicksSinceLastUpdate;
		uint8					m_TicksPerUpdate;
		FW::FileWatcher m_Watcher;

	public:
		FileWatcher();
		~FileWatcher();
		/**
		* Set the interval at which the FileSystem will be checked for changes.
		* @param _ticks Number of Ticks for each Update
		*/
		void SetTicksPerUpdate(uint8 ticks);
		/**
		* Returns the current Update Rate.
		* @return uint8 Ticks per update.
		*/
		uint8 GetTicksPerUpdate();
		/**
		* Adds a directory to be watched.
		* @param _directory Name of directory.
		*/
		void AddDirectory(std::string directory);
		/**
		* Updates the file watcher.
		*/
		void Update();
	};
} // namespace terminus
