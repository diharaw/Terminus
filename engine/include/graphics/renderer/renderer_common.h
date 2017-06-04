#pragma once

#include <core/macro.h>
#include <core/types.h>

TERMINUS_BEGIN_NAMESPACE

// Uniform Slots

#define PER_FRAME_UNIFORM_SLOT 0
#define PER_DRAW_UNIFORM_SLOT 1
#define PER_DRAW_MATERIAL_UNIFORM_SLOT 2
#define PER_DRAW_BONE_OFFSETS_UNIFORM_SLOT 3

// Uniform Buffer Structs
struct TE_ALIGNED(16) PerFrameUniforms
{
	Matrix4 last_view_projection;
	Matrix4 view_projection;
	Matrix4 inverse_view_projection;
	Matrix4 projection;
	Matrix4 view;
	Matrix4 view_sky;
	Vector3 view_pos;
	Vector3 view_dir;
};

struct TE_ALIGNED(16) PerFrameSkyUniforms
{
	Matrix4 projection;
	Matrix4 view;
};

struct TE_ALIGNED(16) PerDrawUniforms
{
	Matrix4 model_view_projection;
	Matrix4 model;
	Vector3 position;
};

struct TE_ALIGNED(16) PerDrawMaterialUniforms
{
	Vector4 diffuse;
	float   roughness;
	float   metalness;
};

struct TE_ALIGNED(16) PerDrawBoneOffsetUniforms
{
	Matrix4 bone_offset[100];
};

struct RenderSubPass;
struct CommandBuffer;
struct UniformBuffer;
struct Material;

namespace renderer
{
	extern void bind_sub_pass(CommandBuffer* cmd_buf, RenderSubPass* sub_pass, UniformBuffer* uniform_buf, PerFrameUniforms* uniforms);
	extern void bind_material(CommandBuffer* cmd_buf, UniformBuffer* uniform_buf, Material* material, PerDrawMaterialUniforms* uniforms);
	extern void bind_draw_data(CommandBuffer* cmd_buf, UniformBuffer* uniform_buf, PerDrawUniforms* uniforms);
	extern void bind_bone_data(CommandBuffer* cmd_buf, UniformBuffer* uniform_buf, PerDrawBoneOffsetUniforms* uniforms);
}

TERMINUS_END_NAMESPACE