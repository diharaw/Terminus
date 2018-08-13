#include <iostream>
#include <core/application.hpp>
#include <core/engine_core.hpp>
#include <core/terminus_macros.hpp>
#include <io/reflection.hpp>
#include <io/json_serializer.hpp>
#include <io/binary_serializer.hpp>
#include <io/memory_stream.hpp>
#include <io/file_stream.hpp>
#include <io/logger.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

void on_state_input(Event* e)
{
	if (e->data.input_state.hash == TE_HASH("fire"))
	{
		if (e->data.input_state.state == 1)
			std::cout << "Fire State ON" << std::endl;
		else
			std::cout << "Fire State OFF" << std::endl;
	}
}

void on_action_input(Event* e)
{
	if (e->data.input_action.hash == TE_HASH("block"))
		std::cout << "Block action" << std::endl;
}

void on_axis_input(Event* e)
{
	if (e->data.input_axis.hash == TE_HASH("forward"))
	{
		std::cout << "Forward axis : " << e->data.input_axis.value << std::endl;
	}
}

struct Bar
{
	int a;
	float b;

	REFLECT()
};

struct Foo
{
	Vector<Bar> test;
	Array<int, 10> list;
	StaticHashMap<StringBuffer32, int, 32> map;

	REFLECT()
};

BEGIN_DECLARE_REFLECT(Bar)
	REFLECT_MEMBER(a)
	REFLECT_MEMBER(b)
END_DECLARE_REFLECT()

BEGIN_DECLARE_REFLECT(Foo)
	REFLECT_MEMBER(test)
	REFLECT_MEMBER(list)
	REFLECT_MEMBER(map)
END_DECLARE_REFLECT()

void create_foo(Foo& foo)
{
	foo.list.resize(10);

	for (int i = 0; i < 10; i++)
		foo.list[i] = rand();

	for (int i = 0; i < 10; i++)
	{
		Bar obj;
		obj.a = rand();
		obj.b = rand();
		foo.test.push_back(obj);
	}

	foo.map.set("Jesse", 434);
	foo.map.set("Walter", 85);
}

void test_bin_serialize_fs(const Foo& foo)
{
	File* f = global::filesystem().open_file("test_f.bin", TE_FS_WRITE | TE_FS_BINARY);

	FileStream stream(f);
	BinarySerializer serializer(stream);

	serializer.save(foo);
	serializer.flush_to_stream();

	global::filesystem().close_file(f);
}

void test_bin_deserialize_fs()
{
	File* f = global::filesystem().open_file("test_f.bin", TE_FS_READ | TE_FS_BINARY);

	FileStream stream(f);
	BinarySerializer serializer(stream);

	Foo foo;
	serializer.load(foo);

	global::filesystem().close_file(f);
}

void test_serialize_fs(const Foo& foo)
{
	File* f = global::filesystem().open_file("test_f.json", TE_FS_WRITE | TE_FS_BINARY);

	FileStream stream(f);
	JsonSerializer serializer(stream);

	serializer.save(foo);
	serializer.flush_to_stream();

	global::filesystem().close_file(f);
}

void test_deserialize_fs()
{
	File* f = global::filesystem().open_file("test_f.json", TE_FS_READ | TE_FS_BINARY);

	FileStream stream(f);
	JsonSerializer serializer(stream);

	serializer.print();

	Foo foo;
	serializer.load(foo);

	global::filesystem().close_file(f);
}

void test_serialize_ms(const Foo& foo)
{
	File* f = global::filesystem().open_file("test_m.json", TE_FS_WRITE | TE_FS_BINARY);

	MemoryStream stream;
	JsonSerializer serializer(stream);

	serializer.save(foo);
	serializer.flush_to_stream();

	f->write(stream.data(), stream.size(), 1);
	global::filesystem().close_file(f);
}

void test_deserialize_ms()
{
	File* f = global::filesystem().open_file("test_m.json", TE_FS_READ | TE_FS_BINARY);

	size_t size = f->size();
	char* buf = (char*)TE_HEAP_ALLOC(size + 1);
	f->read(buf, size, 1);
	buf[size] = '\0';

	MemoryStream stream(buf, size);
	JsonSerializer serializer(stream);

	serializer.print();

	Foo foo;
	serializer.load(foo);

	global::filesystem().close_file(f);

	TE_HEAP_DEALLOC(buf);
}

class Runtime : public Application
{
public:
	const uint32_t  m_frame_count = 3;
	InputMap		input_map;
	PipelineState*  m_pso;
	PipelineLayout* m_pipeline_layout;
	VertexArray*    m_vao;
	CommandBuffer*	m_command_buffers[3];
	CommandPool*	m_command_pools[3];
	Fence*			m_fence[3];
	Shader*			m_vs;
	Shader*			m_fs;
	SemaphoreGPU*   m_image_available_sema;
	SemaphoreGPU*	m_render_finished_sema;
	Framebuffer*	m_fbo;
	uint32_t		m_frame_index;

	Runtime()
	{
		m_title = "Terminus Runtime";
		m_width = 1280;
		m_height = 720;
		m_window_flags |= TE_WINDOW_RESIZABLE;

		m_frame_index = 0;
	}

	~Runtime()
	{

	}

	bool initialize() override
	{
		InputManager& input_manager = global::input_manager();
		EventManager& event_manager = global::event_manager();

		uint32_t player = input_manager.create_player();

		input_map.set_action(TE_HASH("block"), TE_MOUSE_LEFT, TE_UNASSIGNED_INPUT, TE_UNASSIGNED_INPUT);
		input_map.set_state(TE_HASH("fire"), TE_UNASSIGNED_INPUT, TE_KEYBOARD_A, TE_UNASSIGNED_INPUT);
		input_map.set_axis(TE_HASH("forward"), TE_UNASSIGNED_INPUT, TE_KEYBOARD_W, TE_KEYBOARD_S, TE_UNASSIGNED_INPUT);

		input_manager.set_input_map(&input_map, player);

		uint32_t callback_hndl = event_manager.register_callback(TE_EVENT_STATE_INPUT, on_state_input);

		event_manager.register_callback(TE_EVENT_AXIS_INPUT, on_axis_input);
		event_manager.register_callback(TE_EVENT_ACTION_INPUT, on_action_input);

		Foo foo;
		create_foo(foo);

		test_serialize_fs(foo);
		test_deserialize_fs();

		//test_serialize_ms();
		//test_deserialize_ms();

		test_bin_serialize_fs(foo);
		test_bin_deserialize_fs();

		if (!intialize_graphics())
			return false;

		return true;
	}

	void update() override
	{
		render();
	}

	void shutdown() override
	{
		shutdown_graphics();
	}

private:
	bool load_shaders()
	{
		{
			File* vs = global::filesystem().open_file("vert.spv", TE_FS_READ | TE_FS_BINARY);

			if (!vs)
			{
				TE_LOG_ERROR("Failed to load Vertex Shader!");
				return false;
			}

			size_t size = vs->size();
			void* vs_src = TE_HEAP_ALLOC(size);
			vs->read(vs_src, size, 1);

			global::filesystem().close_file(vs);

			BinaryShaderCreateDesc desc;

			desc.type = GFX_SHADER_STAGE_VERTEX;
			desc.size = size;
			desc.data = vs_src;
			desc.entry_point = "main";
			
			m_vs = global::gfx_device().create_shader_from_binary(desc);

			if (!m_vs)
			{
				TE_LOG_ERROR("Failed to load Vertex Shader!");
				return false;
			}
		}

		{
			File* fs = global::filesystem().open_file("frag.spv", TE_FS_READ | TE_FS_BINARY);

			if (!fs)
			{
				TE_LOG_ERROR("Failed to load Fragment Shader!");
				return false;
			}

			size_t size = fs->size();
			void* fs_src = TE_HEAP_ALLOC(size);
			fs->read(fs_src, size, 1);

			global::filesystem().close_file(fs);

			BinaryShaderCreateDesc desc;

			desc.type = GFX_SHADER_STAGE_FRAGMENT;
			desc.size = size;
			desc.data = fs_src;
			desc.entry_point = "main";

			m_fs = global::gfx_device().create_shader_from_binary(desc);

			if (!m_fs)
			{
				TE_LOG_ERROR("Failed to load Fragment Shader!");
				return false;
			}
		}

		return true;
	}

	bool create_pipeline_layout()
	{
		PipelineLayoutCreateDesc desc;

		desc.descriptor_set_count = 0;
		desc.descriptor_sets = nullptr;
		desc.push_constant_range_count = 0;
		desc.push_constant_ranges = nullptr;

		m_pipeline_layout = global::gfx_device().create_pipeline_layout(desc);

		if (!m_pipeline_layout)
		{
			TE_LOG_ERROR("Failed to create Pipeline Layout!");
			return false;
		}

		return true;
	}

	bool create_pipeline_state()
	{
		PipelineStateCreateDesc pso_desc;

		pso_desc.pipeline_layout = m_pipeline_layout;

		pso_desc.type = GFX_PIPELINE_GRAPHICS;
		pso_desc.graphics.shader_count = 2;
		pso_desc.graphics.input_layout = nullptr;

		Shader* shaders[] = { m_vs, m_fs };
		pso_desc.graphics.shaders = shaders;

		BlendStateCreateDesc blend_state;
		blend_state.enable = false;
		blend_state.blend_op = GFX_BLEND_OP_ADD;
		blend_state.blend_op_alpha = GFX_BLEND_OP_ADD;
		blend_state.src_func = GFX_BLEND_FUNC_ONE;
		blend_state.dst_func = GFX_BLEND_FUNC_ONE_MINUS_DST_COLOR;
		blend_state.src_func_alpha = GFX_BLEND_FUNC_ONE;
		blend_state.dst_func_alpha = GFX_BLEND_FUNC_ONE_MINUS_DST_COLOR;

		pso_desc.graphics.blend_state.blend_states = &blend_state;
		pso_desc.graphics.blend_state.enable_logic_op = false;
		pso_desc.graphics.blend_state.logic_op = GFX_LOGIC_OP_COPY;

		pso_desc.graphics.primitive = GFX_PRIMITIVE_TOPOLOGY_TRIANGLES;

		TextureFormat color_fmt = GFX_FORMAT_R8G8B8A8_UNORM;
		LoadOp color_op = GFX_LOAD_OP_CLEAR;
		pso_desc.graphics.render_target_count = 1;
		pso_desc.graphics.color_attachment_formats = &color_fmt;
		pso_desc.graphics.color_load_ops = &color_op;
		pso_desc.graphics.sample_count = GFX_SAMPLE_COUNT_1;

		pso_desc.graphics.depth_stencil_format = GFX_FORMAT_UNKNOWN;
		pso_desc.graphics.depth_stencil_load_op = GFX_LOAD_OP_CLEAR;

		pso_desc.graphics.depth_stencil_state.enable_stencil_test = false;
		pso_desc.graphics.depth_stencil_state.enable_depth_write = false;
		pso_desc.graphics.depth_stencil_state.enable_depth_test = false;
		pso_desc.graphics.depth_stencil_state.depth_cmp_func = GFX_COMP_FUNC_LESS;

		pso_desc.graphics.depth_stencil_state.back_stencil_cmp_func = GFX_COMP_FUNC_LESS;
		pso_desc.graphics.depth_stencil_state.back_stencil_fail = GFX_STENCIL_OP_INCR;
		pso_desc.graphics.depth_stencil_state.back_stencil_mask = 0;
		pso_desc.graphics.depth_stencil_state.back_stencil_pass_depth_fail = GFX_STENCIL_OP_INCR;
		pso_desc.graphics.depth_stencil_state.back_stencil_pass_depth_pass = GFX_STENCIL_OP_INCR;

		pso_desc.graphics.depth_stencil_state.front_stencil_cmp_func = GFX_COMP_FUNC_LESS;
		pso_desc.graphics.depth_stencil_state.front_stencil_fail = GFX_STENCIL_OP_INCR;
		pso_desc.graphics.depth_stencil_state.front_stencil_mask = 0;
		pso_desc.graphics.depth_stencil_state.front_stencil_pass_depth_fail = GFX_STENCIL_OP_INCR;
		pso_desc.graphics.depth_stencil_state.front_stencil_pass_depth_pass = GFX_STENCIL_OP_INCR;

		pso_desc.graphics.rasterizer_state.cull_mode = GFX_CULL_MODE_NONE;
		pso_desc.graphics.rasterizer_state.fill_mode = GFX_FILL_MODE_SOLID;
		pso_desc.graphics.rasterizer_state.front_winding_ccw = true;
		pso_desc.graphics.rasterizer_state.multisample = false;
		pso_desc.graphics.rasterizer_state.scissor = false;

		m_pso = global::gfx_device().create_pipeline_state(pso_desc);
		
		if (!m_pso)
		{
			TE_LOG_ERROR("Failed to create Pipeline State!");
			return false;
		}
	
		return true;
	}

	bool intialize_graphics()
	{
		if (!load_shaders())
			return false;

		if (!create_pipeline_layout())
			return false;

		if (!create_pipeline_state())
			return false;

		for (int i = 0; i < 3; i++)
		{
			m_command_pools[i] = global::gfx_device().create_command_pool(GFX_CMD_POOL_GRAPHICS);
			m_command_buffers[i] = global::gfx_device().create_command_buffer(m_command_pools[i]);
			m_fence[i] = global::gfx_device().create_fence();
		}

		m_image_available_sema = global::gfx_device().create_semaphore();
		m_render_finished_sema = global::gfx_device().create_semaphore();
	}

	void render()
	{
		ClearValue color_clear =
		{
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			1.0f,
			0.0f
		};

		GfxDevice& device = global::gfx_device();

		m_fbo = device.accquire_next_framebuffer(m_image_available_sema);
		Fence* fence = m_fence[m_frame_index];
		CommandPool* cmd_pool = m_command_pools[m_frame_index];
		CommandBuffer* cmd_buffer = m_command_buffers[m_frame_index];
		
		device.wait_for_fences(1, &fence, INFINITY);
		device.reset_command_pool(cmd_pool);

		device.cmd_begin_recording(cmd_buffer);

		device.cmd_bind_framebuffer(cmd_buffer, m_fbo, &color_clear, color_clear);
		device.cmd_set_viewport(cmd_buffer, 0, 0, m_width, m_height);
		device.cmd_bind_pipeline_state(cmd_buffer, m_pso);
		device.cmd_draw(cmd_buffer, 3, 1, 0, 0);

		device.cmd_end_recording(cmd_buffer);

		device.submit_graphics(1, &cmd_buffer, 1, &m_image_available_sema, 1, &m_render_finished_sema, fence);
		device.present(1, &m_render_finished_sema);

		m_frame_index++;

		if (m_frame_index == 3)
			m_frame_index = 0;
	}

	void shutdown_graphics()
	{
		global::gfx_device().wait_for_idle();

		global::gfx_device().destroy_semaphore(m_render_finished_sema);
		global::gfx_device().destroy_semaphore(m_image_available_sema);

		for (int i = 0; i < 3; i++)
		{
			global::gfx_device().destroy_fence(m_fence[i]);
			TE_HEAP_DELETE(m_command_buffers[i]);
			global::gfx_device().destroy_command_pool(m_command_pools[i]);
		}

		global::gfx_device().destory_pipeline_state(m_pso);
		global::gfx_device().destroy_pipeline_layout(m_pipeline_layout);
		global::gfx_device().destroy_shader(m_vs);
		global::gfx_device().destroy_shader(m_fs);
	}
};

TE_END_TERMINUS_NAMESPACE

TE_DECLARE_APP_MAIN(te::Runtime)