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

	int WorkerThreadCount();
	TaskData* CreateTask(int _ThreadIndex = 0);
	ProcessData* CreateProcess(int _ThreadIndex = 0);
	void Execute();
};
