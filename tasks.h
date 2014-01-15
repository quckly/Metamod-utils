/*		TaskManager v0.5
			by quckly	(quckly.ru)
			
	Require c++11

	How to:
		1. Create global object TaskManager Tasks;
			P.S.: Already created in tasks.cpp
		2. Use:
			Tasks.Add(TASK_Handler, 5.0, TaskID, TF_INF|TF_NONE)
		3. Create handler:
			void TASK_Handler(edict_t* ent) { }
		4. Add to void StartFrame()
			Tasks.Think();
	Functions:
		In definition of TaskManager
*/
#pragma once

#include <list>

class TaskManager;
extern TaskManager Tasks;

enum TaskFlags
{
	TF_NONE = 0,		// Once exec
	TF_INF	= 1			// Infinity loop task
};

enum TaskResult
{
	TR_Delete = 0,
	TR_Save
};

typedef void (*TaskCallback)(edict_t* ent);

class TaskManager
{
private:
	class Task
	{
	public:
		Task(TaskCallback function, const double time, const int taskid, const int flags, edict_t* data);
		TaskResult Think();
		int GetTaskID() const;

		void Remove();
		bool getRemove() const;
	private:
		int m_taskid;
		TaskCallback m_function;
		double m_time;
		double m_gametime;
		int m_flags;
		edict_t* m_edict;

		bool m_remove;
	};

public:
	// Create new task
	void Add(TaskCallback function, const double time = 0.0, const int taskid = 0, const int flags = TF_NONE);
	void Add(TaskCallback function, const double time, const int taskid, const int flags, edict_t* data);
	
	bool Exists(const int taskid) const;
	
	// Delete a task by task ID
	void Delete(const int taskid);

	// Execution this function in |void StartFrame()|
	void Think();

private:
	std::list<Task*> tasklist;
	bool m_think;
};
