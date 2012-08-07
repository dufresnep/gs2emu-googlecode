#ifndef TRCLISTSOCK_H
#define TRCLISTSOCK_H

#include "CBuffer.h"
#include "CSocket.h"

class TRCListSock;
class TServerWindow;
typedef void (TRCListSock::*TRCListSockFunc)(CPacket&);

class TRCListSock : public CSocket
{
	public:
		TRCListSock();
		~TRCListSock();

		int main();
		void end();

		/* Data-Functions */
		void compressOut();
		void sendOut(CBuffer pPacket);
		void sendPacket(CPacket pPacket);

		/* Packet-Functions */
		static TRCListSockFunc msgFuncs[];
		void parsePacket(CPacket& pPacket);

		void msgSERVERLIST_SL(CPacket& pPacket);
		void msgSERVERLIST_1(CPacket& pPacket);
		void msgSERVERLIST_2(CPacket& pPacket);
		void msgSERVERLIST_HOMEURL(CPacket& pPacket);
		void msgSERVERLIST_ERROR(CPacket& pPacket);
		void msgSERVERLIST_PAYURL(CPacket& pPacket);

	private:
		CBuffer inPacket, outPacket;
};

#endif // TRCLISTSOCK_H

