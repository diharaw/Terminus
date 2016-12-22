#pragma once

#include "TaskQueue.h"

#define TASK_METHOD_DECLARATION(x) void x(void* data)
#define TASK_METHOD_DEFINITION(x, y) void x::y(void* data)

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
