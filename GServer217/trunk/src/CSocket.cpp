/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

#ifndef __GNUC__ // rain
	#pragma comment( lib, "ws2_32.lib" )
#endif
	// 	Some of these might not be valid in Linux, but I don't really care right now.
	//#define WSANOTINITIALISED	WSANOTINITIALISED
	#define ENETDOWN			WSAENETDOWN
	#define EADDRINUSE			WSAEADDRINUSE
	#define EINTR				WSAEINTR
	#define EINPROGRESS			WSAEINPROGRESS
	#define EALREADY			WSAEALREADY
	#define EADDRNOTAVAIL		WSAEADDRNOTAVAIL
	#define EAFNOSUPPORT		WSAEAFNOSUPPORT
	#define ECONNREFUSED		WSAECONNREFUSED
	#define EFAULT				WSAEFAULT
	#define EINVAL				WSAEINVAL
	#define EISCONN				WSAEISCONN
	#define ENETUNREACH			WSAENETUNREACH
	#define ENOBUFS				WSAENOBUFS
	#define ENOTSOCK			WSAENOTSOCK
	#define ETIMEDOUT			WSAETIMEDOUT
	#define EWOULDBLOCK			WSAEWOULDBLOCK
	#define EACCES				WSAEACCES
	#define ENOTCONN			WSAENOTCONN
	#define ENETRESET			WSAENETRESET
	#define EOPNOTSUPP			WSAEOPNOTSUPP
	#define ESHUTDOWN			WSAESHUTDOWN
	#define EMSGSIZE			WSAEMSGSIZE
	#define ECONNABORTED		WSAECONNABORTED
	#define ECONNRESET			WSAECONNRESET
	#define EHOSTUNREACH		WSAEHOSTUNREACH
	#define SHUT_WR				SD_SEND

#else
	#include <netdb.h>
	#include <errno.h>
	#include <unistd.h>

	#include <sys/socket.h>
	#include <netinet/in.h>
	#define SOCKET_ERROR	-1
	#define INVALID_SOCKET	(unsigned int)-1
#endif

#include <memory.h>
#include <stdio.h>
#include <stdarg.h>

#include "CSocket.h"
#include "main.h"
#include "COther.h"

int CSocket::was_initiated = 0;

// Class functions
CSocket::CSocket()
{
	if ( CSocket::was_initiated == 0 ) socketSystemInit();
	properties.handle = 0;
	properties.protocol = 0;
	properties.type = 0;
	properties.options = 0;
	properties.state = SOCKET_STATE_DISCONNECTED;
	memset( (char *)&properties.description, 0, SOCKET_MAX_DESCRIPTION );
}

CSocket::CSocket( CString& host, CString& port, sock_properties* properties )
{
	if ( CSocket::was_initiated == 0 ) socketSystemInit();
	if ( properties != 0 )
		memcpy( (void*)&this->properties, properties, sizeof( sock_properties ) );
	else
	{
		this->properties.handle = 0;
		this->properties.protocol = SOCKET_PROTOCOL_TCP;
		this->properties.type = SOCKET_TYPE_CLIENT;
		this->properties.options = 0;
		this->properties.state = SOCKET_STATE_DISCONNECTED;
		memset( (char *)&this->properties.description, 0, SOCKET_MAX_DESCRIPTION );
	}
	this->init( host, port );
}

CSocket::~CSocket()
{
	// 	Destroy if still connected.
	if ( properties.state != SOCKET_STATE_DISCONNECTED )
		disconnect();
}

int CSocket::init( CString& host, CString& port )
{
	struct addrinfo hints;
	struct addrinfo* res;

	// Type: 6 = TCP, 17 = UDP
	// PF_INET, SOCK_STREAM, IPPROTO_TCP/IPPROTO_UDP

	// 	Make sure a TCP socket is disconnected.
	if ( properties.protocol == SOCKET_PROTOCOL_TCP && properties.state != SOCKET_STATE_DISCONNECTED )
	{
		errorOut( "debuglog.txt", "[CSocket::sock_init] TCP socket must be disconnected first." );
		return SOCKET_ALREADY_CONNECTED;
	}

	// 	Announce what we are going to do.
	if ( properties.type == SOCKET_TYPE_CLIENT )
		errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Pointing socket to " << host << ":" << port << "... " );
	else
		errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Setting socket to port " << port );

	// Start creating the hints.
	memset( (struct sockaddr_storage*)&properties.address, 0, sizeof(struct sockaddr_storage) );
	memset( (void*)&hints, 0, sizeof(hints) );
	if ( properties.protocol == SOCKET_PROTOCOL_TCP ) hints.ai_socktype = SOCK_STREAM;
	if ( properties.protocol == SOCKET_PROTOCOL_UDP ) hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;

	// 	Create the host.
	int error;
	if ( properties.type == SOCKET_TYPE_CLIENT && host.length() != 0 )
		error = getaddrinfo( host.text(), port.text(), &hints, &res );
	else if ( properties.type == SOCKET_TYPE_SERVER )
	{
		hints.ai_flags = AI_PASSIVE;		// Local socket.
		error = getaddrinfo( 0, port.text(), &hints, &res );
	}

	// Check for errors.
	if ( error )
	{
		errorOut( "debuglog.txt", "[CSocket::sock_init] getaddrinfo() returned an error." );
		return SOCKET_HOST_UNKNOWN;
	}
	else
		memcpy( (void*)&properties.address, res->ai_addr, res->ai_addrlen );

	return 0;
}

void CSocket::destroy()
{
	// 	Shut down the socket.
	errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Shutting down socket..." );
	if ( shutdown( properties.handle, SHUT_WR ) == SOCKET_ERROR )
		identifyError();

	properties.state = SOCKET_STATE_TERMINATING;

	// 	Destroy the socket of d00m.
#if defined(_WIN32) || defined(_WIN64)
	if ( closesocket( properties.handle ) == SOCKET_ERROR )
	{
#else
	if ( close( properties.handle ) == SOCKET_ERROR )
	{
#endif
		errorOut( "debuglog.txt", CString() << "[CSocket::sock_destroy] Error closing socket: " << properties.description );
		identifyError();
	}

	// 	Reset the socket state.
	properties.state = SOCKET_STATE_DISCONNECTED;
}

int CSocket::connect()
{
	// 	Make sure the socket is disconnected.
	if ( properties.state != SOCKET_STATE_DISCONNECTED )
	{
		errorOut( "debuglog.txt", "[CSocket::sock_connect] Socket is already connected." );
		return SOCKET_ALREADY_CONNECTED;
	}

	// 	Check if the description exists.
	if ( properties.description == 0 )
	{
		errorOut( "debuglog.txt", "[CSocket::sock_connect] Socket description not found." );
		return SOCKET_UNKNOWN_DESC;
	}

	// 	Announce what we are going to do.
	errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Establishing socket..." );

	// 	Flag the socket as connecting.
	properties.state = SOCKET_STATE_CONNECTING;

	// 	Create socket.
	if ( properties.protocol == SOCKET_PROTOCOL_TCP )
		properties.handle = (unsigned int)socket( AF_INET, SOCK_STREAM, 0 );
	else
		properties.handle = (unsigned int)socket( AF_INET, SOCK_DGRAM, 0 );

	// 	Make sure the socket was created correctly.
	if ( properties.handle == INVALID_SOCKET )
	{
		errorOut( "debuglog.txt", "[CSocket::sock_connect] socket() returned INVALID_SOCKET." );
		properties.state = SOCKET_STATE_DISCONNECTED;
		return SOCKET_INVALID;
	}

	// 	Bind the socket if it is a server-type socket.
	if ( properties.type == SOCKET_TYPE_SERVER )
	{
		errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Binding socket..." );
		if ( ::bind( properties.handle, (struct sockaddr *)&properties.address, sizeof(properties.address) ) == SOCKET_ERROR )
		{
			errorOut( "debuglog.txt", "[CSocket::sock_connect] bind() returned SOCKET_ERROR." );
			identifyError();
			#if defined(_WIN32) || defined(_WIN64)
				closesocket( properties.handle );
			#else
				close( properties.handle );
			#endif
			properties.state = SOCKET_STATE_DISCONNECTED;
			return SOCKET_BIND_ERROR;
		}
	}

	// 	Connect the socket.
	errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Connecting socket..." );
	if ( properties.type != SOCKET_TYPE_SERVER )
	{
		if ( ::connect( properties.handle, (struct sockaddr *)&properties.address, sizeof(properties.address) ) == SOCKET_ERROR )
		{
			errorOut( "debuglog.txt", "[CSocket::sock_connect] connect() returned SOCKET_ERROR." );
			identifyError();
			#if defined(WIN32) || defined(WIN64)
				closesocket( properties.handle );
			#else
				close( properties.handle );
			#endif
			properties.state = SOCKET_STATE_DISCONNECTED;
			return SOCKET_CONNECT_ERROR;
		}
	}

	// 	Socket connected!
	properties.state = SOCKET_STATE_CONNECTED;

	// 	Listening sockets.
	if ( properties.type == SOCKET_TYPE_SERVER )
	{
		if ( properties.protocol == SOCKET_PROTOCOL_UDP )
		{
			errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Socket entering a listening state." );
			properties.state = SOCKET_STATE_LISTENING;
		}
		else if ( properties.protocol == SOCKET_PROTOCOL_TCP )
		{
			errorOut( "debuglog.txt", CString() << ":: " << properties.description << " - Setting socket to a listening state..." );
			if ( ::listen( properties.handle, SOMAXCONN ) == SOCKET_ERROR )
			{
				errorOut( "debuglog.txt", "[CSocket::sock_connect] listen() returned SOCKET_ERROR." );
				identifyError();
				#if defined(WIN32) || defined(WIN64)
					closesocket( properties.handle );
				#else
					close( properties.handle );
				#endif
				properties.state = SOCKET_STATE_DISCONNECTED;
				return SOCKET_CONNECT_ERROR;
			}

			properties.state = SOCKET_STATE_LISTENING;
		}
	}

	return 0;
}

int CSocket::disconnect()
{
	destroy();
	return 0;
}

int CSocket::reconnect( long delay, int tries )
{
	int socket_reconnect_delay = 0;
	int socket_reconnect_attempts = 1;

//	setting_geti( "socket_reconnect_delay", &socket_reconnect_delay );
//	setting_geti( "socket_reconnect_attempts", &socket_reconnect_attempts );

	if ( delay == 0 )
		delay = socket_reconnect_delay;
	if ( tries == 0 )
		tries = socket_reconnect_attempts;

	for ( int i = 0; i < tries; i++ )
	{
		switch ( this->connect() )
		{
			case 0:
			case SOCKET_ALREADY_CONNECTED:
				return 0;
				break;
			case SOCKET_INVALID:
			case SOCKET_BIND_ERROR:
			case SOCKET_CONNECT_ERROR:
			default:
				// 	Do nothing.
				break;
		}
		if ( delay != 0 ) wait( delay );
	}
	return SOCKET_CONNECT_ERROR;
}

CSocket* CSocket::accept()
{
	// 	Make sure the socket is connected!
	if ( properties.state == SOCKET_STATE_DISCONNECTED )
		return 0;

	// Only server type TCP sockets can accept new connections.
	if ( properties.type != SOCKET_TYPE_SERVER || properties.protocol != SOCKET_PROTOCOL_TCP )
		return 0;

	sockaddr_storage addr;
	int addrlen = sizeof( addr );
	unsigned int handle = 0;

	// Try to accept a new connection.
#if defined(_WIN32) || defined(_WIN64)
	handle = (unsigned int)::accept( properties.handle, (struct sockaddr*)&addr, &addrlen );
#else
	handle = (unsigned int)::accept( properties.handle, (struct sockaddr*)&addr, (socklen_t*)&addrlen );
#endif
	if ( handle == -1 )
	{
		identifyError( 1 );
		return 0;
	}

	// Create the new socket to store the new connection.
	CSocket* sock = new CSocket();
	sock_properties props;
	memcpy( (void*)&props.address, &addr, sizeof( sockaddr_storage ) );
	props.options = properties.options;
	props.protocol = properties.protocol;
	props.type = SOCKET_TYPE_CLIENT;
	props.state = SOCKET_STATE_CONNECTED;
	props.handle = handle;
	sock->setProperties( props );
	sock->setDescription( sock->tcpIp() );

	return sock;
}

int CSocket::sendData( CPacket& data )
{
	int intError;

	// 	Make sure the socket is connected!
	if ( properties.state == SOCKET_STATE_DISCONNECTED )
		return SOCKET_INVALID;

	// 	Send our data, yay!
	if ( send( properties.handle, data.text(), data.length(), 0 ) == SOCKET_ERROR )
	{
		intError = identifyError();
		switch ( intError )
		{
			case ENETDOWN:
			case ENETRESET:
			case ENOTCONN:
			case EHOSTUNREACH:
			case ECONNABORTED:
			case ECONNRESET:
			case ETIMEDOUT:
				// Destroy the bad socket and create a new one.
				errorOut( "debuglog.txt", CString() << ":: " << properties.description << " connection lost!" );
				disconnect();
				return intError;
				break;
		}
		return intError;
	}

	return 0;
}

int CSocket::getData()
{
	int intError;
	char buff[ 0x10000 ]; // 65536 bytes, 64KB
	CPacket temp;

	// 	Make sure it is connected!
	if ( properties.state == SOCKET_STATE_DISCONNECTED )
		return SOCKET_ERROR;

	do
	{
		// Allocate buff.
		memset( (char *)buff, 0, 0x10000 );

		// 	Get our data
		if ( properties.protocol == SOCKET_PROTOCOL_UDP )
			intError = recvfrom( properties.handle, buff, 0x10000, 0, 0, 0 );
		else
			intError = recv( properties.handle, buff, 0x10000, 0 );

		// Add to the buffer.
		temp << buff;

		// 	Check for error!
		if ( intError == SOCKET_ERROR )
		{
			intError = identifyError();
			switch ( intError )
			{
				case ENETDOWN:
				case ENETRESET:
				case ENOTCONN:
				case EHOSTUNREACH:
				case ECONNABORTED:
				case ECONNRESET:
				case ETIMEDOUT:
				case ESHUTDOWN:
					// 	Destroy the bad socket and create a new one.
					errorOut( "debuglog.txt", CString() << ":: " << properties.description << " connection lost!" );
					disconnect();
					break;
				default:
					break;
			}
		}
	} while ( intError > 0 );

	// Add the data we just got to the buffer.
	buffer << temp;

	// 	Return the amount of data obtained.
	if ( temp.length() == 0 ) return -1;
	return temp.length();
}

char* CSocket::peekData()
{
	int recvsize = 0x10000;
	int intError;
	char *buff = NULL;

	// 	Make sure it is connected!
	if ( properties.state == SOCKET_STATE_DISCONNECTED )
		return 0;

	// 	Make a buffer to store data!
	buff = new char[ recvsize ];
	memset( (char *)buff, 0, recvsize );

	// 	Get our data
	if ( properties.protocol == SOCKET_PROTOCOL_UDP )
		intError = recvfrom( properties.handle, buff, recvsize, MSG_PEEK, NULL, NULL );
	else
		intError = recv( properties.handle, buff, recvsize, MSG_PEEK );

	// 	Check for error!
	if ( intError == SOCKET_ERROR )
	{
		intError = identifyError();
		switch ( intError )
		{
			case ENETDOWN:
			case ENETRESET:
			case ENOTCONN:
			case EHOSTUNREACH:
			case ECONNABORTED:
			case ECONNRESET:
			case ETIMEDOUT:
			case ESHUTDOWN:
				// 	Destroy the bad socket and create a new one.
				errorOut( "debuglog.txt", CString() << ":: " << properties.description << " connection lost!" );
				disconnect();
				break;
			default:
				break;
		}
		if ( buff )
		{
			delete [] buff;
			buff = 0;
		}
		return 0;
	}

	// 	Return the data.
	return buff;
}

int CSocket::setProtocol( int sock_proto )
{
	if ( properties.state == SOCKET_STATE_DISCONNECTED )
		properties.protocol = sock_proto;
	else
		return SOCKET_INVALID;
	return 0;
}

int CSocket::setType( int sock_type )
{
	if ( properties.state == SOCKET_STATE_DISCONNECTED )
		properties.type = sock_type;
	else
		return SOCKET_INVALID;
	return 0;
}

int CSocket::setOptions( int iOptions )
{
	// 	Check if the options are a-0.
	properties.options = iOptions;

	return 0;
}

int CSocket::setDescription( const char *strDescription )
{
	memset( (void*)&properties.description, 0, SOCKET_MAX_DESCRIPTION );
	memcpy( (void*)&properties.description, strDescription, SOCKET_MAX_DESCRIPTION - 1 );
	return 0;
}

int CSocket::setProperties( sock_properties newprop )
{
	memcpy( (void *)&this->properties, (void *)&newprop, sizeof( sock_properties ) );
	return 0;
}

int CSocket::setState( int iState )
{
	this->properties.state = iState;
	return 0;
}

const char* CSocket::tcpIp()
{
	int addr_size = sizeof(sockaddr_storage);
	char* hostret;
	char host[1025];
	memset( (void*)host, 0, 1025 );

	int error = getnameinfo( (struct sockaddr*)&properties.address, sizeof( struct sockaddr ), host, 1025, 0, 0, NI_NUMERICSERV );
	if ( error ) return 0;
	hostret = host;
	return hostret;
}

int CSocket::socketSystemInit()
{
	errorOut( "debuglog.txt", ":: Initializing socket system..." );
#if defined(_WIN32) || defined(_WIN64)
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return 1;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup();
		return 1;
	}
#endif
	CSocket::was_initiated = 1;
	return 0;
}

void CSocket::socketSystemDestroy()
{
	errorOut( "debuglog.txt", ":: Destroying socket system..." );
#if defined(_WIN32) || defined(_WIN64)
	int intTimeCheck = 0;

	while ( intTimeCheck++ < 10 )
	{
		if ( WSACleanup() == SOCKET_ERROR )
			identifyError();
		wait( 1000 );
	}
#endif
}

int identifyError( int source )
{
#if defined(WIN32) || defined(WIN64)
	int i_error = WSAGetLastError();
#else
	int i_error;
	if ( source != 0 )
		i_error = h_errno;
	else
		i_error = errno;
#endif
	errorOut( "debuglog.txt", CString() << "Socket error: [" << toString(i_error) << "]" );

	switch ( i_error )
	{
#if defined(_WIN32) || defined(_WIN64)
		case WSANOTINITIALISED:
			errorOut( "debuglog.txt", "WSANOTINITIALISED" ); break;
#endif
		case ENETDOWN:
			errorOut( "debuglog.txt", "ENETDOWN" ); break;
		case EADDRINUSE:
			errorOut( "debuglog.txt", "EADDRINUSE" ); break;
		case EINTR:
			errorOut( "debuglog.txt", "EINTR" ); break;
		case EINPROGRESS:
			errorOut( "debuglog.txt", "EINPROGRESS" ); break;
		case EALREADY:
			errorOut( "debuglog.txt", "EALREADY" ); break;
		case EADDRNOTAVAIL:
			errorOut( "debuglog.txt", "EADDRNOTAVAIL" ); break;
		case EAFNOSUPPORT:
			errorOut( "debuglog.txt", "EAFNOSUPPORT" ); break;
		case ECONNREFUSED:
			errorOut( "debuglog.txt", "ECONNREFUSED" ); break;
		case EFAULT:
			errorOut( "debuglog.txt", "EFAULT" ); break;
		case EINVAL:
			errorOut( "debuglog.txt", "EINVAL" ); break;
		case EISCONN:
			errorOut( "debuglog.txt", "EISCONN" ); break;
		case ENETUNREACH:
			errorOut( "debuglog.txt", "ENETUNREACH" ); break;
		case ENOBUFS:
			errorOut( "debuglog.txt", "ENOBUFS" ); break;
		case ENOTSOCK:
			errorOut( "debuglog.txt", "ENOTSOCK" ); break;
		case ETIMEDOUT:
			errorOut( "debuglog.txt", "ETIMEDOUT" ); break;
		case EWOULDBLOCK:
			errorOut( "debuglog.txt", "EWOULDBLOCK" ); break;
		case EACCES:
			errorOut( "debuglog.txt", "EACCES" ); break;
		case ENOTCONN:
			errorOut( "debuglog.txt", "ENOTCONN" ); break;
		case ENETRESET:
			errorOut( "debuglog.txt", "ENETRESET" ); break;
		case EOPNOTSUPP:
			errorOut( "debuglog.txt", "EOPNOTSUPP" ); break;
		case ESHUTDOWN:
			errorOut( "debuglog.txt", "ESHUTDOWN" ); break;
		case EMSGSIZE:
			errorOut( "debuglog.txt", "EMSGSIZE" ); break;
		case ECONNABORTED:
			errorOut( "debuglog.txt", "ECONNABORTED" ); break;
		case ECONNRESET:
			errorOut( "debuglog.txt", "ECONNRESET" ); break;
		case EHOSTUNREACH:
			errorOut( "debuglog.txt", "EHOSTUNREACH" ); break;
		default:
			break;
			//g_printf( "\n" ); break;
	}

	return i_error;
}













/*
#include "main.h"
#include "CSocket.h"
#define MIN(a, b) (a < b ? a : b)

CSocket::CSocket(char* pAddress, int pPort)
{
	udp = false;
	sockId = 0;
	blocking = false;
	connectSock(pAddress, pPort);
}

void CSocket::setNagle(bool pOn)
{
	int nagle = !pOn;
	setsockopt(sockId, IPPROTO_TCP, 1,(char*)&nagle, sizeof(nagle));

}
CSocket::CSocket(int pSockId)
{
	int type, typeSize = sizeof(int);
	getsockopt(pSockId, SOL_SOCKET, SO_TYPE, (char*)&type, (socklen_t*)&typeSize);

	udp = (type == SOCK_DGRAM);
	blocking = false;
	sockId = pSockId;
	memset( (void *)&addr, 0, sizeof(sockaddr_in) );
}

bool CSocket::connectSock(char* pAddress, int pPort)
{
	hostent*  hostEntry;
	udp = false;
	if(sockId == 0)
	{
		if((sockId = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
		{
#ifdef _WIN32
			errorOut( "errorlog.txt", CBuffer() << "[Error] CSocket::connectSock - socket() returned " << toString(h_errno) << ".", true );
#else
			errorOut( "errorlog.txt", CBuffer() << "[Error] CSocket::connectSock - socket() returned " << toString(errno) << ": " << strerror(errno), true );
#endif
			return false;
		}
	}

	if ( addr.sin_family != AF_INET )
	{
		if((hostEntry = gethostbyname(pAddress)) == NULL)
		{
#ifdef _WIN32
			errorOut( "errorlog.txt", CBuffer() << "[Error] CSocket::connectSock - gethostbyname() returned " << toString(h_errno) << ".", true );
#else
			errorOut( "errorlog.txt", CBuffer() << "[Error] CSocket::connectSock - gethostbyname() returned " << toString(h_errno) << ": " << hstrerror(errno), true );
#endif
			closeSock();
			return false;
		}

		addr.sin_family = AF_INET;
		addr.sin_addr = *((in_addr*)*hostEntry->h_addr_list);
		addr.sin_port = htons((unsigned short)pPort);
	}

	if (connect(sockId, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1)
	{
#ifdef _WIN32
		errorOut( "errorlog.txt", CBuffer() << "[Error] CSocket::connectSock - connect() failed.  Returned " << toString(h_errno) << ".", true );
#else
		errorOut( "errorlog.txt", CBuffer() << "[Error] CSocket::connectSock - connect() failed.  Returned " << toString(errno) << ": " << strerror(errno), true );
#endif
		closeSock();
		return false;
	}
	return true;
}

bool CSocket::listenSock(int pPort, int pMax)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(pPort);
	udp = false;

	if(sockId == 0)
	{
		if((sockId = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
			return false;
	}

	int value = 1;
	setsockopt(sockId,SOL_SOCKET,SO_REUSEADDR,(char*)&value,sizeof(value));
	if(bind(sockId, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1)
		return false;

	if(::listen(sockId, pMax) == -1)
		return false;
	return true;
}

CSocket* CSocket::accept()
{
	int newSock;
	if((newSock = (int)::accept(sockId, NULL, NULL)) > 0)
		return new CSocket(newSock);
	return NULL;
}

char* CSocket::tcpIp()
{
	sockaddr_in addr;
	int addr_size = sizeof(sockaddr_in);
	if(getpeername(sockId, (sockaddr *)&addr, (socklen_t*)&addr_size) == -1)
		return NULL;
	return inet_ntoa(addr.sin_addr);
}

void CSocket::setSync(bool pBlocking)
{
	blocking = pBlocking;

	#if defined(WIN32)
		unsigned long i = !pBlocking;
		ioctlsocket(sockId, FIONBIO, &i);
	#elif defined(PSPSDK)
		unsigned long i = !pBlocking;
		sceNetInetSetsockopt(sockId, SOL_SOCKET, 0x1009, (const char*)&i, sizeof(u32));
	#else
		if (!pBlocking)
			fcntl(sockId, F_SETFL, O_NONBLOCK);
	#endif
}

bool CSocket::udpBind(int pPort)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(pPort);
	if (sockId == 0)
	{
		if((sockId = (int)socket(AF_INET, SOCK_DGRAM, 0)) == 0)
			return false;
	}

	if (bind(sockId,(sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
		return false;
	udp = true;
	return true;
}

int CSocket::sendBytes(char* pSource, int pLength, char* pIp, int pPort)
{
	sockaddr_in addr;
	int size = 0;
	if(pLength <= 0)
		return 0;
	if(!blocking)
	{
		fd_set set;
		struct timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET((unsigned int)sockId,&set);
		select(sockId+1,NULL,&set,NULL,&tm);
		if (!FD_ISSET(sockId,&set))
			return 0;
	}
	if(udp)
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(pPort);
		addr.sin_addr.s_addr = inet_addr(pIp);
		size = sendto(sockId, pSource, pLength, 0, (sockaddr *)&addr, sizeof(sockaddr_in));
		if(size == 0)
		{
			closeSock();
			return -1;
		}
		else return size;
	}
	size = send(sockId, pSource, pLength, MSG_NOSIGNAL);
	if(size == 0)
	{
		closeSock();
		return -1;
	}
	else return size;
}


int CSocket::sendMessage(CBuffer& pSource, char* pIp, int pPort)
{
	if(udp)
		return sendBuffer(pSource, pIp, pPort);
	CBuffer sendBuff;
	sendBuff.writeuShort(pSource.length());
	sendBuff.writeBuffer(pSource);
	return sendBuffer(sendBuff);
}

int CSocket::receiveBytes(CBuffer& pDestination, int pCount, int pMode)
{
	char buff[8192];
	int size = 0;
	fd_set set;
	struct timeval tm;
	pDestination.clear();
	tm.tv_sec = 0;
	tm.tv_usec = 0;
	while(size < pCount)
	{
		if(!blocking)
		{
			FD_ZERO(&set);
			FD_SET((unsigned int)sockId,&set);
			select(sockId+1,&set,NULL,NULL,&tm);
			if (!FD_ISSET(sockId,&set))
				return size;
		}
		int len = pCount - size;
		if(len > 8192)
			len = 8192;
		if(len < 0)
			return size;

		int a = recv(sockId, buff, len, MSG_NOSIGNAL);
		if (a == 0)
		{
			closeSock();
			return -1;
		}

		if(a < 0)
		{
			if (lastError() == EWOULDBLOCK || lastError() == EMSGSIZE)
				break;
			else return -lastError();
		}
		size+=a;
		pDestination.writeBytes(buff, a);
		if(udp)
			break;
	}
	return size;
}

int CSocket::receiveMessage(CBuffer& pDestination)
{
	int retVal = -1;

	if(udp)
		return receiveBytes(pDestination);

	 if(((retVal = receiveBytes(pDestination, 2))) == 2)
		return receiveBytes(pDestination, pDestination.readuShort());
	return retVal;
}


int CSocket::receiveText(CString& pDestination, char* pSep)
{
	char buffer[65536];
	int size;

	if(!udp)
	{
		size = recv(sockId, buffer, sizeof(buffer), MSG_PEEK);
		if(size <= 0)
			return size;
		buffer[size] = 0;
		int length = (int)(strstr(buffer, pSep) - buffer);

		if(length >= 0)
			return receiveBytes(pDestination, length + (int)strlen(pSep));
		closeSock();
		return -1;
	}
	return -1;
}

CSocket::~CSocket()
{
	closeSock();
}

void CSocket::closeSock()
{
	if(sockId != 0)
	{
		shutdown(sockId, 1);
		#ifdef WIN32
			closesocket(sockId);
		#else
			close(sockId);
		#endif
	}
	sockId = 0;
}

int CSocket::sockStart()
{
	#ifdef WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2),&wsaData) != 0)
			return -1;
	#elif PSPSDK
		if (sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON) < 0)
			return -1;
		if (sceUtilityLoadNetModule(PSP_NET_MODULE_INET) < 0)
			return -1;
		if (pspSdkInetInit() != 0)
			return -2; // false
		if (sceNetApctlConnect(1) != 0)
			return -3;

		while (true)
		{
			int state = 0;
			if (sceNetApctlGetState(&state) != 0)
				return -3;
			if (state == 4)
				break;

			sceKernelDelayThread(1000 * 50); // 50ms
		}
	#endif

	return 0;
}

void CSocket::sockCleanup()
{
	#ifdef WIN32
		WSACleanup();
	#endif
}
*/
