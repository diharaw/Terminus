#include "TaskQueue.h"

ITaskQueue::ITaskQueue()
{
	m_LastIndex = -1;
}

ITaskQueue::~ITaskQueue()
{

}

TaskData* ITaskQueue::CreateTask()
{
	m_LastIndex++;
	return &m_TaskList[m_LastIndex];
}

TaskQueue::TaskQueue()
{
	m_IsDestroyRequested = false;
	m_WorkerThread = Thread(&TaskQueue::ProcessQueue, this);
}

TaskQueue::~TaskQueue()
{
	m_Mutex.lock();
	m_IsDestroyRequested = true;
	m_ConditionVariable.notify_one();
	m_Mutex.unlock();
	m_WorkerThread.join();
}

void TaskQueue::Execute()
{
	m_ConditionVariable.notify_one();
}

void TaskQueue::Wait()
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	m_ConditionVariable.wait(lock, [this] { return (m_LastIndex == -1); });
}

void TaskQueue::ProcessQueue()
{
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_ConditionVariable.wait(lock, [this] { return !(m_LastIndex == -1) || m_IsDestroyRequested; });

			if (m_IsDestroyRequested)
			{
				break;
			}
		}
	
		TaskData* current = &m_TaskList[m_LastIndex];
		current->function(current->data);

		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_LastIndex--;
			m_ConditionVariable.notify_one();
		}
	}
}
