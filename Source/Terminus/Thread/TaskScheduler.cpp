#include "TaskScheduler.h"

TaskScheduler::TaskScheduler()
{
	m_NumThreads = std::thread::hardware_concurrency();
	m_NumWorkerThreads = m_NumThreads;

	for (int i = 0; i < m_NumWorkerThreads; i++)
	{
		m_TaskQueueList.push_back(new TaskQueue());
	}
}

TaskScheduler::~TaskScheduler()
{
	for (int i = 0; i < m_TaskQueueList.size(); i++)
	{
		delete m_TaskQueueList[i];
	}
}

int TaskScheduler::WorkerThreadCount()
{
	return m_NumWorkerThreads;
}

TaskData* TaskScheduler::CreateTask(int _ThreadIndex)
{
	return m_TaskQueueList[_ThreadIndex]->CreateTask();
}

ProcessData* TaskScheduler::CreateProcess(int _ThreadIndex)
{
	return nullptr;
}

void TaskScheduler::Execute()
{
	for (int i = 0; i < m_NumWorkerThreads; i++)
	{
		m_TaskQueueList[i]->Execute();
	}

	for (int i = 0; i < m_NumWorkerThreads; i++)
	{
		m_TaskQueueList[i]->Wait();
	}
}