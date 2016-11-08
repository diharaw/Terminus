#include "RenderSystem.h"
#include "RenderCommon.h"

int			  RenderSystem::m_last_render_pass = -1;
int			  RenderSystem::m_last_render_subpass = -1;
RenderSubPass RenderSystem::m_render_subpass_pool[64];
RenderPass    RenderSystem::m_render_pass_pool[16];
DrawCommand	  RenderSystem::m_command_pool[MAX_DRAW_COMMANDS];
int			  RenderSystem::m_LastCommand = 0;

RenderPass* RenderSystem::create_render_pass()
{
	assert(m_last_render_pass < 16);
	m_last_render_pass++;
	return &m_render_pass_pool[m_last_render_pass];
}

RenderSubPass* RenderSystem::create_render_subpass()
{
	assert(m_last_render_subpass < 16);
	m_last_render_subpass++;
	return &m_render_subpass_pool[m_last_render_subpass];
}

DrawCommand* RenderSystem::create_draw_command()
{
	return &m_command_pool[m_LastCommand++];
}

void RenderSystem::update()
{
    for (int i = 0; i < m_last_render_pass - 1; i++)
    {
		RenderPass* pass = &m_render_pass_pool[i];

		for (int j = 0; j < m_render_pass_pool[i].m_SubPasses.size(); j++)
		{
			RenderSubPass* sub_pass = m_render_pass_pool[i].m_SubPasses[j];
			DrawCommand* cmd = create_draw_command();

			cmd->SetRenderPass(i);
			cmd->SetRenderSubpass(j);

			if (pass->m_geometry == GeometryType::SCENE)
			{
				
			}
			else if (pass->m_geometry == GeometryType::QUAD)
			{

			}
		}
    }
}