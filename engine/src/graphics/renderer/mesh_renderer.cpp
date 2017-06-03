#include <graphics/renderer/mesh_renderer.h>
#include <graphics/render_device.h>
#include <graphics/render_pass.h>
#include <graphics/renderer/renderable.h>
#include <resource/mesh.h>
#include <core/frame_packet.h>
#include <graphics/renderer/renderer_common.h>
#include <graphics/draw_item.h>
#include <io/logger.h>
#include <utility/profiler.h>

TERMINUS_BEGIN_NAMESPACE

bool draw_item_sort(DrawItem i, DrawItem j)
{
	return i.sort_key.key > j.sort_key.key;
}

MeshRenderer::MeshRenderer()
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::render(RenderSubPass* sub_pass, SceneRenderState* scene_state, SceneView* view)
{
	if (sub_pass && scene_state && view)
	{
		view->allocator->Clear();

		DrawItem* draw_item_array = &m_draw_items[view->scene_index][0];
		uint32_t& index = m_num_draw_items[view->scene_index];
		index = 0;

		for (int i = 0; i < scene_state->num_renderables; i++)
		{
			StaticRenderable& renderable = scene_state->static_renderables[i];

			for (int j = 0; j < renderable.mesh->mesh_count; j++)
			{
				DrawItem& draw_item = draw_item_array[index];
				draw_item.uniforms = TE_NEW(view->allocator) PerDrawUniforms();
				draw_item.uniforms->model = renderable.transform;
				draw_item.uniforms->position = renderable.position;
				draw_item.uniforms->model_view_projection = view->view_projection_matrix * draw_item.uniforms->model;
				draw_item.base_index = renderable.mesh->sub_meshes[j].base_index;
				draw_item.base_vertex = renderable.mesh->sub_meshes[j].base_vertex;
				draw_item.index_count = renderable.mesh->sub_meshes[j].index_count;
				draw_item.material = renderable.mesh->sub_meshes[j].material;
				draw_item.vertex_array = renderable.mesh->vertex_array;
				draw_item.type = renderable.type;
				draw_item.renderable_id = i;

				index++;
			}
		}

		// Sort DrawItem array

		TERMINUS_BEGIN_CPU_PROFILE(sort);

		std::sort(std::begin(m_draw_items[view->scene_index]),
				  std::begin(m_draw_items[view->scene_index]) + index,
				  draw_item_sort);

		TERMINUS_END_CPU_PROFILE;
	}
}

TERMINUS_END_NAMESPACE