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
void ListServer_SendServerHQ();

enum
{
	SLSNAME				= 0,
	SLSDESC				= 1,
	SLSLANG				= 2,
	SLSVER				= 3,
	SLSURL				= 4,
	SLSIP				= 5,
	SLSPORT				= 6,
	SLSCOUNT			= 7,
	SLSACCOUNT			= 8,
	SLSGUILD			= 9,
	SLSFILE				= 10,
	SLSNICKNAME			= 11,
	SLSPROFREQ			= 12,
	SLSPROFSET			= 13,
	SLSPLAYERADD		= 14,
	SLSPLAYERREM		= 15,
	SLSPING				= 16,
	SLSVERIACC2			= 17,
	SLSSETLOCALIP		= 18,
	SLSGETFILE2			= 19,
	SLSUPDATEFILE		= 20,
	SLSGETFILE3			= 21,
	SLSNEWSERVER		= 22,
	SLSSERVERHQPASS		= 23,
	SLSSERVERHQLEVEL	= 24,
};

enum
{
	GSVACCOUNT			= 0,
	GSVGUILD			= 1,
	GSVFILEC			= 2,
	GSVFILED			= 3,
	GSVFILES			= 4,
	GSVOLD				= 5,
	GSVCURRENT			= 6,
	GSVPROFILE			= 7,
	GSVMSG				= 8,
	GSVNULL4			= 9,
	GSVNULL5			= 10,
	GSVACCOUNT2			= 11,
	GSVFILESTART2		= 12,
	GSVFILEDATA2		= 13,
	GSVFILEEND2			= 14,
	GSVFILESTART3		= 15,
	GSVFILEDATA3		= 16,
	GSVFILEEND3			= 17,
	GSVPING				= 99,
	GSVRAWDATA			= 100,
};

#endif
