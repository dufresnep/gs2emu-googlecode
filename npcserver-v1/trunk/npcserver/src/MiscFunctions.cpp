#include "gmThread.h"
#include "MiscFunctions.h"

//Function getPlayers();
//Returns: table of all player objects
int GM_CDECL func_getPlayers(gmThread * a_thread)
{
	TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;
	
	std::map<int, TPlayer *> * playerList = server->getGraalPlayers();

	int count = 0;
	
	gmTableObject * m_table;

	m_table = server->getMachine()->AllocTableObject();

	for (std::map<int,TPlayer * >::iterator i = playerList->begin(); i != playerList->end(); ++i)
	{
		gmVariable gmPlayer;
		gmPlayer.SetUser(server->getMachine(),i->second,TPlayer::s_typeId);
		m_table->Set(server->getMachine(),count,gmPlayer);
		count++;	
	}

	a_thread->PushTable(m_table);
	return GM_OK;	
}

//Function: sendtorc(text);
//Returns: void
int GM_CDECL func_sendtorc(gmThread * a_thread)
{
	TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;
	CString message;
	if(a_thread->ParamType(0) == GM_INT)
	{
		message = a_thread->Param(0).m_value.m_int;
	}
	else if(a_thread->ParamType(0) == GM_FLOAT)
	{
		message = a_thread->Param(0).m_value.m_float;
	}
	else 
		message = a_thread->Param(0).GetCStringSafe();

	server->rcChat(message);

  return GM_OK;                                        
}

//Function: sendPM(account,text);
//Returns: void
int GM_CDECL func_sendPM(gmThread * a_thread)
{
	TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;
	CString account = a_thread->Param(0).GetCStringSafe();
	CString message = a_thread->Param(1).GetCStringSafe();

	server->sendPlayerPM(account,message);

	return GM_OK;   
}

//Function: getPlayer(account);
//Returns: player object
int GM_CDECL func_findplayer(gmThread * a_thread)
{
	TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;

	CString account = a_thread->Param(0).GetCStringSafe();

	TPlayer * pPlayerToGet = server->getGraalPlayer(account);
	
	if (pPlayerToGet == 0) return GM_OK;  

	a_thread->PushNewUser(pPlayerToGet,TPlayer::s_typeId);

	return GM_OK;  

}

//Function: getPlayerCount();
//Returns: Total Number of Players
int GM_CDECL func_getPlayerCount(gmThread * a_thread)
{
	TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;
	a_thread->PushInt(server->getPlayerCount());
  return GM_OK;                                        
}

//Function: tokenize(string,delim);
//Returns: returns table of tokens 
int GM_CDECL func_tokenize(gmThread * a_thread)
{
	CString stringToTokenize = a_thread->Param(0).GetCStringSafe();
	CString tokenChar = a_thread->Param(1).GetCStringSafe();

	std::vector<CString> tokneizedString = stringToTokenize.tokenize(tokenChar);

	gmMachine * machine = a_thread->GetMachine();

	gmTableObject * m_table;

	m_table = machine->AllocTableObject();
	
	int count = 0;
	for (std::vector<CString>::iterator i = tokneizedString.begin(); i != tokneizedString.end(); ++i)
	{
		gmVariable param;
		if (i->isNumber()) 
		{
			float val = (float)atof(i->text());
			param.SetFloat(val);
		}
		else 
		{
			param.SetString(machine->AllocStringObject(i->text()));
		}
		m_table->Set(machine,count,param);
		count++;
	}

	a_thread->PushTable(m_table);

	return GM_OK;    
}








