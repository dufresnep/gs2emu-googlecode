#include "CSocket.h"
#define MIN(a, b) (a < b ? a : b)

CSocket::CSocket(char* pAddress, int pPort)
{
	udp = false;
	sockId = 0;
	blocking = false;
	connect(pAddress, pPort);
}

void CSocket::setNagle(bool pOn)
{
	int nagle = !pOn;
	setsockopt(sockId, IPPROTO_TCP, 1,(char*)&nagle, sizeof(nagle));

}
CSocket::CSocket(int pSockId)
{
	int type;
	int typeSize = sizeof(int);
	getsockopt(pSockId, SOL_SOCKET, SO_TYPE, (char*)&type, (socklen_t*)&typeSize);
	switch(type)
	{
		case SOCK_DGRAM:
			udp = true;
		break;

		case SOCK_STREAM:
			udp = false;
		break;
	}
	blocking = false;
	sockId = pSockId;
}

bool CSocket::connect(char* pAddress, int pPort)
{
	sockaddr_in addr;
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
	if(::connect(sockId, (sockaddr*)&addr, sizeof(sockaddr_in)) == -1)
	{
		sockId = 0;
		return false;
	}
	return true;
}

bool CSocket::listen(int pPort, int pMax)
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
	#ifdef WIN32
	unsigned long i = !pBlocking;
	ioctlsocket(sockId, FIONBIO, &i);
	#else
	if(!pBlocking)
		fcntl(sockId, F_SETFL, O_NONBLOCK);
	#endif
}

bool CSocket::udpBind(int pPort)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(pPort);
	if(sockId == 0)
	{
		if((sockId = (int)socket(AF_INET, SOCK_DGRAM, 0)) == 0)
			return false;
	}

	if(bind(sockId,(sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
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
			return -1;
		else return size;
	}
	size = send(sockId, pSource, pLength, 0);
	if(size == 0)
		return -1;
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

		int a = recv(sockId, buff, len, 0);
		if(a == 0)
			return -1;
		if(a < 0)
		{
			if(lastError() == EWOULDBLOCK || lastError() == EMSGSIZE)
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

void CSocket::sockStart()
{
	#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2),&wsaData);
	#endif
}

void CSocket::sockCleanup()
{
	#ifdef WIN32
	WSACleanup();
	#endif
}
