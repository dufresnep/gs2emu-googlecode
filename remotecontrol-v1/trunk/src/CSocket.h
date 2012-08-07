/* GraalReborn Server
    $Id: CSocket.h 167 2008-03-04 06:32:02Z agret@graalforge.com $
 (C) GraalReborn 2007 */

#ifndef HCSOCKET
#define HCSOCKET

// Requires CBuffer
#if defined(WIN32)
	#include <windows.h>
	#define socklen_t int
	#define EWOULDBLOCK WSAEWOULDBLOCK
	#define EMSGSIZE WSAEMSGSIZE
#elif defined(PSPSDK)
	#include <pspkernel.h>
	#include <pspdebug.h>
	#include <pspsdk.h>
	#include <pspnet.h>
	#include <pspnet_inet.h>
	#include <pspnet_apctl.h>
	#include <pspnet_resolver.h>
	#include <psputility_netmodules.h>

	#define accept sceNetInetAccept
	#define bind sceNetInetBind
	#define close sceNetInetClose
	#define connect sceNetInetConnect
	#define getpeername sceNetInetGetpeername
	#define getsockname sceNetInetGetsockname
	#define getsockopt sceNetInetGetsockopt
	#define listen sceNetInetListen
	#define recv sceNetInetRecv
	#define recvfrom sceNetInetRecvfrom
	#define recvmsg sceNetInetRecvmsg
	#define send sceNetInetSend
	#define sendto sceNetInetSendto
	#define sendmsg sceNetInetSendmsg
	#define setsockopt sceNetInetSetsockopt
	#define shutdown sceNetInetShutdown
	#define socket sceNetInetSocket
#endif

#ifndef WIN32
	#include <sys/select.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <errno.h>
#endif

#include "CPacket.h"

class CSocket
{
	public:
		CSocket(char* pAddress, int pPort);
		CSocket(int pSockId);
		CSocket(){sockId = 0;}
		virtual ~CSocket();

		bool connectSock(char* pAddress, int pPort = 0);
		bool listenSock(int pPort, int pMax);
		bool udpBind(int pPort);
		char* tcpIp();
		CSocket* accept();
		int getSocket();
		int receiveBytes(CBuffer& pDestination, int pCount = 8195);
		int sendBytes(char* pSource, int pLength, char* pIp = "", int pPort = 0);
		int sendBuffer(CBuffer& pSource, char* pIp = "", int pPort = 0);
		void setNagle(bool pOn);
		void setSync(bool pBlocking);
		void closeSock();
		static int lastError();
		static int sockStart();
		static void sockCleanup();

		virtual int main() { return -1; }
		virtual void end() { }

	protected:
	 	int sockId;
		bool blocking, udp;

};

inline int CSocket::getSocket()
{
	return sockId;
}

inline int CSocket::lastError()
{
	#if defined(WIN32)
		return WSAGetLastError();
	#elif defined(PSPSDK)
		return sceNetInetGetErrno();
	#else
		return errno;
	#endif
}

inline int CSocket::sendBuffer(CBuffer& pSource, char* pIp, int pPort)
{
	return sendBytes(pSource.text(), pSource.length(), pIp, pPort);
}

#endif // HCSOCKET

