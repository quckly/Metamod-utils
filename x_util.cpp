/* 
	x_util.cpp

	Utils 0.2 beta by quckly

	Required:
		amxmodx
*/

#include "x_util.h"

// Global vars
int MSGID_ShowMenu = 0;
int MSGID_HLTV = 0;
int MSGID_TextMsg = 0;
int MSGID_SayText = 0;

int random_num(int start, int end)
{
	return RANDOM_LONG(start, end);
}

/*static unsigned long random_x=123456789, random_y=362436069, random_z=521288629;
unsigned long xorshf96(void)
{	//period 2^96-1
	unsigned long random_t;
	random_x ^= random_x << 16;
	random_x ^= random_x >> 5;
	random_x ^= random_x << 1;

	random_t = random_x;
	random_x = random_y;
	random_y = random_z;
	random_z = random_t ^ random_x ^ random_y;

	return random_z;
}

int random_num(int start, int end)
{
	return (( xorshf96() % (end-start+1)) + start);
}*/

void ShowMenu(edict_t* pEdict, int keys, int time, char *text)
{
	static char textmenu[8193]; // 8 kb.
	int len = snprintf(textmenu, sizeof(textmenu)-1, "%s", text);

	UTIL_ShowMenu(pEdict, keys, time, textmenu, len);
}

/* warning - don't pass here const string */
void UTIL_ShowMenu(edict_t* pEdict, int slots, int time, char *menu, int mlen)
{
	char *n = menu;
	char c = 0;
	int a;

	if(!MSGID_ShowMenu)
	{
		MSGID_ShowMenu = GET_USER_MSG_ID(PLID, "ShowMenu", NULL);

		if(!MSGID_ShowMenu)
			return;
	}

	while (*n)
	{
		a = mlen;
		if (a > 175) a = 175;
		mlen -= a;
		c = *(n+=a);
		*n = 0;
		
		MESSAGE_BEGIN(MSG_ONE, MSGID_ShowMenu, NULL, pEdict);
		WRITE_SHORT(slots);
		WRITE_CHAR(time);
		WRITE_BYTE(c ? TRUE : FALSE);
		WRITE_STRING(menu);
		MESSAGE_END();
		*n = c;
		menu = n;
	}
}

void UTIL_ClientPrint(edict_t *pEntity, CLIENT_PRINT_TYPE msg_dest, const char *pMessage, ...)
{
	if(!MSGID_TextMsg)
	{
		MSGID_TextMsg = GET_USER_MSG_ID(PLID, "TextMsg", NULL);

		if(!MSGID_TextMsg)
			return;
	}

	va_list argptr;
    static char buffer[191];
     
    va_start ( argptr, pMessage);
    vsnprintf( buffer, sizeof(buffer), pMessage, argptr );
    va_end   ( argptr );

	buffer[190] = 0;

	if(pEntity)
	{
		MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, MSGID_TextMsg, NULL, pEntity);
			WRITE_BYTE(msg_dest);
			WRITE_STRING(buffer);
		MESSAGE_END();
	}
	else
	{
		for(int id = 1; id <= gpGlobals->maxClients; id++)
		{
			if( MF_IsPlayerIngame(id) ) // amxx
			{
				MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, MSGID_TextMsg, NULL, INDEXENT(id));
					WRITE_BYTE(msg_dest);
					WRITE_STRING(buffer);
				MESSAGE_END();
			}
		}
	}
}

void UTIL_ColorChat(edict_t *pEntity, const char *pMessage, ...)
{ 
    if(!MSGID_SayText)
	{
		MSGID_SayText = GET_USER_MSG_ID(PLID, "SayText", NULL);

		if(!MSGID_SayText)
			return;
	}

    va_list argptr;
    static char buffer[191];
     
    va_start ( argptr, pMessage);
    vsnprintf( buffer, sizeof(buffer), pMessage, argptr );
    va_end   ( argptr );

	buffer[190] = 0;

	if(pEntity)
	{
		MESSAGE_BEGIN( MSG_ONE_UNRELIABLE, MSGID_SayText, NULL, pEntity);
			WRITE_BYTE( ENTINDEX( pEntity ) );
			WRITE_STRING( buffer );
		MESSAGE_END();
	}
	else
	{
		for(int id = 1; id <= gpGlobals->maxClients; id++)
		{
			if( MF_IsPlayerIngame(id) ) // amxx
			{
				MESSAGE_BEGIN( MSG_ONE_UNRELIABLE, MSGID_SayText, NULL, INDEXENT(id));
					WRITE_BYTE( id );
					WRITE_STRING( buffer );
				MESSAGE_END();
			}
		}
	}
}  

void PlaySound(char* sound)
{
	edict_t* pPlayer;
	for(int i = 1; i <= gpGlobals->maxClients; i++)
	{
		if( MF_IsPlayerIngame(i) ) // amxx
		{
			pPlayer = INDEXENT(i);
			CLIENT_COMMAND(pPlayer, "spk \"%s\"\n", sound);
		}
	}
}

bool IS_VALID_ENT(const edict_t* ent)
{
	if(FNullEnt(ent))
	{
		return false;
	}

	int iEnt = ENTINDEX(ent);
	if (iEnt < 1 || iEnt > gpGlobals->maxEntities) 
		return false;

	if (iEnt <= gpGlobals->maxClients)
	{
		if (!MF_IsPlayerIngame(iEnt)) // amxx
		{
			return false;
		}
	}

	return true;
}

bool IS_VALID_ENT(const int index)
{
	if (index < 1 || index > gpGlobals->maxEntities) 
		return false;

	if (index <= gpGlobals->maxClients)
	{
		if (!MF_IsPlayerIngame(index)) // amxx
		{
			return false;
		}
	} else {
		if (FNullEnt(INDEXENT(index)))
		{
			return false;
		}
	}

	return true;
}

// Print to console.
/*void DLLINTERNAL LOG_CONS(const char *fmt, ...)
{
	va_list ap;
	char buf[MAX_LOGMSG_LEN], buf2[MAX_LOGMSG_LEN];
	unsigned int len;

	va_start(ap, fmt);
	_vsnprintf(buf2, sizeof(buf2), fmt, ap);
	va_end(ap);

	snprintf(buf, sizeof(buf)-1, "[%s] %s", MODULE_LOGTAG, buf2);

	len=strlen(buf);
	if(len < sizeof(buf)-2) {	// -1 null, -1 for newline
		buf[len+0] = '\n';
		buf[len+1] = 0;
	}
	else
		buf[len-1] = '\n';

	SERVER_PRINT(buf);
}*/