#include "TaskQueue.h"

TaskData* TaskQueue::CreateTask()
{
    std::lock_guard<std::mutex> lock(m_mutex);
	m_last_index++;
	return &m_task_list[m_last_index];
}

TaskQueue::TaskQueue()
{
	m_last_index = -1;
	m_is_destroy_requested = false;
	m_worker_thread = Thread(&TaskQueue::ProcessQueue, this);
	m_is_long_running = false;
}

TaskQueue::~TaskQueue()
{
	m_mutex.lock();
	m_is_destroy_requested = true;
	m_condition_variable.notify_one();
	m_mutex.unlock();
	m_worker_thread.join();
}

void TaskQueue::Execute()
{
	m_condition_variable.notify_one();
}

void TaskQueue::Wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_condition_variable.wait(lock, [this] { return (m_last_index == -1); });
}

void TaskQueue::ProcessQueue()
{
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_condition_variable.wait(lock, [this] { return !(m_last_index == -1) || m_is_destroy_requested; });

			if (m_is_destroy_requested)
			{
				break;
			}
		}

		TaskData* current = &m_task_list[m_last_index];
		current->function.Invoke(current->data);

		{
			std::lock_guard<std::mutex> lock(m_mutex);
            m_last_index--;
			m_condition_variable.notify_one();
		}
	}
}
