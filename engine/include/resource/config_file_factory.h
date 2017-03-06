#pragma once

#include <core/types.h>
#include <core/config_file.h>
#include <resource/text_loader.h>

namespace terminus
{
    namespace config_file_factory
    {
        extern ConfigFile* create(StringBuffer64 file_name);
    }
}
