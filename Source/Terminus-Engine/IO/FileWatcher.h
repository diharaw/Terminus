#include "../Types.h"
#include <string>

namespace FileWatcher
{
    /**
     * Set the interval at which the FileSystem will be checked for changes.
     * @param _ticks Number of Ticks for each Update
     */
    void set_ticks_per_update(uint8 _ticks);
    /**
     * Returns the current Update Rate.
     * @return uint8 Ticks per update.
     */
    uint8 get_ticks_per_update();
    /**
     * Adds a directory to be watched.
     * @param _directory Name of directory.
     */
    void add_directory(std::string _directory);
    /**
     * Updates the file watcher.
     */
    void update();
}
