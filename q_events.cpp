/*
*		Event Manager by quckly
*				Version: 1.0
*
*/

#include "q_events.h"

EventManager Events;

EventManager::EventManager()
{
	m_hook = false;
	//m_args = std::vector<EventArg>(16);
}

void EventManager::RegisterEvent(const int _msg_id, EventCallback _func)
{
	// Check for exist
	for (auto iter = m_regevents.cbegin(); iter != m_regevents.cend(); iter++)
		if (iter->msg_id == _msg_id && iter->func == _func)
			return;

	m_regevents.push_back(EventRegister(_msg_id, _func));
}

void EventManager::RegisterEvent(const char* msg_name, EventCallback func)
{
	RegisterEvent(ID(msg_name), func);
}

int EventManager::ID(const char* msg_name)
{
	return GET_USER_MSG_ID(PLID, msg_name, NULL);
}

// Get args
int EventManager::GetArgInt(int num)
{
	if (num < 0 || num >= m_args.size())
		return 0;

	return m_args[num].value.iValue;
}

float EventManager::GetArgFloat(int num)
{
	if (num < 0 || num >= m_args.size())
		return 0.0;

	return m_args[num].value.flValue;
}

const char* EventManager::GetArgString(int num)
{
	if (num < 0 || num >= m_args.size() || m_args[num].m_type != at_string)
		return nullptr;

	return m_args[num].str.c_str();
}

// Set Args
void EventManager::SetArg(int num, int set)
{
	if (num < 0 || num >= m_args.size())
		return;

	m_args[num].value.iValue = set;
}

void EventManager::SetArg(int num, float set)
{
	if (num < 0 || num >= m_args.size())
		return;

	m_args[num].value.flValue = set;
}

void EventManager::SetArg(int num, const char* set)
{
	if (num < 0 || num >= m_args.size() || m_args[num].m_type != at_string)
		return;

	m_args[num].str = set;
}

// Handlers
void EventManager::EM_MessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	m_hook = false;
	
	for (auto iter = m_regevents.cbegin(); iter != m_regevents.cend(); iter++)
	{
		if (msg_type == iter->msg_id)
		{
			m_hook = true;

			m_msg_dest = msg_dest;
			m_msg_type = msg_type;
			m_origin = pOrigin;
			m_ed = ed;

			RETURN_META(MRES_SUPERCEDE);
		}
	}

	RETURN_META(MRES_IGNORED);
}

void EventManager::EM_MessageEnd()
{
	if (m_hook)
	{
		int cb_return = ER_IGNORED, _cbret;

		// Execute all registered events
		for (auto iter = m_regevents.cbegin(); iter != m_regevents.cend(); iter++)
		{
			if (m_msg_type == iter->msg_id)
			{
				_cbret = iter->func(m_msg_dest, m_msg_type, m_origin, m_ed);
				
				if (_cbret == ER_SUPERCEDE)
					cb_return = ER_SUPERCEDE;
			}
		}

		if (cb_return == ER_SUPERCEDE)
		{
			RETURN_META(MRES_SUPERCEDE);
		}

		// Send message
		MESSAGE_BEGIN(m_msg_dest, m_msg_type, m_origin, m_ed);

		for (auto iter = m_args.cbegin(); iter != m_args.cend(); iter++)
		{
			switch (iter->m_type)
			{
			case at_byte:
				WRITE_BYTE(iter->value.iValue);
				break;
			case at_char:
				WRITE_CHAR(iter->value.iValue);
				break;
			case at_short:
				WRITE_SHORT(iter->value.iValue);
				break;
			case at_long:
				WRITE_LONG(iter->value.iValue);
				break;
			case at_angle:
				WRITE_ANGLE(iter->value.flValue);
				break;
			case at_coord:
				WRITE_COORD(iter->value.flValue);
				break;
			case at_string:
				WRITE_STRING(iter->str.c_str());
				break;
			case at_entity:
				WRITE_ENTITY(iter->value.iValue);
				break;
			}
		}

		MESSAGE_END();

		// Clear
		m_args.clear();

		m_hook = false;

		RETURN_META(MRES_SUPERCEDE);
	}

	RETURN_META(MRES_IGNORED);
}

void EventManager::EM_WriteByte(int iValue)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_byte, iValue));

	RETURN_META(MRES_SUPERCEDE);
}

void EventManager::EM_WriteChar(int iValue)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_char, iValue));

	RETURN_META(MRES_SUPERCEDE);
}

void EventManager::EM_WriteShort(int iValue)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_short, iValue));

	RETURN_META(MRES_SUPERCEDE);
}

void EventManager::EM_WriteLong(int iValue)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_long, iValue));

	RETURN_META(MRES_SUPERCEDE);
}

void EventManager::EM_WriteAngle(float flValue)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_angle, flValue));

	RETURN_META(MRES_SUPERCEDE);
}

void EventManager::EM_WriteCoord(float flValue)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_coord, flValue));

	RETURN_META(MRES_SUPERCEDE);
}

void EventManager::EM_WriteString(const char *sz)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_string, sz));

	RETURN_META(MRES_SUPERCEDE);
}

void EventManager::EM_WriteEntity(int iValue)
{
	if (!m_hook)
		RETURN_META(MRES_IGNORED);

	m_args.push_back(EventArg(at_entity, iValue));

	RETURN_META(MRES_SUPERCEDE);
}