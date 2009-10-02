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

int GM_CDECL func_setshape(gmThread * a_thread)
{

	TNPC * pNPC = (TNPC *)a_thread->ThisUser();

	if (!pNPC) return GM_OK;
	float width = 0;
	float height = 0;

	if (a_thread->Param(0).IsInt())  width = (float)a_thread->Param(0).GetInt();
	else if (a_thread->Param(0).IsFloat())  width = a_thread->Param(0).GetFloat();

	if (a_thread->Param(1).IsInt())  height = (float)a_thread->Param(1).GetInt();
	else if (a_thread->Param(1).IsFloat())  height = a_thread->Param(1).GetFloat();

	pNPC->width = width;
	pNPC->height = height;

	return GM_OK;
}
