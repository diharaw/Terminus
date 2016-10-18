#pragma once

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "RenderPass.h"
#include "CommandList.h"
#include <assert.h>

#define MAX_DRAW_COMMANDS 1024

class RenderSystem
{
private:
	static RenderSubPass m_render_subpass_pool[64];
	static RenderPass    m_render_pass_pool[16];
	static DrawCommand	 m_command_pool[MAX_DRAW_COMMANDS];
	static int			 m_LastCommand;

	static int m_last_render_pass;
	static int m_last_render_subpass;

public:
	static RenderPass* create_render_pass();
	static RenderSubPass* create_render_subpass();
	static DrawCommand* create_draw_command();
    static void update();
};

#endif
