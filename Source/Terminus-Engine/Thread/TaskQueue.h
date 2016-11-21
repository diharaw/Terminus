#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>
#include <iostream>

#define MAX_TASKS 1024

class ITaskQueue;
class TaskQueue;

typedef void(*WorkerFunction) (void*);
typedef std::vector<ITaskQueue*> TaskQueueList;
typedef std::thread Thread;
typedef std::mutex Mutex;
typedef std::condition_variable ConditionVariable;

struct TaskData
{
	WorkerFunction function;
	void*		   data;
};

struct ProcessData
{

};

class ITaskQueue
{
protected:
	int		 m_LastIndex;
	TaskData m_TaskList[MAX_TASKS];

public:
	ITaskQueue();
	virtual ~ITaskQueue();
	virtual void Execute() = 0;
	virtual void Wait() = 0;
	TaskData* CreateTask();

private:
	virtual void ProcessQueue() = 0;
};

class TaskQueue : public ITaskQueue
{
private:
	Thread			  m_WorkerThread;
	Mutex			  m_Mutex;
	ConditionVariable m_ConditionVariable;
	bool			  m_IsDestroyRequested;

public:
	TaskQueue();
	virtual ~TaskQueue();
	virtual void Execute();
	virtual void Wait();
	
private:
	virtual void ProcessQueue();
};
