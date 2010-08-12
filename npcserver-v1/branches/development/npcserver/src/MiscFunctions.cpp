#include "gmThread.h"
#include "MiscFunctions.h"
#include "md5.h"

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

//Function: echo(text);
//Returns: void
int GM_CDECL func_sendtorc_echo(gmThread * a_thread)
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

	server->rcChat("NPC (Server): " + message);

  return GM_OK;                                        
}

//Function: sendtonc(text);
//Returns: void
int GM_CDECL func_sendtonc(gmThread * a_thread)
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

	server->ncChat(message);

  return GM_OK;                                        
}

//Function: sendpm(account,text);
//Returns: void
int GM_CDECL func_sendPM(gmThread * a_thread)
{
	TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;
	CString account = a_thread->Param(0).GetCStringSafe();
	CString message = a_thread->Param(1).GetCStringSafe();

	server->sendPlayerPM(account,message);

	return GM_OK;   
}

//Function: sendrpgmessage(account,text);
//Returns: void
int GM_CDECL func_sendRPGMessage(gmThread * a_thread)
{
	TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;
	CString account = a_thread->Param(0).GetCStringSafe();
	CString message = a_thread->Param(1).GetCStringSafe();

	server->sendPlayerRPGMessage(account,message);

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

//Function: uppercase(string);
//Returns: returns string in uppercase
int GM_CDECL func_uppercase(gmThread * a_thread)
{
	CString stringToConvert = a_thread->Param(0).GetCStringSafe();
	CString upperString = stringToConvert.toUpper();

	gmMachine * machine = a_thread->GetMachine();
	a_thread->PushString(machine->AllocStringObject(upperString.text()));

	return GM_OK;    
}

//Function: lowercase(string);
//Returns: returns string in lowercase
int GM_CDECL func_lowercase(gmThread * a_thread)
{
	CString stringToConvert = a_thread->Param(0).GetCStringSafe();
	CString lowerString = stringToConvert.toLower();

	gmMachine * machine = a_thread->GetMachine();
	a_thread->PushString(machine->AllocStringObject(lowerString.text()));

	return GM_OK;    
}

//Function: md5(string);
//Returns: returns string as an md5 hash
int GM_CDECL func_md5(gmThread * a_thread)
{
	CString stringToConvert = a_thread->Param(0).GetCStringSafe();
	MD5String(stringToConvert.text(),stringToConvert.length());

    // CString md5String = (char*)md5sum;

	// TNPCServer * server = (TNPCServer*)a_thread->GetFunctionObject()->m_cUserData;
	// server->rcChat(md5String);

	// printf("MD5: %s\n", (char*)md5sum);
	//gmMachine * machine = a_thread->GetMachine();
	// a_thread->PushString(machine->AllocStringObject(md5String.text()));

	return GM_OK;    
}





