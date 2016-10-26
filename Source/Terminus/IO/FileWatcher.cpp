#include "FileWatcher.h"
#include "FileSystem.h"
#include <iostream>
#include <include/FileWatcher/FileWatcher.h>

namespace FileWatcher
{
    class UpdateListener : public FW::FileWatchListener
    {
    public:
        UpdateListener() {}
        void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
                              FW::Action action)
        {
            // Fire event
            if(action == FW::Actions::Add)
            {
                std::cout << "File " << filename << " Added" << std::endl;
            }
            else if(action == FW::Actions::Modified)
            {
                std::cout << "File " << filename << " Modified" << std::endl;
            }
        }
    };
    
    uint8 m_TicksSinceLastUpdate = 1;
    uint8 m_TicksPerUpdate = 1;
    FW::FileWatcher m_Watcher;
        
    void SetTicksPerUpdate(uint8 _ticks)
    {
        m_TicksPerUpdate = _ticks;
    }
    
    uint8 GetTicksPerUpdate()
    {
        return m_TicksPerUpdate;
    }
    
    void AddDirectory(std::string _directory)
    {
#ifdef __APPLE__
        std::string cwd = FileSystem::GetCurrentWorkingDirectory();
        cwd += "/";
        std::string dir = cwd + _directory;
        m_Watcher.addWatch(dir, new UpdateListener(), true);
#else
        m_Watcher.addWatch(_directory, new UpdateListener(), true);
#endif
    }
    
    void Update()
    {
        if(m_TicksSinceLastUpdate == m_TicksPerUpdate)
        {
            m_Watcher.update();
            m_TicksSinceLastUpdate = 1;
        }
        else
            m_TicksSinceLastUpdate++;
    }
}