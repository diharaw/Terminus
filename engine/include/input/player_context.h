#pragma once

#include <Input/input_map.h>
#include <container/handle_array.h>

#define MAX_INPUT_MAPS 5

namespace terminus
{
    using InputMapHandle = Handle<InputMap>;
    
    struct PlayerContext
    {
        uint8                                 _gamepad_index;
        InputMapHandle                        _active;
        HandleArray<InputMap, MAX_INPUT_MAPS> _input_maps;
    };
}
