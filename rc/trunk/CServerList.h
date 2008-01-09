#ifndef CSERVERLIST_H
#define CSERVERLIST_H

#include "CBuffer.h"
#include "CPacket.h"
#include "CSocket.h"
#include "CStringList.h"

class CServer
{
	public:
		CServer(CString name, CString language, CString description, CString url, CString version, int playercount, CString ip, int port);
		~CServer();
		
		CString name, language, description, url, version, ip;
		int id, playercount, port, type;
};

#endif