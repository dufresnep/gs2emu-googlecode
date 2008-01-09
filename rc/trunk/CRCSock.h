#ifndef CRCSOCK_H
#define CRCSOCK_H

#include "CBuffer.h"
#include "CPacket.h"
#include "CSocket.h"
#include "CStringList.h"
#include "ui_main.h"
#include "zlib.h"

class CRCSock : public QObject
{
	Q_OBJECT
	
	public:
		CRCSock(int type, CString server);
		~CRCSock();
		
		bool connect(char *ip, int port);
		bool main();
		CString server;
		int type;
		void parsePacket(CPacket& pPacket);
		void sendPacket(CPacket& pPacket);
		
		bool connected;
	signals:
		void createError(QString error);
		void TabRem(CRCSock *sock);
		void ListPacket(CRCSock *sock, CPacket& pPacket);
		void LivePacket(CRCSock *sock, CPacket& pPacket);
		void IrcPacket(CRCSock *sock, CPacket& pPacket);

	private:
		CSocket* RCSock;
};

#endif
