#include "gmThread.h"
#include "gmVariable.h"
#include "NPCFunctions.h"

int GM_CDECL func_setTimer(gmThread * a_thread)
{

	TNPC * pNPC = (TNPC *)a_thread->ThisUser();

	if (!pNPC) return GM_OK;
	float timer = -1;

	if (a_thread->Param(0).IsInt())  timer = (float)a_thread->Param(0).GetInt();
	else if (a_thread->Param(0).IsFloat())  timer = a_thread->Param(0).GetFloat();

	pNPC->setTimer(timer);

	return GM_OK;
}

int GM_CDECL func_setimg(gmThread * a_thread)
{
	TNPC * pNPC = (TNPC *)a_thread->ThisUser();
	if (!pNPC) return GM_OK;

	/*CString image = a_thread->Param(0).GetCStringSafe;
	pNPC->image = image;
	pNPC->setProps(CLVER_2_17
	pNPC->setProps(CString() >> (char)pServer->npcPropAccess[propName].propEnumValue >> (char)propFloatValue);
	pNPC->SendProp(pServer->npcPropAccess[propName].propEnumValue);*/

	return GM_OK;
}

int GM_CDECL func_setshape(gmThread * a_thread)
{
	TNPC * pNPC = (TNPC *)a_thread->ThisUser();

	if (!pNPC) return GM_OK;
	float width = 0;
	float height = 0;

	if(a_thread->Param(3).IsInt()) // Add support for official graal way of doing it ex: setshape(1,32,32);
	{
		if (a_thread->Param(1).IsInt())  width = (float)a_thread->Param(1).GetInt();
		else if (a_thread->Param(1).IsFloat())  width = a_thread->Param(1).GetFloat();

		if (a_thread->Param(2).IsInt())  height = (float)a_thread->Param(2).GetInt();
		else if (a_thread->Param(2).IsFloat())  height = a_thread->Param(2).GetFloat();
	}else{ // Graal Reborn way of doing it ex: setshape(32,32);
		if (a_thread->Param(0).IsInt())  width = (float)a_thread->Param(0).GetInt();
		else if (a_thread->Param(0).IsFloat())  width = a_thread->Param(0).GetFloat();

		if (a_thread->Param(1).IsInt())  height = (float)a_thread->Param(1).GetInt();
		else if (a_thread->Param(1).IsFloat())  height = a_thread->Param(1).GetFloat();
	}
	pNPC->width = width;
	pNPC->height = height;
    cout << "Set npc width & height" << endl;
	return GM_OK;
}

int GM_CDECL func_warpto(gmThread * a_thread)
{
	TNPC * pNPC = (TNPC *)a_thread->ThisUser();

	if (!pNPC) return GM_OK;
	float x = 0;
	float y = 0;

	if (a_thread->Param(1).IsInt())  x = (float)a_thread->Param(1).GetInt();
	else if (a_thread->Param(1).IsFloat())  x = a_thread->Param(1).GetFloat();

	if (a_thread->Param(2).IsInt())  y = (float)a_thread->Param(2).GetInt();
	else if (a_thread->Param(2).IsFloat())  y = a_thread->Param(2).GetFloat();

	/*pNPC->level = a_thread->Param(0);
	pNPC->x = x;
	pNPC->y = y;*/
	/*pNPC->TNPCSetDot(pNPC,"x")
	pNPC->setVar("y",y);*/

    cout << "Warpto Called" << endl;
	return GM_OK;
}