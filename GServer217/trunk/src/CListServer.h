/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#ifndef CLISTSERVERH
#define CLISTSERVERH

#include "CBuffer.h"
#include "CList.h"
#include "CPacket.h"
#include "CSocket.h"
#include "CStringList.h"

void ListServer_Connect();
void ListServer_End();
void ListServer_Main();
void ListServer_Send(CPacket &pPacket);

enum
{
	SLSNAME,
	SLSDESC,
	SLSLANG,
	SLSVER,
	SLSURL,
	SLSIP,
	SLSPORT,
	SLSCOUNT,
	SLSACCOUNT,
	SLSGUILD,
	SLSFILE,
	SLSNICKNAME,
	SLSPROFREQ,
	SLSPROFSET,
	SLSPLAYERADD,
	SLSPLAYERREM,
	SLSPING,
	SLSVERIACC2,
	SLSSETLOCALIP
};

enum
{
	GSVACCOUNT,
	GSVGUILD,
	GSVFILEC,
	GSVFILED,
	GSVFILES,
	GSVOLD,
	GSVCURRENT,
	GSVPROFILE,
	GSVMSG,
	GSVNULL4,
	GSVNULL5,
	GSVACCOUNT2,
	GSVPING = 99,
};

#endif
