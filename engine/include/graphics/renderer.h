#pragma once

#include <core/macro.h>
#include <container/concurrent_queue.h>
#include <thread/thread_pool.h>

// Uniform Slots

#define PER_FRAME_UNIFORM_SLOT 0
#define PER_DRAW_UNIFORM_SLOT 1
#define PER_DRAW_MATERIAL_UNIFORM_SLOT 2
#define PER_DRAW_BONE_OFFSETS_UNIFORM_SLOT 3

TERMINUS_BEGIN_NAMESPACE

using GraphicsUploadQueue = ConcurrentQueue<Task, 100>;

// Forward Declarations

class Scene;
class ShaderCache;
struct FramePacket;
struct CommandQueue;
struct UniformBuffer;

// Uniform Buffer Structs
struct TE_ALIGNED(16) PerFrameUniforms
{
    Matrix4 last_view_projection;
    Matrix4 view_projection;
    Matrix4 inverse_view_projection;
    Matrix4 projection;
    Matrix4 view;
    Matrix4 view_sky;
    Vector3 view_position;
    Vector3 view_direction;
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

struct RenderPrepareTaskData
{
    uint32_t _scene_index;
    Scene*	 _scene;
};

class Renderer
{
private:
    DefaultThreadPool*  _thread_pool;
	GraphicsUploadQueue _graphics_upload_queue;
	std::thread         _thread;
	FramePacket*		_pkt;
	CommandQueue*		_graphics_queue;
	bool				_running;
    
public:
    UniformBuffer* _per_frame_buffer;
    UniformBuffer* _per_frame_sky_buffer;
    UniformBuffer* _per_draw_buffer;
    UniformBuffer* _per_draw_material_buffer;
    UniformBuffer* _per_draw_bone_offsets_buffer;
    
public:
    Renderer();
    ~Renderer();
    void initialize(FramePacket* pkts);
    void shutdown(FramePacket* pkts);
    void submit(FramePacket* pkt);
	void render(FramePacket* pkt);
	void enqueue_upload_task(Task& task);

    
private:
	void initialize_internal();
	void shutdown_internal();
	void render_loop();
    void generate_commands_view(void* data);
};

extern void submit_gpu_upload_task(Task& task);

TERMINUS_END_NAMESPACE
