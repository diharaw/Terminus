#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>
#include <iostream>
#include "Delegate11.h"

#define MAX_TASKS 1024

class TaskQueue;

typedef Terminus::Delegate<void(void*)> WorkerFunction;
typedef std::vector<TaskQueue*> TaskQueueList;
typedef std::thread Thread;
typedef std::mutex Mutex;
typedef std::condition_variable ConditionVariable;

struct TaskData
{
	WorkerFunction function;
	void*		   data;
};

class TaskQueue
{
protected:
	int		 m_last_index;
	TaskData m_task_list[MAX_TASKS];
private:
	Thread			  m_worker_thread;
	Mutex			  m_mutex;
	ConditionVariable m_condition_variable;
	bool			  m_is_destroy_requested;
	bool              m_is_long_running;

public:
	TaskQueue();
	~TaskQueue();
	void Execute();
	void Wait();
	TaskData* CreateTask();

private:
	void ProcessQueue();
};
