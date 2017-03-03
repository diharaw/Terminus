#pragma once

#ifndef SHADERFACTORY_H
#define SHADERFACTORY_H

#include <Graphics/render_common.h>
#include <Graphics/render_device.h>
#include <Resource/asset_common.h>
#include <Resource/shader_key.h>
#include <Utility/string_utility.h>

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

#endif
