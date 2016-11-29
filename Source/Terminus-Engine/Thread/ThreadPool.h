#pragma once

#include "TaskQueue.h"

class ThreadPool
{
private:
	int			  m_num_threads;
	int			  m_num_worker_threads;
	TaskQueueList m_task_queue_list;

public:
	ThreadPool();
	~ThreadPool();
	void Initialize(int threadCount = 0);

	int WorkerThreadCount();
	TaskData* CreateTask(int threadIndex = 0);
	void Submit();
	void Wait();
	void SubmitAndWait();
};