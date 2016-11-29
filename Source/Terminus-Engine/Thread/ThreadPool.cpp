#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	m_num_threads = std::thread::hardware_concurrency();
}

ThreadPool::~ThreadPool()
{
	for (int i = 0; i < m_task_queue_list.size(); i++)
	{
		delete m_task_queue_list[i];
	}
}

void ThreadPool::Initialize(int threadCount)
{
	if (threadCount == 0)
		m_num_worker_threads = m_num_threads;
	else
	{
		assert(threadCount <= m_num_threads);
		m_num_worker_threads = threadCount;
	}

	for (int i = 0; i < m_num_worker_threads; i++)
	{
		m_task_queue_list.push_back(new TaskQueue());
	}
}

int ThreadPool::WorkerThreadCount()
{
	return m_num_worker_threads;
}

TaskData* ThreadPool::CreateTask(int threadIndex)
{
	return m_task_queue_list[threadIndex]->CreateTask();
}

void ThreadPool::Submit()
{
	for (int i = 0; i < m_num_worker_threads; i++)
	{
		m_task_queue_list[i]->Execute();
	}
}

void ThreadPool::Wait()
{
	for (int i = 0; i < m_num_worker_threads; i++)
	{
		m_task_queue_list[i]->Wait();
	}
}

void ThreadPool::SubmitAndWait()
{
	Submit();
	Wait();
}
