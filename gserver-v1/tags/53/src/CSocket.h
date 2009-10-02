#ifndef HCSOCKET
#define HCSOCKET

//Requires CBuffer
#ifdef WIN32
#include <windows.h>
#define socklen_t int
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EMSGSIZE WSAEMSGSIZE
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#endif
#include "CBuffer.h"
class CSocket
{
    int sockId;
    bool udp;
    bool blocking;
    public:
    CSocket(char* pAddress, int pPort);
    CSocket(int pSockId);
    CSocket(){sockId = 0;}
    ~CSocket();
    bool connect(char* pAddress, int pPort = 0);
    bool listen(int pPort, int pMax);
    CSocket* accept();
    char* tcpIp();
    void setSync(bool pBlocking);
    void closeSock();
	static int lastError();
    bool udpBind(int pPort);
	int sendBytes(char* pSource, int pLength, char* pIp = "", int pPort = 0);
    int sendBuffer(CBuffer& pSource, char* pIp = "", int pPort = 0);
    int sendMessage(CBuffer& pSource, char* pIp = "", int pPort = 0);
    int receiveBytes(CBuffer& pDestination, int pCount = 8195, int pMode = 0);
    int receiveMessage(CBuffer& pDestination);
    int receiveText(CString& pDestination, char* pSep);
	void setNagle(bool pOn);
    int getSocket();
    static void sockStart();
    static void sockCleanup();
};

inline int CSocket::getSocket()
{
    return sockId;
}

inline int CSocket::lastError()
{
	return 0;
#ifdef WIN32
//	return WSAGetLastError();
#else
	return errno;
#endif
}

inline int CSocket::sendBuffer(CBuffer& pSource, char* pIp, int pPort)
{
	return sendBytes(pSource.text(), pSource.length(), pIp, pPort);
}

#endif // HCSOCKET

