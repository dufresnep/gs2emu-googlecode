#ifndef NCONTROL_H
#define NCONTROL_H

#include <iostream>
using namespace std;

#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "CString.h"
#include "IUtil.h"
#include <map>
#include "CSocket.h"
#include <stdio.h>
#include "CFileQueue.h"


class TNPCServer;

class TGserverConn : public CSocketStub
{
	public:
		bool onRecv();
		bool onSend()				{ return true; }
		bool onRegister()			{ return true; }
		void onUnregister()			{}
		SOCKET getSocketHandle()	{ return gserverSock.getHandle(); }
		bool canRecv()				{ return true; }
		bool canSend()				{ return false; }
		
		TGserverConn();
		bool init();
		bool connect(CString host, int port);
		void login(CString nickname, CString account,CString password, int port);
		bool disconnect();
		bool sendPacket(CString pPacket);
		bool parsePacket(CString& pPacket);
		void sendCompress();
		void doMain();

		bool msgPLI_LOGIN(CString& pPacket);
		bool msgPLI_NULL(CString& pPacket);
		bool msgGI_NPCPROPS(CString& pPacket);
		bool msgGI_NPCWEAPONADD(CString& pPacket);
		bool msgGI_OTHERPLPROPS(CString& pPacket);
		bool msgGI_FLAGSET(CString& pPacket);
		bool msgGI_PRIVATEMESSAGE(CString& pPacket);
		bool msgGI_NEWWORLDTIME(CString& pPacket);
		bool msgGI_NPCSERVERSIDE(CString& pPacket);
		bool msgGI_NC_REQUEST(CString& pPacket);
		bool msgGI_NPCADDCLASS(CString& pPacket);
		bool msgGI_TRIGGERACTION(CString& pPacket);
		bool msgGI_LEVELNAME(CString& pPacket);
		bool msgGI_LEVELLINK(CString& pPacket);
		bool msgGI_LEVELSIGN(CString& pPacket);
		bool msgGI_LEVELMODTIME(CString& pPacket);
		bool msgGI_NPCDEL(CString& pPacket);
		bool msgGI_PLAYER_RIGHTS(CString& pPacket);
		void sendPM(int playerId, CString message);
		static bool created;
		static void createFunctions();

		void sendToGserver(CString pPacket);
		
		//void rcPrint(CString message);
		
		//void updateLevelList();
		
		CSocket gserverSock;
		CString rBuffer, sBuffer, oBuffer;
		CFileQueue fileQueue;

		TNPCServer * server;
		
		int NPCLOGIN;
		int SENDNICK;
		int type;
		CString homepath;
		
		unsigned int dataSize;
};
#endif
