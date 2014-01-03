/*		TaskManager v0.4
			by quckly

	How to:
		1. Create global object TaskManager Tasks;
		2. Use:
			Tasks.Add(TASK_Handler, 5.0, TaskID, TF_INF|TF_NONE, (void *) pointer to data, sizeof(data))
		3. Create handler:
			void TASK_Handler(void* data, size_t data_size, int taskid) { }
		4. Add to void StartFrame()
			Tasks.Think();
	Functions:
		In definition of TaskManager
*/

#pragma once

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
		Task(void *const function, const double time, const int taskid, const int flags);
		Task(void *const function, const double time, const int taskid, const int flags, edict_t *const data);
		TaskResult Think();
		int GetTaskID() const;

		void Remove();
		bool getRemove() const;
	private:
		int m_taskid;
		void *m_function;
		double m_time;
		double m_gametime;
		int m_flags;
		edict_t* m_edict;

		bool m_remove;
	};

public:
	// Create new task
	// Return task id. Return -1 if failure.
	// Function must have this type: void (*)(void*, size_t)
	void Add(void *const function /* void (*)(void*, size_t) */, const double time = 0.0, const int taskid = 0, const int flags = TF_NONE);
	void Add(void *const function /* void (*)(void*, size_t) */, const double time, const int taskid, const int flags, edict_t *const data);

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
	bool m_think;

	//int nexttaskid;

	int framecount;
};

extern TaskManager Tasks;