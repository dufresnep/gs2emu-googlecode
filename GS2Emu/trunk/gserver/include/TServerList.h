#ifndef TSERVERLIST_H
#define TSERVERLIST_H

#include "CSocket.h"
#include "CString.h"

class TServerList : public CSocket
{
	public:
		// Constructor - Deconstructor
		TServerList();
		~TServerList();

		// Socket-Control Functions
		bool getConnected();
		bool main();
		bool connectServer(const CString& pServerIp, int pServerPort = 14900);
		inline bool reconnectServer();
		void sendCompress();
		void sendPacket(CString& pPacket);

		// Send New Server-Info
		void setDesc(const CString& pServerDesc);
		void setIp(const CString& pServerIp);
		void setName(const CString& pServerName);
		void setPort(int pServerPort);
		void setUrl(const CString& pServerUrl);
		void setVersion(const CString& pServerVersion);

		// Packet Functions
		void parsePacket(CString& pPacket);
		void msgSVI_NULL(CString& pPacket);

	protected:
		bool isConnected;
		CString serverIp, rBuffer, sBuffer;
		int serverPort;
};

inline bool TServerList::reconnectServer()
{
	return connectServer(serverIp, serverPort);
}

// Packet-Functions
typedef void (TServerList::*TSLSock)(CString&);
void createSLFunctions();

#endif // TSERVERLIST_H
