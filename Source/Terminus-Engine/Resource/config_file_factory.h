#pragma once

#include <Core/config_file.h>
#include <Resource/text_loader.h>
#include <types.h>

namespace terminus
{
    namespace config_file_factory
    {
        extern ConfigFile* create(StringBuffer64 file_name);
    }
}
