#pragma once

#include <core/terminus_macros.hpp>
#include <gfx/gfx_descs.hpp>

#include <stl/vector.hpp>
#include <vulkan/vulkan.h>
#include <SDL_vulkan.h>

TE_BEGIN_TERMINUS_NAMESPACE

struct VertexBuffer {};
struct IndexBuffer {};
struct UniformBuffer {};
struct StorageBuffer {};
struct IndirectBuffer {};
struct VertexArray {};
struct PipelineState {};
struct CommandBuffer {};

class GfxDevice
{
public:
	GfxDevice();
	~GfxDevice();

	bool initialize();
	void shutdown();

	// Buffer creation API
	VertexBuffer* create(const VertexBufferDesc& desc);

	// Buffer destruction API
	void destroy(VertexBuffer* buffer);

private:
	bool create_instance();
	bool choose_physical_device();
	bool create_surface();
	bool create_logical_device();
	bool create_queues();
	bool check_validation_layer_support();
	void required_extensions(Vector<const char*>& extensions);

private:
	DeviceProperties m_device_properties;
	DeviceLimits	 m_device_limits;
	VkInstance		 m_instance;
	VkDevice		 m_device;
	VkQueue			 m_graphics_queue;
	VkQueue			 m_compute_queue;
	VkQueue			 m_transfer_queue;
	VkPhysicalDevice m_physical_device;
};

TE_END_TERMINUS_NAMESPACE