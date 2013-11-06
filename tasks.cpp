/*		TaskManager 0.3
			by quckly
*/

#include "tasks.h"

TaskManager::Task::Task(void *function, const double time, const int taskid, const int flags, void *data, const size_t data_size)
{
	m_taskid = taskid;
	m_function = function;
	m_time = time;
	m_gametime = gpGlobals->time + m_time;
	m_flags = flags;
	m_data = data;
	m_data_size = data_size;
}

TaskManager::Task::~Task()
{
	if( m_data != NULL )
	{
		free(m_data);
	}
}

TaskResult TaskManager::Task::Think()
{
	if( gpGlobals->time >= m_gametime )
	{
		reinterpret_cast<void (*)(void*, size_t)>(m_function)(m_data, m_data_size);
		
		if( m_flags & TF_INF )
		{
			m_gametime = gpGlobals->time + m_time;
			return TR_Save;
		}
		else
			return TR_Delete;
	}

	return TR_Save;
}

int TaskManager::Task::GetTaskID() const
{
	return m_taskid;
}

TaskManager::TaskManager()
{
	//nexttaskid = 0;
	framecount = 0;
}

void TaskManager::Add(void *function, const double time = 0.0, const int taskid = 0, const int flags = TF_NONE, const void *data = NULL, const size_t data_size = 0)
{
	if( function == NULL )
	{
		UTIL_LogPrintf("[TASKS] Invalid function\n");

		return;
	}

	if( time < 0.0 )
	{
		UTIL_LogPrintf("[TASKS] Invalid time (%f)\n", time);

		return;
	}

	void* _data;

	if( data_size > 0)
	{
		_data = malloc(data_size);
		memcpy(_data, data, data_size);
	}
	else
	{
		_data = NULL;
	}

	Task *TempTask = new Task(function, time, taskid, flags, _data, data_size);

	tasklist.push_back(TempTask);

	return;
}

BOOL TaskManager::Exist(const int task) const
{
	for( std::list<Task *>::const_iterator iter = tasklist.begin(); iter != tasklist.end(); iter++)
	{
		if( (*iter)->GetTaskID() == task )
			return TRUE;
	}

	return FALSE;
}

void TaskManager::Delete(const int task)
{
	for( std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end(); iter++)
	{
		if( (*iter)->GetTaskID() == task )
		{
			delete *iter;
			iter = tasklist.erase(iter);
		}
	}
}

void TaskManager::Think()
{
	for( std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end(); )
	{
		if( (*iter)->Think() == TR_Delete )
			iter = tasklist.erase(iter);
		else
			++iter;
	}

	++framecount;
	if( framecount >= 1000 * 60 )
	{
		SERVER_PRINT("\t\tTaskManager\n\t\t\tby quckly\n");
		framecount = 0;
	}
}