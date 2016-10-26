#include "FileWatcher.h"
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
        }
    };
    
    uint8 m_TicksSinceLastUpdate = 0;
    uint8 m_TicksPerUpdate = 1;
    FW::FileWatcher m_Watcher;
    
    void Initialize()
    {
        
    }
    
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
        m_Watcher.addWatch(_directory, new UpdateListener(), true);
    }
    
    void Update()
    {
        if(m_TicksSinceLastUpdate == m_TicksPerUpdate)
        {
            m_Watcher.update();
            m_TicksSinceLastUpdate = 0;
        }
        else
            m_TicksSinceLastUpdate++;
    }
}