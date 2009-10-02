#include "TScriptEngine.h"
#include "IUtil.h"


void TScriptEngine::callFunction(CString function,TPlayer * pPlayer,CString params)
{        
	gmVariable returnVar;

	if (mGameMonkeyScript.isEmpty()) return;

	int errors = machine->CheckSyntax(mGameMonkeyScript.text());

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

	//Attach obj to this.
	gmUserObject * weaponObj = machine->AllocUserObject(this,scriptType);
	gmVariable weapon = gmVariable(weaponObj);

	if (thisFunctions[function] == 0) return;
	if (call.BeginFunction(machine,thisFunctions[function],weapon))
	{
		
		if (pPlayer) call.AddParamUser(pPlayer,TPlayer::s_typeId);
		if (params != "") call.AddParamTable(m_table);
		call.End();
	}
	return;
}