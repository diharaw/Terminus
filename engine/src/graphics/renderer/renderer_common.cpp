#include <graphics/renderer/renderer_common.h>
#include <graphics/render_pass.h>
#include <graphics/command_buffer.h>
#include <graphics/render_device.h>
#include <graphics/material.h>

TERMINUS_BEGIN_NAMESPACE

void bind_sub_pass(CommandBuffer* cmd_buf, RenderSubPass* sub_pass, UniformBuffer* uniform_buf, PerFrameUniforms* uniforms)
{
	uint32_t command_type;

	// Bind Pipeline State Object
	command_type = CommandBuffer::BindPipelineStateObject;

	cmd_buf->write(command_type);

	BindPipelineStateObjectData cmd0;
	cmd0.pso = sub_pass->pso;

	cmd_buf->write<BindPipelineStateObjectData>(cmd0);

	// Bind Framebuffer Command
	command_type = CommandBuffer::BindFramebuffer;
	cmd_buf->write(command_type);

	BindFramebufferCmdData cmd1;
	cmd1.framebuffer = sub_pass->framebuffer_target;

	cmd_buf->write<BindFramebufferCmdData>(cmd1);

	// Clear Framebuffer
	if (sub_pass->clear_types != FramebufferClearTarget::NONE)
	{
		command_type = CommandBuffer::ClearFramebuffer;
		cmd_buf->write(command_type);

		ClearFramebufferCmdData cmd2;
		cmd2.clear_target = (FramebufferClearTarget)sub_pass->clear_types;
		cmd2.clear_color = sub_pass->clear_color;

		cmd_buf->write<ClearFramebufferCmdData>(cmd2);
	}

	// Copy Per Frame data
	command_type = CommandBuffer::CopyUniformData;
	cmd_buf->write(command_type);

	CopyUniformCmdData cmd4;
	cmd4.buffer = uniform_buf;
	cmd4.data = uniforms;
	cmd4.size = sizeof(PerFrameUniforms);
	cmd4.map_type = BufferMapType::WRITE;

	cmd_buf->write<CopyUniformCmdData>(cmd4);

	// Bind Per Frame Global Uniforms
	command_type = CommandBuffer::BindUniformBuffer;
	cmd_buf->write(command_type);

	BindUniformBufferCmdData cmd3;
	cmd3.buffer = uniform_buf;
	cmd3.shader_type = ShaderType::VERTEX;
	cmd3.slot = PER_FRAME_UNIFORM_SLOT;

	cmd_buf->write<BindUniformBufferCmdData>(cmd3);
}

void bind_material(CommandBuffer* cmd_buf, UniformBuffer* uniform_buf, Material* material, PerDrawMaterialUniforms* uniforms)
{

}

void bind_draw_data(CommandBuffer* cmd_buf, UniformBuffer* uniform_buf, PerDrawUniforms* uniforms)
{

}

void bind_bone_data(CommandBuffer* cmd_buf, UniformBuffer* uniform_buf, PerDrawBoneOffsetUniforms* uniforms)
{

}

TERMINUS_END_NAMESPACE