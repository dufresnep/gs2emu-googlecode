#include "TScriptWeapon.h"
#include "IUtil.h"


gmType TScriptWeapon::s_typeId = GM_FUNCTION;

TScriptWeapon::TScriptWeapon(CString newWeaponName,CString newWeaponimage, CString newWeaponScript, TNPCServer * pServer, gmMachine * weaponMachine)
{
	mWeaponName = newWeaponName;
	mWeaponImage = newWeaponimage;
	mWeaponFullScript = newWeaponScript;

	machine = weaponMachine;
	mServer = pServer;

	parseServerside();
	
	callFunction("onCreated",NULL,"");
}

TScriptWeapon::~TScriptWeapon()
{

	
}

void TScriptWeapon::callFunction(CString function,TPlayer * pPlayer,CString params)
{        
	gmVariable returnVar;

	if (mWeaponServerScript.isEmpty()) return;

	int errors = machine->CheckSyntax(mWeaponGameMonkeyScript.text());

	if(errors) return;

	gmTableObject * m_table;
	
	if (params != "")
	{
		std::vector<CString> paramList = params.tokenize(",");

		m_table = machine->AllocTableObject();
		
		int count = 0;
		for (std::vector<CString>::iterator i = paramList.begin(); i != paramList.end(); ++i)
		{
			
			gmVariable param;
			if (i->isNumber()) 
			{
				float val = (float)atof(i->text());
				param.SetFloat(val);
			}
			else 
			{
				param.SetString(pPlayer->machine.AllocStringObject(i->text()));
			}
			m_table->Set(machine,count,param);
			count++;
		}
	}

	int resultInt = 0;

	//Attach weapon to this.
	gmUserObject * weaponObj = machine->AllocUserObject(this,TScriptWeapon::s_typeId);
	gmVariable weapon = gmVariable(weaponObj);

	if (thisVariables[function] == 0) return;
	gmFunctionObject * funcObj = thisVariables[function]->GetFunctionObjectSafe();
	if (call.BeginFunction(machine,funcObj,weapon))
	{
		
		if (pPlayer) call.AddParamUser(pPlayer,TPlayer::s_typeId);
		if (params != "") call.AddParamTable(m_table);
		call.End();
	}
	returnVar = call.GetReturnedVariable();

	return;
}

void TScriptWeapon::compileScript()
{
	if (mWeaponGameMonkeyScript.isEmpty()) return;

	int errors = machine->CheckSyntax(mWeaponGameMonkeyScript.text());

	if(errors)
	{
		bool first = true;
		const char * message;
	    CString output;
		bool done = false;

		std::vector<CString> errorList;
		mServer->ncChat(CString() << "Script compiler output for " << this->getName() << ":");
		
		while((message = machine->GetLog().GetEntry(first))) 
		{
			output << message;

			if (!done) errorList.push_back(CString() << "error: " << output.subString(output.find(",")+1,output.length()-output.find(",")+1).trim());
			done= true;
		}

		mServer->ncChat(errorList[0]);

		delete message;
		machine->GetLog().Reset();
	 }
	else
	{
		gmFunctionObject * initFunction = machine->CompileStringToFunction(mWeaponGameMonkeyScript.text());

		//Attach weapon to this.
		gmUserObject * weaponObj = machine->AllocUserObject(this,TScriptWeapon::s_typeId);
		gmVariable weapon = gmVariable(weaponObj);

		if (call.BeginFunction(machine,initFunction,weapon))
		{
		
			call.End();
		}
	}
}


void TScriptWeapon::update(CString weaponImage, CString weaponScript)
{
	mWeaponImage = weaponImage;
	setScript(weaponScript);
	thisVariables.clear();
}


void TScriptWeapon::setScript(CString script)
{
	mWeaponFullScript = script;
	parseServerside();
	if (mWeaponGameMonkeyScript.isEmpty()) return;
	callFunction("onCreated",NULL,"");
}


void TScriptWeapon::parseServerside()
{
	CString script = mWeaponFullScript.readString("//#CLIENTSIDE");
	mWeaponServerScript = removeComments(script,"\xa7");
	mWeaponServerScript.removeAllI("/*");
	mWeaponServerScript.removeAllI("*/");

	//Need to parse before removing newLine delimiters
	mWeaponGameMonkeyScript = gs2ToGameMonkey(mWeaponServerScript);

	//Store cleaned gs2
	mWeaponServerScript.removeAllI("\xa7");

	for (std::vector<int>::iterator i = threads.begin(); i != threads.end(); ++i)
	{
		machine->KillThread(*i);
	}
	
	//Clear Variables
	thisVariables.clear();
	//Clear Threads
	threads.clear();

	machine->CollectGarbage();
	
	compileScript();
}

void TScriptWeapon::setVar(CString prop, gmVariable value)
{
	thisVariables[prop] = new gmVariable();
	*thisVariables[prop] = value;
}

//Function: npcobj.var
//Returns: value of var
void GM_CDECL TScriptWeapon::TScriptWeaponGetDot(gmThread * a_thread, gmVariable * a_operands)
{
	//O_GETDOT = 0,       // object, "member"          (tos is a_operands + 2)
	GM_ASSERT(a_operands[0].m_type == TScriptWeapon::s_typeId);
	gmUserObject * user = (gmUserObject *) GM_OBJECT(a_operands[0].m_value.m_ref);
	TScriptWeapon * pWeapon = (TScriptWeapon *) user->m_user;

	CString prop = a_operands[1].GetCStringSafe();

	gmVariable * thisVar = pWeapon->getThisVar(prop);

	if (thisVar == 0) 
	{
		a_operands[0].Nullify();
		return;
	}
	a_operands[0] = *thisVar;
	return;
}

//Function: npcrobj.var=val;
//Returns: sets var to val
void GM_CDECL TScriptWeapon::TScriptWeaponSetDot(gmThread * a_thread, gmVariable * a_operands)
{
	//O_SETDOT,           // object, value, "member"   (tos is a_operands + 3)
	GM_ASSERT(a_operands[0].m_type == TScriptWeapon::s_typeId);
	gmUserObject * user = (gmUserObject *) GM_OBJECT(a_operands[0].m_value.m_ref);
	TScriptWeapon * pWeapon = (TScriptWeapon *) user->m_user;

	CString propName = a_operands[2].GetCStringSafe();
	pWeapon->setVar(propName,a_operands[1]);

	return;
}

