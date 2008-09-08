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
	SLSPING
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
	GSVPING = 99,
};

#endif
