#include "CSocket.h"

#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <winsock2.h>

	#ifndef __GNUC__
		#pragma comment( lib, "ws2_32.lib" )
	#endif

	#define SHUT_WR				SD_SEND
	#define SHUT_RDWR			SD_BOTH
#endif

CSocket::CSocket(const char* pAddress, int pPort)
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
}

bool CSocket::connectSock(const char* pAddress, int pPort)
{
    sockaddr_in addr;
	memset( (void*)&addr, 0, sizeof(sockaddr_in) );
	hostent*  hostEntry;
	udp = false;
	if(sockId == 0)
	{
        if((sockId = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == 0)
            return false;
	}

    if((hostEntry = gethostbyname(pAddress)) == NULL)
		return false;

	addr.sin_family = AF_INET;
	addr.sin_addr = *((in_addr*)*hostEntry->h_addr_list);
	addr.sin_port = htons((u_short)pPort);
    if (connect(sockId, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1)
    {
        sockId = 0;
        return false;
    }
	return true;
}

bool CSocket::listenSock(int pPort, int pMax)
{
    sockaddr_in addr;
	memset( (void*)&addr, 0, sizeof(sockaddr_in) );
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
	memset( (void*)&addr, 0, sizeof(sockaddr_in) );
    int addr_size = sizeof(sockaddr_in);
    if(getpeername(sockId, (sockaddr *)&addr, (socklen_t*)&addr_size) == -1)
        return NULL;
    return inet_ntoa(addr.sin_addr);
}

void CSocket::setSync(bool pBlocking)
{
    blocking = pBlocking;

    #if defined(_WIN32)
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
	memset( (void*)&addr, 0, sizeof(sockaddr_in) );
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

int CSocket::sendBytes(const char *pSource, int pLength, const char *pIp, int pPort)
{
    sockaddr_in addr;
	memset( (void*)&addr, 0, sizeof(sockaddr_in) );
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
            return -1;
        else return size;
    }
    size = send(sockId, pSource, pLength, 0);
    if(size == 0)
        return -1;
    else return size;
}

int CSocket::receiveBytes(CString& pDestination, int pCount)
{
	char buff[8192];
	memset( (void*)buff, 0, sizeof(buff) );
	int size = 0;
	fd_set set;
	struct timeval tm;
	memset( (void*)&tm, 0, sizeof(struct timeval) );
	pDestination.clear();
	tm.tv_sec = 0;
	tm.tv_usec = 0;
	while(size < pCount)
	{
		if (!blocking)
		{
			FD_ZERO(&set);
			FD_SET((unsigned int)sockId,&set);
			select(sockId+1,&set,NULL,NULL,&tm);
			if (!FD_ISSET(sockId,&set))
				return size;
		}
		int len = pCount - size;
		if (len > 8192)
			len = 8192;
		if (len < 0)
			return size;

		int a = recv(sockId, buff, len, 0);
		if (a == 0)
		{
			shutdown( sockId, SHUT_RDWR );
			sockId = 0;
			//return -1;
			return (size > 0 ? size : -1);
		}

		if (a < 0)
		{
			if (lastError() == EWOULDBLOCK || lastError() == EMSGSIZE)
				break;
			else return -lastError();
		}
		size+=a;
		pDestination.write(buff, a);
		if (udp)
			break;
	}
	return size;
}

CSocket::~CSocket()
{
    closeSock();
}

void CSocket::closeSock()
{
    if (sockId != 0)
    {
		shutdown(sockId, 1);

		#ifdef _WIN32
			closesocket(sockId);
		#else
			close(sockId);
		#endif
    }

    sockId = 0;
}

int CSocket::sockStart()
{
    #ifdef _WIN32
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
    #ifdef _WIN32
		WSACleanup();
    #endif
}
