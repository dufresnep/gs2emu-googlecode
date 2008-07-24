#ifndef HCSOCKET
#define HCSOCKET

// Requires CBuffer
#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <winsock2.h>
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

#ifndef _WIN32
	#include <sys/select.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <errno.h>
#endif

#include "CString.h"

class CSocket
{
	public:
		CSocket(const char *pAddress, int pPort);
		CSocket(int pSockId);
		CSocket() { sockId = 0; }
		virtual ~CSocket();

		// server
		bool listenSock(int pPort, int pMax);
		bool udpBind(int pPort);
		CSocket* accept();

		// client
		bool connectSock(const char *pAddress, int pPort = 0);

		// both
		char* tcpIp();
		int getSocket();
		void closeSock();
		void setNagle(bool pOn);
		void setSync(bool pBlocking);

		// data-sending
		int receiveBytes(CString& pDestination, int pCount = 8195);
		int sendBytes(const char *pSource, int pLength, const char *pIp = "", int pPort = 0);
		int sendBuffer(const CString& pSource, const char *pIp = "", int pPort = 0);

		// static
		static int lastError();
		static int sockStart();
		static void sockCleanup();

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
	#if defined(_WIN32)
		return WSAGetLastError();
	#elif defined(PSPSDK)
		return sceNetInetGetErrno();
	#else
		return errno;
	#endif
}

inline int CSocket::sendBuffer(const CString& pSource, const char *pIp, int pPort)
{
	return sendBytes(pSource.text(), pSource.length(), pIp, pPort);
}

#endif // HCSOCKET
