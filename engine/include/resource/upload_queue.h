#pragma once

#include <core/macro.h>
#include <mutex>

TERMINUS_BEGIN_NAMESPACE

template<typename T, size_t N>
class UploadQueue
{
private:
	std::mutex m_critical_section;
	T		   m_queue[N];
	uint32_t   m_front;
	uint32_t   m_back;
	uint32_t   m_num_items;

public:
	UploadQueue()
	{
		m_num_items = 0;
		m_front = 0;
		m_back = 0;
	}

	~UploadQueue()
	{

	}

	T* allocate()
	{
		uint32_t item_index = m_num_items++;
		return &m_queue[item_index & (N - 1u)];
	}

	void push(T* item)
	{
		std::lock_guard<std::mutex> lock(m_critical_section);

		m_queue[m_back & (N - 1u)] = item;
		++m_back;
	}

	T* pop()
	{
		std::lock_guard<std::mutex> lock(m_critical_section);

		const uint32_t item_count = m_back - m_front;
		if (item_count <= 0)
			return nullptr;

		--m_back;
		return m_queue[m_back & (N - 1u)];
	}

	bool empty()
	{
		return (m_front == 0 && m_back == 0);
	}
};

TERMINUS_END_NAMESPACE
