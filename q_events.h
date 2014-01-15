#pragma once

#include <vector>
#include <string>

#include <extdll.h>
#include <meta_api.h>

enum
{
	ER_IGNORED = 0,
	ER_SUPERCEDE
};

typedef int (*EventCallback)(int, int, const float*, edict_t*);

class EventManager
{
public:
	// Hook event
	void RegisterEvent(const int msg_id, EventCallback func);
	void RegisterEvent(const char* msg_id, EventCallback func);

	// Get args
	int GetArgInt(int num);
	float GetArgFloat(int num);
	const char* GetArgString(int num);

	// Set Args
	void SetArg(int num, int set);
	void SetArg(int num, float set);
	void SetArg(int num, const char* set);

	// Get ID of name of event
	int ID(const char* msg_name);

private:
	enum EM_ArgType{
		at_byte = 0,
		at_char,
		at_short,
		at_long,
		at_angle,
		at_coord,
		at_string,
		at_entity
	};

	class EventArg
	{
	public:
		EventArg(EM_ArgType _type, int val) : m_type(_type) { value.iValue = val; };
		EventArg(EM_ArgType _type, float val) : m_type(_type) { value.flValue = val; };
		EventArg(EM_ArgType _type, const char* val) : m_type(_type) { str = val; };

		union	u_Value
		{
			int iValue;
			float flValue;
		}	value;
		std::string str;
		
		EM_ArgType m_type;
	};

	class EventRegister
	{
	public:
		EventRegister(int _msg_id, EventCallback _func) : msg_id(_msg_id), func(_func) {};

		EventCallback func;
		int msg_id;
	};

public:
	EventManager();

	// Handlers
	void EM_MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
	void EM_MessageEnd();
	void EM_WriteByte(int iValue);
	void EM_WriteChar(int iValue);
	void EM_WriteShort(int iValue);
	void EM_WriteLong(int iValue);
	void EM_WriteAngle(float flValue);
	void EM_WriteCoord(float flValue);
	void EM_WriteString(const char *sz);
	void EM_WriteEntity(int iValue);
	
private:
	std::vector<EventArg> m_args;
	int m_msg_dest, m_msg_type;
	const float *m_origin;
	edict_t *m_ed;

	bool m_hook;

	// Callbacks
	std::vector<EventRegister> m_regevents;
};

extern EventManager Events;