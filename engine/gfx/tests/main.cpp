#include "render_device.hpp"
#include "application.hpp"
#include <cassert>
#include "types.hpp"
#include <SDL.h>

PROTOGFX_BEGIN_NAMESPACE

class TestApp : public Application
{
public:
	TestApp()
	{
		
	}

	~TestApp()
	{
		
	}

private:
	bool init() override
	{
		if (!device.init())
			return false;

        gfx::FramebufferCreateDesc desc;

		desc.num_render_targets = 1;

		gfx::Texture2DCreateDesc texture_2d_desc;

		texture_2d_desc.create_render_target_view = true;
		texture_2d_desc.data = nullptr;
		texture_2d_desc.format = gfx::TextureFormat::R8G8B8A8_UNORM;
		texture_2d_desc.generate_mipmaps = true;
		texture_2d_desc.height = 900;
		texture_2d_desc.width = 1600;
		texture_2d_desc.mipmap_levels = 10;

		m_rt = device.create_texture_2d(texture_2d_desc);
		assert(m_rt != nullptr);

		desc.render_targets[0] = m_rt;

		m_test_fbo = device.create_framebuffer(desc);
		assert(m_test_fbo != nullptr);
		
		Vertex cube[] = 
		{
			{ Vector3(-1.0f, -1.0f, 1.0f),  Vector2(0.0f, 1.0f) },
			{ Vector3(-1.0f, 1.0f, 1.0f),   Vector2(0.0f, 0.0f) },
			{ Vector3(1.0f, -1.0f, 1.0f),   Vector2(1.0f, 1.0f) },
			{ Vector3(1.0f, 1.0f, 1.0f),    Vector2(1.0f, 0.0f) },
			{ Vector3(-1.0f, -1.0f, -1.0f), Vector2(0.0f, 1.0f) },
			{ Vector3(-1.0f, 1.0f, -1.0f),  Vector2(0.0f, 0.0f) },
			{ Vector3(1.0f, -1.0f, -1.0f),  Vector2(1.0f, 1.0f) },
			{ Vector3(1.0f, 1.0f, -1.0f),   Vector2(1.0f, 0.0f) },
		};

		uint16_t indices[] = 
		{ 
			0,2,1,2,3,1,0,5,4,0,1,5,3,2,7,7,2,6,
			5,1,7,7,1,3,0,4,6,6,2,0,7,6,4,4,5,7
		};

        gfx::BufferCreateDesc vtx_desc =
		{
			cube,
			sizeof(Vertex) * 8,
            gfx::BufferUsageType::STATIC
		};

		vertex_buffer = device.create_vertex_buffer(vtx_desc);

        gfx::BufferCreateDesc idx_desc =
		{
			indices,
			sizeof(uint16_t) * 36,
            gfx::BufferUsageType::STATIC
		};

		index_buffer = device.create_index_buffer(idx_desc);

        gfx::InputElement elements[] =
		{
            { 3, gfx::DataType::FLOAT, false, 0                          , "POSITION" },
			{ 2, gfx::DataType::FLOAT, false, offsetof(Vertex, position) , "TEXCOORD" },
			{ 3, gfx::DataType::FLOAT, false, offsetof(Vertex, tex_coord), "NORMAL" },
			{ 4, gfx::DataType::FLOAT, false, offsetof(Vertex, normal)   , "TANGENT" }
		};
        
        gfx::InputLayoutCreateDesc ia_desc =
        {
            elements,
            sizeof(Vertex),
            4
        };
        
        m_input_layout = device.create_input_layout(ia_desc);

        gfx::VertexArrayCreateDesc vao_desc =
		{
			vertex_buffer,
			index_buffer,
			m_input_layout
		};

		vertex_array = device.create_vertex_array(vao_desc);

		return true;
	}

	void render() override
	{
		device.bind_framebuffer(nullptr);
        float clear[] = {1.0f, 0.0f, 0.0f, 1.0f};
        device.clear_framebuffer(gfx::ClearTarget::COLOR | gfx::ClearTarget::DEPTH, clear);
	}

	void frame(double dt) override
	{

	}

	void shutdown() override
	{
		device.destroy_vertex_array(vertex_array);
		device.destroy_framebuffer(m_test_fbo);
	}

private:
    gfx::Device       device;
    gfx::Framebuffer* m_test_fbo;
	gfx::ShaderProgram* m_program;
    gfx::InputLayout* m_input_layout;
	gfx::VertexArray* m_vao;
	gfx::Texture2D* m_rt;
	gfx::IndexBuffer*  index_buffer = nullptr;
	gfx::VertexBuffer* vertex_buffer = nullptr;
	gfx::VertexArray*  vertex_array = nullptr;

};

PROTOGFX_END_NAMESPACE

PROTOGFX_DECLARE_MAIN(protogfx::TestApp);
