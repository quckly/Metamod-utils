/*		TaskManager 0.4
			by quckly
*/
#include <extdll.h>
#include <meta_api.h>
#include "CTasks.h"

TaskManager Tasks;

TaskManager::Task::Task(void *const function, const double time, const int taskid, const int flags)
{
	m_edict = nullptr;

	m_taskid = taskid;
	m_function = function;
	m_time = time;
	m_gametime = gpGlobals->time + m_time;
	m_flags = flags;

	m_remove = false;
}

TaskManager::Task::Task(void *const function, const double time, const int taskid, const int flags, edict_t *const data)
{
	m_edict = data;

	m_taskid = taskid;
	m_function = function;
	m_time = time;
	m_gametime = gpGlobals->time + m_time;
	m_flags = flags;

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
	if( gpGlobals->time >= m_gametime )
	{
		reinterpret_cast< void (*)(void*, size_t)>(m_function)(m_edict, 0);
		
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
// govnokod
void TaskManager::Add(void *const function, const double time, const int taskid, const int flags)
{
	if( function == NULL )
	{
		//UTIL_LogPrintf("[TASKS] Invalid function\n");

		return;
	}

	if( time < 0.0 )
	{
		//UTIL_LogPrintf("[TASKS] Invalid time (%f)\n", time);

		return;
	}

	Task *TempTask = new Task(function, time, taskid, flags);

	tasklist.push_back(TempTask);

	return;
}

void TaskManager::Add(void *const function, const double time, const int taskid, const int flags, edict_t *const data)
{
	if( function == NULL )
	{
		//UTIL_LogPrintf("[TASKS] Invalid function\n");

		return;
	}

	if( time < 0.0 )
	{
		//UTIL_LogPrintf("[TASKS] Invalid time (%f)\n", time);

		return;
	}

	Task *TempTask = new Task(function, time, taskid, flags, data);

	tasklist.push_back(TempTask);

	return;
}
// end
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
    for (std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end(); )
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

    for (std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end(); )
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

	for (std::list<Task *>::iterator iter = tasklist.begin(); iter != tasklist.end(); )
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
 
    ++framecount;
    if( framecount >= 1000 * 60 )
    {
            printf("\t\tTaskManager\n\t\t\tby quckly\n");
            framecount = 0;
    }

	m_think = false;
}