#include <graphics/renderer/scene_renderer.h>
#include <graphics/render_pass.h>
#include <core/frame_packet.h>
#include <io/logger.h>
#include <utility/profiler.h>

TERMINUS_BEGIN_NAMESPACE

SceneRenderer::SceneRenderer()
{

}

SceneRenderer::~SceneRenderer() 
{

}

void SceneRenderer::render(RenderSubPass* sub_pass, SceneRenderState* scene_state, SceneView* view)
{
	if (sub_pass && scene_state && view)
	{
		for (int i = 0; i < num_renderables; i++)
		{
			Renderable& renderable = renderable_array[i];

			for (int j = 0; j < renderable._mesh->MeshCount; j++)
			{
				int index = scene_view._num_items;
				DrawItem& draw_item = scene_view._draw_items[index];

				draw_item.uniforms = uniform_allocator->NewPerFrame<PerDrawUniforms>();
				draw_item.uniforms->model = *renderable._transform;
				draw_item.uniforms->position = *renderable._position;
				draw_item.uniforms->model_view_projection = *scene_view._view_projection_matrix * draw_item.uniforms->model;
				draw_item.base_index = renderable._mesh->SubMeshes[j].m_BaseIndex;
				draw_item.base_vertex = renderable._mesh->SubMeshes[j].m_BaseVertex;
				draw_item.index_count = renderable._mesh->SubMeshes[j].m_IndexCount;
				draw_item.material = renderable._mesh->SubMeshes[j]._material;
				draw_item.vertex_array = renderable._mesh->VertexArray;
				draw_item.type = renderable._type;
				draw_item.renderable_id = i;

				scene_view._num_items++;
			}
		}

		TERMINUS_END_CPU_PROFILE;

		// Sort DrawItem array

		TERMINUS_BEGIN_CPU_PROFILE(sort);

		std::sort(scene_view._draw_items.begin(),
				  scene_view._draw_items.begin() + scene_view._num_items,
				  DrawItemSort);

		TERMINUS_END_CPU_PROFILE;
	}
}

TERMINUS_END_NAMESPACE