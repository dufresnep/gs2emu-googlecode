// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef HCSERVERBOT
#define HCSERVERBOT
#include "CPacket.h"
#include "CSocket.h"

//This bot connects to your server and does shit
//Not currently used...may use in future

class CServerBot
{
	CSocket* botSock;
	
	public:
		bool connect(char* pServer, int pPort);
		bool main();
		void parsePacket(CPacket& pPacket);
		void processChat(CString& pFrom, CString& pMessage);
		void sendPacket(CPacket& pPacket);
};
#endif
