/*		TaskManager v0.3	19.01.2013
			by quckly

	How to:
		1. Create global object TaskManager Tasks;
		2. Use:
			Tasks.Add((void *) TASK_Handler, 5.0, TaskID, TF_INF|TF_NONE, (void *) pointer to data, sizeof(data))
		3. Create handler:
			void TASK_Handler(void* data, size_t data_size) { }
		4. Add to void StartFrame()
			Tasks.Think();
	Functions:
		In definition of TaskManager
*/

#pragma once

#include "sdk/amxxmodule.h"
#include <list>

enum TaskFlags
{
	TF_NONE = 0,		// Once exec
	TF_INF				// Infinity loop task
};

enum TaskResult
{
	TR_Delete = 0,
	TR_Save
};

class TaskManager
{
private:
	class Task
	{
	public:
		Task(void *function, const double time, const int taskid, const int flags, void *data, const size_t data_size);
		~Task();
		TaskResult Think();
		int GetTaskID() const;
	private:
		int m_taskid;
		void *m_function;
		double m_time;
		double m_gametime;
		int m_flags;
		void* m_data;
		size_t m_data_size;
	};

public:
	// Create new task
	// Return task id. Return -1 if failure.
	// Function must have this type: void (*)(void*, size_t)
	void Add(void *function /* void (*)(void*, size_t) */, const double time = 0.0, const int taskid = 0, const int flags = TF_NONE, const void *data = NULL, const size_t data_size = 0);

	// Check task of exist
	// return 1 or 0
	BOOL Exist(const int taskid) const;
	
	// Delete task on task id
	void Delete(const int taskid);

	// Execution this function in void StartFrame()
	void Think();

	TaskManager();

private:
	std::list<Task *> tasklist;

	//int nexttaskid;

	int framecount;
};