#include "PlayerFunctions.h"

//Function: playerobj.removeweapon(weapon);
//Returns: void
int GM_CDECL func_removeweapon(gmThread * a_thread)
{
	TPlayer * pPlayer = (TPlayer*)a_thread->ThisUser();

	if (!pPlayer) return GM_OK;

	CString weapon = a_thread->Param(0).GetCStringSafe();
	
	TNPCServer * server = pPlayer->GetServer();

	server->sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_PLAYERWEAPONDEL 
		>> (short)pPlayer->GetId() << weapon);

	return GM_OK;
}

//Function: playerobj.addweapon(weapon);
//Returns: void
int GM_CDECL func_addweapon(gmThread * a_thread)
{
	TPlayer * pPlayer = (TPlayer*)a_thread->ThisUser();

	if (!pPlayer) return GM_OK;

	CString weapon = a_thread->Param(0).GetCStringSafe();
	
	TNPCServer * server = pPlayer->GetServer();

	server->sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_PLAYERWEAPONADD 
		>> (short)pPlayer->GetId() << weapon);

	return GM_OK;
}

//Function: playerobj.setlevel2(level,x,y);
//Returns: void
int GM_CDECL func_setlevel2(gmThread* a_thread)
	{
	TPlayer * pPlayer = (TPlayer*)a_thread->ThisUser();

	if (!pPlayer) return GM_OK;

	CString newLevel = a_thread->Param(0).GetCStringSafe();
	float x = 0;
	float y = 0;

	if (a_thread->Param(1).IsInt())  x = (float)a_thread->Param(1).GetInt();
	else if (a_thread->Param(1).IsFloat())  x = a_thread->Param(1).GetFloat();

	if (a_thread->Param(2).IsInt())  y = (float)a_thread->Param(2).GetInt();
	else if (a_thread->Param(2).IsFloat())  y = a_thread->Param(2).GetFloat();
	
	
	TNPCServer * server = pPlayer->GetServer();

	server->sendToGserver(CString() >> (char)GO_NC_QUERY >> (char)NCO_PLAYERPACKET 
									>> (short)pPlayer->GetId() >> (char)PLI_LEVELWARP 
									>> (char)(x * 2) >> (char)(y * 2) << newLevel); 
							
	return GM_OK;
}

//Function: playerobj.var;
//Returns: value of var
void GM_CDECL TPlayerGetDot(gmThread * a_thread, gmVariable * a_operands)
{
	//O_GETDOT = 0,       // object, "member"          (tos is a_operands + 2)
	GM_ASSERT(a_operands[0].m_type == TPlayer::s_typeId);
	gmUserObject * user = (gmUserObject *) GM_OBJECT(a_operands[0].m_value.m_ref);
	TPlayer * pPlayer = (TPlayer *) user->m_user;

	std::map<CString, CString> propList = pPlayer->GetPropList();
	CString prop = a_operands[1].GetCStringSafe();


	TNPCServer * pServer = pPlayer->GetServer();

	if (pServer->playerPropAccess[prop].valueType == PROP_UNKOWN) 
	{
		a_operands[0].Nullify();
		return;
	}

	gmVariable propReturn;
	CString propGet;
	CString propData = pPlayer->GetPlayerProp(pServer->playerPropAccess[prop].propEnumValue);

	switch(pServer->playerPropAccess[prop].packetType)
	{
	case PROP_STRING:
		propGet = propData.readChars(propData.readGUChar());
		propReturn.SetString(pPlayer->machine.AllocStringObject(propGet.text(), propGet.length()));
		a_operands[0] = propReturn;
		break;
	case PROP_CHAR:
		propGet = propData.readGUChar();
		switch(pServer->playerPropAccess[prop].valueType)
		{
		case PROP_FLOAT:
		{
			if (prop == "x" || prop == "y") 
			{
				float posVal = (float)atoi(propGet.text());
				posVal /= 2;
				propReturn.SetFloat(posVal);
				a_operands[0] = propReturn;
			}
		}
		break;
		}
		break;
	case PROP_INT:
		propReturn.SetInt(propData.readGUInt());
		a_operands[0] = propReturn;
		break;
	
	}
}

//Function: playerobj.var=val;
//Returns: sets var to val
void GM_CDECL TPlayerSetDot(gmThread * a_thread, gmVariable * a_operands)
{
	//O_SETDOT,           // object, value, "member"   (tos is a_operands + 3)
	GM_ASSERT(a_operands[0].m_type == TPlayer::s_typeId);
	gmUserObject * user = (gmUserObject *) GM_OBJECT(a_operands[0].m_value.m_ref);
	TPlayer * pPlayer = (TPlayer *) user->m_user;

	std::map<CString, CString> propList = pPlayer->GetPropList();

	CString propName = a_operands[2].GetCStringSafe();

	CString propStringValue;
	float propFloatValue = 0;


	if (a_operands[1].IsFloat()) propStringValue = propFloatValue = a_operands[1].GetFloat();
	
	else if (a_operands[1].IsInt()) 
	{	
		propFloatValue = (float)a_operands[1].GetInt();
		propStringValue = a_operands[1].GetInt();
	}

	else propStringValue = a_operands[1].GetCStringSafe();

	TNPCServer * pServer = pPlayer->GetServer();

	//SPECIAL NEEDS
	if (propName == "x" || propName == "y") 
	{
		if (!propStringValue.isEmpty())
			propFloatValue = (float)strtofloat(propStringValue);
		propFloatValue *= 2;
	}
	//END SPECIAL NEEDS

	if (!pServer->playerPropAccess[propName].readOnly)
	{
		//What type of value are we trying to set?
		switch (pServer->playerPropAccess[propName].valueType)
		{
		case PROP_STRING://Prop type is String
			pPlayer->SetProps(CString() >> (char)pServer->playerPropAccess[propName].propEnumValue >> (char)propStringValue.length() << propStringValue.text(),true);
			break;
		case PROP_INT: //prop type is int/float
			//How should we send the packet?
			switch (pServer->playerPropAccess[propName].packetType)
			{
			case PROP_CHAR://prop packet type is char
				pPlayer->SetProps(CString() >> (char)pServer->playerPropAccess[propName].propEnumValue >> (char)propFloatValue,true);
				break;
			case PROP_INT://prop packet type is int
				pPlayer->SetProps(CString() >> (char)pServer->playerPropAccess[propName].propEnumValue >> (int)propFloatValue,true);
				break;
			}
			break;
		}
	pPlayer->SendProp(pServer->playerPropAccess[propName].propEnumValue);
	}
}