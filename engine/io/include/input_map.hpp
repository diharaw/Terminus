#pragma once

#include <core/include/terminus_macros.hpp>
#include <io/include/io_macros.hpp>
#include <stl/include/string_hash.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class InputMap
{
public:
    virtual StringHash name()        = 0;
    virtual uint32_t num_states()    = 0;
    virtual uint32_t num_axis()      = 0;
    virtual uint32_t num_actions()   = 0;
    virtual void set_action(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad) = 0;
    virtual void set_state(uint64_t hash, int32_t mouse, int32_t keyboard, int32_t gamepad) = 0;
    virtual void set_axis(uint64_t hash, int32_t mouse, int32_t keyboard_pos, int32_t keyboard_neg, int32_t gamepad) = 0;
    virtual void clear_action(StringHash hash) = 0;
    virtual void clear_state(StringHash hash) = 0;
    virtual void clear_axis(StringHash hash) = 0;
};

TE_END_TERMINUS_NAMESPACE
