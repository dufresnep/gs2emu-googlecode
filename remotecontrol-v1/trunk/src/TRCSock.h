#ifndef TRCSOCK_H
#define TRCSOCK_H

#include "CBuffer.h"
#include "CSocket.h"
#include "TTabWidget.h"

class TRCSock;
typedef void (TRCSock::*TRCSockFunc)(CPacket&);
void createRCFunc();

class TRCSock : public TTabWidget, public CSocket
{
	public:
		TRCSock();
		~TRCSock();

		/* Event-Functions */
		int main();
		void end();

		static TRCSock* createSock(CBuffer pTabName, CBuffer pIp, int pPort);

		/* RC-Functions */
		QWidget* createWindow(int pWindowType);
		TPlayer* findPlayer(int pId);
		void openFolderConfig();
		void openPlayerList();
		void openServerOptions();
		void sendText(CBuffer pText);

		/* Data-Functions */
		void compressOut();
		void sendOut(CBuffer pPacket);
		void sendPacket(CPacket pPacket);

		/* Packet-Functions */
		static TRCSockFunc msgFuncs[];
		void parsePacket(CPacket& pPacket);

		void msgRCI_NULL(CPacket& pPacket);
		void msgRCI_OPLPROPS(CPacket& pPacket);
		void msgRCI_DISCMSG(CPacket& pPacket);
		void msgRCI_SIGNATURE(CPacket& pPacket);
		void msgRCI_SERVERFLAGS(CPacket& pPacket);
		void msgRCI_GRAALTIME(CPacket& pPacket);
		void msgRCI_ADDPLAYER(CPacket& pPacket);
		void msgRCI_DELPLAYER(CPacket& pPacket);
		void msgRCI_CHATLOG(CPacket& pPacket);
		void msgRCI_OPENATTRS(CPacket& pPacket);
		void msgRCI_GETSVROPTS(CPacket& pPacket);
		void msgRCI_GETFLDRCFG(CPacket& pPacket);

	private:
		CBuffer inPacket, outPacket;
		TPlayerWindow playerWindow;

		/* Window-Objects */
		std::vector<TPlayer *> playerList;
		std::vector<QWidget *> windowList;
};

#endif // TRCSOCK_H
