#pragma once

#include "TaskQueue.h"

class TaskScheduler
{
private:
	int			  m_NumThreads;
	int			  m_NumWorkerThreads;
	TaskQueueList m_TaskQueueList;

public:
	TaskScheduler();
	~TaskScheduler();

	int worker_thread_count();
	TaskData* create_task(int _ThreadIndex = 0);
	ProcessData* create_process(int _ThreadIndex = 0);
	void execute();
};
