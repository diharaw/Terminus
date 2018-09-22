#include <gfx/vulkan/descriptor_set_allocator.hpp>
#include <gfx/gfx_types.hpp>
#include <core/engine_core.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

namespace vk
{
	// -----------------------------------------------------------------------------------------------------------------------------------

	DescriptorSetAllocator::DescriptorSetAllocator()
	{

	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	DescriptorSetAllocator::~DescriptorSetAllocator()
	{
		for (const auto& pool : m_pools)
		{
			if (pool != VK_NULL_HANDLE)
				vkDestroyDescriptorPool(m_device, pool, nullptr);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	bool DescriptorSetAllocator::initialize(VkDevice device)
	{
		m_device = device;
		VkDescriptorPool pool = create_descriptor_pool();

		// Create one pool initially.
		if (pool != VK_NULL_HANDLE)
		{
			m_pools.push_back(pool);
			return true;
		}

		return false;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	DescriptorSet* DescriptorSetAllocator::alloc(VkDescriptorSetLayout layout)
	{
		
		for (auto& pool : m_pools)
		{
			DescriptorSet* set = alloc_from_pool(pool, layout);

			if (set)
				return set;
		}

		// At this point we know that the existing pools failed to allocate the set.
		VkDescriptorPool pool = create_descriptor_pool();

		if (pool != VK_NULL_HANDLE)
		{
			m_pools.push_back(pool);
			return alloc_from_pool(pool, layout);
		}

		return nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	void DescriptorSetAllocator::free(DescriptorSet* set)
	{
		if (set)
		{
			vkFreeDescriptorSets(m_device, set->vk_pool, 1, &set->vk_ds);
			TE_HEAP_DELETE(set);
		}
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	VkDescriptorPool DescriptorSetAllocator::create_descriptor_pool()
	{
		VkDescriptorPool pool = VK_NULL_HANDLE;
		VkDescriptorPoolSize pool_sizes[6];

		pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		pool_sizes[0].descriptorCount = kUniformBufferCount;

		pool_sizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		pool_sizes[1].descriptorCount = kStorageBufferCount;

		pool_sizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		pool_sizes[2].descriptorCount = kDynamicUniformBufferCount;

		pool_sizes[3].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
		pool_sizes[3].descriptorCount = kDynamicStorageBufferCount;

		pool_sizes[4].type = VK_DESCRIPTOR_TYPE_SAMPLER;
		pool_sizes[4].descriptorCount = kSamplerCount;

		pool_sizes[5].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		pool_sizes[5].descriptorCount = kTextureCount;

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = 6;
		pool_info.pPoolSizes = &pool_sizes[0];
		pool_info.maxSets = kDescriptorSetCount;

		if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &pool) != VK_SUCCESS)
		{
			TE_LOG_ERROR("Failed to create descriptor pool!");
			return VK_NULL_HANDLE;
		}

		return pool;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------

	DescriptorSet* DescriptorSetAllocator::alloc_from_pool(VkDescriptorPool pool, VkDescriptorSetLayout layout)
	{
		VkDescriptorSet descriptor_set = VK_NULL_HANDLE;

		VkDescriptorSetAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = pool;
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = &layout;

		if (vkAllocateDescriptorSets(m_device, &alloc_info, &descriptor_set) == VK_SUCCESS)
		{
			DescriptorSet* set = TE_HEAP_NEW DescriptorSet();
			set->vk_ds = descriptor_set;
			set->vk_pool = pool;

			return set;
		}
		
		return nullptr;
	}

	// -----------------------------------------------------------------------------------------------------------------------------------
}

TE_END_TERMINUS_NAMESPACE