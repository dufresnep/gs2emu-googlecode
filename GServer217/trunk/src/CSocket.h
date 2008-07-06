/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#ifndef CSocket_H
#define CSocket_H

#include "CString.h"
#include "CPacket.h"

// Defines
//#define SOCK_BUFFER						8192
//#define SEND_BUFFER						4096

#define SOCKET_MAX_DESCRIPTION			51

#define SOCKET_PROTOCOL_UDP				0
#define SOCKET_PROTOCOL_TCP				1
#define SOCKET_TYPE_CLIENT				0
#define SOCKET_TYPE_SERVER				1

#define SOCKET_OPTION_PROTECTED			(unsigned int)0x8000	// 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0  - Don't let plugins delete

#define SOCKET_STATE_DISCONNECTED		0
#define SOCKET_STATE_CONNECTING			1
#define SOCKET_STATE_CONNECTED			2
#define SOCKET_STATE_LISTENING			3
#define SOCKET_STATE_TERMINATING		4

#define SOCKET_INVALID					1
#define SOCKET_HOST_UNKNOWN				2
#define SOCKET_BIND_ERROR				3
#define SOCKET_CONNECT_ERROR			4
#define SOCKET_ALREADY_CONNECTED		5
#define SOCKET_SEND_FAILED				6
#define SOCKET_UNKNOWN_DESC				7

#if defined(_WIN32) || defined(_WIN64)
	#include <winsock2.h>
#else
	#include <netinet/in.h>
 	#include <sys/socket.h>
	#include <sys/types.h>
#endif

//! Properties to pass to the socket.
struct sock_properties
{
	unsigned int handle;
	int protocol;
	int type;
	int options;
	int state;
	char description[ SOCKET_MAX_DESCRIPTION ];
	sockaddr_storage address;
};

//! Socket class.
class CSocket
{
	private:
		sock_properties properties;
		CPacket buffer;
		static int was_initiated;

		static int socketSystemInit();

	public:
		CSocket();
		CSocket( CString& host, CString& port, sock_properties* properties = 0 );
		~CSocket();

		int init( CString& host, CString& port );
		void destroy();

		int connect();
		int disconnect();
		int reconnect( long delay = 0, int tries = 1 );
		CSocket* accept();

		int sendData( CPacket& data );
		int getData();
		char* peekData();

		unsigned int getHandle();
		int getProtocol();
		int getType();
		int getOptions();
		const char *getDescription();
		int getState();
		CPacket& getBuffer();

		int setProtocol( int sock_proto );
		int setType( int sock_type );
		int setOptions( int iOptions );
		int setDescription( const char *strDescription );
		int setProperties( sock_properties newprop );
		int setState( int iState );

		const char* tcpIp();

		static void socketSystemDestroy();
};

static int identifyError( int source = 0 );


inline
unsigned int CSocket::getHandle()
{
	return properties.handle;
}

inline
int CSocket::getProtocol()
{
	return properties.protocol;
}

inline
int CSocket::getType()
{
	return properties.type;
}

inline
int CSocket::getOptions()
{
	return properties.options;
}

inline
const char *CSocket::getDescription()
{
	return properties.description;
}

inline
int CSocket::getState()
{
	return properties.state;
}

inline
CPacket& CSocket::getBuffer()
{
	return buffer;
}


#endif



/*
#ifndef HCSocket
#define HCSocket

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

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0x2000
#endif

#include "CPacket.h"

class CSocket
{
	int sockId;
	bool blocking, udp;
    sockaddr_in addr;

	public:
		CSocket(char* pAddress, int pPort);
		CSocket(int pSockId);
		CSocket(){sockId = 0;}
		~CSocket();

		bool connectSock(char* pAddress, int pPort = 0);
		bool listenSock(int pPort, int pMax);
		bool udpBind(int pPort);
		char* tcpIp();
		CSocket* accept();
		int getSocket();
		int receiveBytes(CBuffer& pDestination, int pCount = 8195, int pMode = 0);
		int receiveMessage(CBuffer& pDestination);
		int receiveText(CString& pDestination, char* pSep);
		int sendBytes(char* pSource, int pLength, char* pIp = "", int pPort = 0);
		int sendBuffer(CBuffer& pSource, char* pIp = "", int pPort = 0);
		int sendMessage(CBuffer& pSource, char* pIp = "", int pPort = 0);
		void setNagle(bool pOn);
		void setSync(bool pBlocking);
		void closeSock();
		static int lastError();
		static int sockStart();
		static void sockCleanup();
};

inline int CSocket::getSocket()
{
	return sockId;
}

inline int CSocket::lastError()
{
	return 0;
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

#endif // HCSocket
*/
