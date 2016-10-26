#include "../Types.h"
#include <string>

namespace FileWatcher
{
    void Initialize();
    void SetTicksPerUpdate(uint8 _ticks);
    uint8 GetTicksPerUpdate();
    void AddDirectory(std::string _directory);
    void Update();
}