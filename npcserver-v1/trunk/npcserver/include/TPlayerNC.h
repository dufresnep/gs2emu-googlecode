#ifndef NPLAYER_H
#define NPLAYER_H

#include <math.h>
#include <algorithm>
#include <vector>
#include "CString.h"
#include "IUtil.h"
#include "CSocket.h"
#include "TScriptWeapon.h"
#include "TScriptClass.h"
#include "CFileQueue.h"

class TScriptWeapon;
class TScriptClass;
class TNPCServer;




class TPlayerNC :  public CSocketStub
{
public:
	TPlayerNC(CSocket* pSocket, TNPCServer* pNPCServer);
	~TPlayerNC();

	//CSocketStub
	bool onRecv();
	bool onSend()				{ return true; }
	SOCKET getSocketHandle()	{ return playerSock->getHandle(); }
	bool canRecv()				{ return true; }
	bool canSend()				{ return false; }
	void doMain();

	//Callbacks
	bool msgPLI_NULL(CString& pPacket);
	bool msgPLI_RC_CHAT(CString& pPacket);
	bool msgPLI_RC_WEAPONLISTGET(CString& pPacket);
	bool msgPLI_NC_WEAPONGET(CString& pPacket);
	bool msgPLI_LOGIN(CString& pPacket);
	bool msgPLI_NC_WEAPONADD(CString& pPacket);
	bool msgPLI_NC_WEAPONDELETE(CString& pPacket);
	bool msgPLI_NC_LEVELLISTGET(CString& pPacket);
	bool msgPLI_NC_CLASSEDIT(CString& pPacket);
	bool msgPLI_NC_CLASSADD(CString& pPacket);
	bool msgPLI_NC_CLASSDELETE(CString& pPacket);

	//Get functions
	CSocket* playerSock;
	CString account,password,Nickname,version;
	
	std::map<CString, CString> folderRights;
	
	CString rBuffer, sBuffer, oBuffer;
	int type;

	TNPCServer* server;

	unsigned int dataSize;
	CFileQueue fileQueue;

	void createFunctions();
	bool sendPacket(CString pPacket);
	void sendCompress();
	bool parsePacket(CString& pPacket);
};
#endif
