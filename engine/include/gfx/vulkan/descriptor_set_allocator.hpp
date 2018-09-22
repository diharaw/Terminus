#pragma once

#include <vulkan/vulkan.h>
#include <stl/vector.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

struct DescriptorSet;

namespace vk
{
	class DescriptorSetAllocator
	{
	public:
		DescriptorSetAllocator();
		~DescriptorSetAllocator();

		// Create the intial Descriptor Pool.
		bool initialize(VkDevice device);

		// Attempt to allocate a Descriptor Set from existing pools, else create new pool.
		DescriptorSet* alloc(VkDescriptorSetLayout layout);

		// Free Descriptor Set.
		void free(DescriptorSet* set);

	private:
		// Create a new Descriptor Pool.
		VkDescriptorPool create_descriptor_pool();

		// Allocate Descriptor Set from the specified pool.
		DescriptorSet* alloc_from_pool(VkDescriptorPool pool, VkDescriptorSetLayout layout);

		Vector<VkDescriptorPool> m_pools;
		VkDevice m_device;

		const uint32_t kDescriptorSetCount = 2048;
		const uint32_t kUniformBufferCount = 2048;
		const uint32_t kStorageBufferCount = 2048;
		const uint32_t kDynamicUniformBufferCount = 64;
		const uint32_t kDynamicStorageBufferCount = 64;
		const uint32_t kSamplerCount = 2048;
		const uint32_t kTextureCount = 2048;
	};
}

TE_END_TERMINUS_NAMESPACE