#pragma once

#include <core/macro.h>
#include <resource/upload_queue.h>
#include <thread/thread_pool.h>
#include <graphics/renderer/scene_renderer.h>
#include <graphics/renderer/post_process_renderer.h>

// Uniform Slots

#define PER_FRAME_UNIFORM_SLOT 0
#define PER_DRAW_UNIFORM_SLOT 1
#define PER_DRAW_MATERIAL_UNIFORM_SLOT 2
#define PER_DRAW_BONE_OFFSETS_UNIFORM_SLOT 3

TERMINUS_BEGIN_NAMESPACE

// Forward Declarations

class Scene;
class ShaderCache;
struct FramePacket;
struct CommandQueue;
struct UniformBuffer;

class Renderer
{
private:
    DefaultThreadPool*  m_thread_pool;
	UploadQueue<Task, 100> m_graphics_upload_queue;
	std::thread         m_thread;
	FramePacket*		m_pkt;
	CommandQueue*		m_graphics_queue;
	bool				m_running;
	SceneRenderer		m_scene_renderer;
	PostProcessRenderer m_post_process_renderer;
    UniformBuffer*		m_per_frame_buffer;
    UniformBuffer*		m_per_frame_sky_buffer;
    UniformBuffer*		m_per_draw_buffer;
    UniformBuffer*		m_per_draw_material_buffer;
    UniformBuffer*		m_per_draw_bone_offsets_buffer;
    
public:
    Renderer();
    ~Renderer();
    void initialize(FramePacket* pkts);
    void shutdown(FramePacket* pkts);
    void submit(FramePacket* pkt);
	void render(FramePacket* pkt);
	Task* create_upload_task();
	void enqueue_upload_task(Task* task);
    
private:
	void initialize_internal();
	void shutdown_internal();
	void render_loop();
    void generate_view_commands(void* data);
};

TERMINUS_END_NAMESPACE
