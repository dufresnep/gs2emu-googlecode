/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#ifndef CSOCKET_H
#define CSOCKET_H

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

#define SOCKET_OPTION_NONBLOCKING		(unsigned int)0x0001

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
	#include <arpa/inet.h>

	typedef unsigned int SOCKET;
#endif

//#define WIN2000_BUILD

//! Properties to pass to the socket.
struct sock_properties
{
	SOCKET handle;
	int protocol;
	int type;
	int options;
	int state;
	char description[ SOCKET_MAX_DESCRIPTION ];
#ifdef WIN2000_BUILD
	sockaddr_in address;
#else
	sockaddr_storage address;
#endif
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

		SOCKET getHandle();
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
		const char* getLocalIp();

		static void socketSystemDestroy();
};

inline
SOCKET CSocket::getHandle()
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
