#pragma once

#include <graphics/render_common.h>
#include <graphics/render_device.h>
#include <resource/asset_common.h>
#include <resource/shader_key.h>
#include <utility/string_utility.h>

namespace terminus
{
    namespace shader_factory
    {
        extern Shader* create(asset_common::TextLoadData* data);
        extern Shader* create(StringList& defines, ShaderType type);
        extern Shader* create(ShaderType type, String shader_template, StringList defines);
        extern ShaderProgram* create_program(Shader* vs, Shader* ps, Shader* tcs, Shader* tes, Shader* gs);
    }
}
