/*		TaskManager 0.5
			by quckly
*/
#include <extdll.h>
#include <meta_api.h>
#include "tasks.h"

TaskManager Tasks;

TaskManager::Task::Task(TaskCallback function, const double time, const int taskid, const int flags, edict_t* data)
{
	m_taskid = taskid;
	m_function = function;
	m_time = time;
	m_gametime = gpGlobals->time + m_time;
	m_flags = flags;
	m_edict = data;
	
	m_remove = false;
}

void TaskManager::Task::Remove()
{
	m_remove = true;
}

bool TaskManager::Task::getRemove() const
{
	return m_remove;
}

TaskResult TaskManager::Task::Think()
{
	if (gpGlobals->time >= m_gametime)
	{
		m_function(m_edict);
		
		if (m_flags & TF_INF)
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

void TaskManager::Add(TaskCallback function, const double time, const int taskid, const int flags)
{
	Add(function, time, taskid, flags, nullptr);
}

void TaskManager::Add(TaskCallback function, const double time, const int taskid, const int flags, edict_t* data)
{
	if (function == nullptr)
	{
		//UTIL_LogPrintf("[TASKS] Invalid function\n");

		return;
	}

	if (time < 0.0)
	{
		//UTIL_LogPrintf("[TASKS] Invalid time (%f)\n", time);

		return;
	}

	Task *TempTask = new Task(function, time, taskid, flags, data);

	tasklist.push_back(TempTask);

	return;
}

bool TaskManager::Exists(const int task) const
{
	for (std::list<Task *>::const_iterator iter = tasklist.begin(); iter != tasklist.end(); iter++)
	{
		if ((*iter)->GetTaskID() == task)
			return true;
	}

	return false;
}

void TaskManager::Delete(const int task)
{
    for (std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end();)
    {
            if ((*iter)->GetTaskID() == task)
            {
				if (m_think)
				{
					(*iter)->Remove();
					++iter;
				}
				else
				{
                    delete *iter;
                    iter = tasklist.erase(iter);
				}
            }
            else
            {
                    ++iter;
            }
    }
}

void TaskManager::Think()
{
	m_think = true;

    for (std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end();)
    {
		if ((*iter)->getRemove())
		{
			delete *iter;
            iter = tasklist.erase(iter);

			break;
		}

        if ((*iter)->Think() == TR_Delete)
        {
            delete *iter;
            iter = tasklist.erase(iter);
        }
        else
        {
                ++iter;
        }
    }

	for (std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end();)
    {
		if ((*iter)->getRemove())
		{
			delete *iter;
            iter = tasklist.erase(iter);
		}
		else
        {
                ++iter;
        }
	}

	m_think = false;
}