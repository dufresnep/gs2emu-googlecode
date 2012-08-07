#include "main.h"
#include "TRCListSock.h"

/*
	Pointer-Functions for Packets
*/
TRCListSockFunc TRCListSock::msgFuncs[] =
{
	&TRCListSock::msgSERVERLIST_SL,
	&TRCListSock::msgSERVERLIST_1,
	&TRCListSock::msgSERVERLIST_2,
	&TRCListSock::msgSERVERLIST_HOMEURL,
	&TRCListSock::msgSERVERLIST_ERROR,
	&TRCListSock::msgSERVERLIST_PAYURL,
};

/*
	Constructor / Destructor
*/

TRCListSock::TRCListSock()
{
	sockList.add(this);
}

TRCListSock::~TRCListSock()
{
	sockList.remove(this);
}

/*
	Main Loop - Read/Send Packets etc.
*/
int TRCListSock::main()
{
	CBuffer recvBuff;
	int len = 0, size = 0;

	// Read Buffer
	if ((size = receiveBytes(recvBuff, 65536)) < 0)
	{
		closeSock();
		return -1;
	}

	inPacket << recvBuff;

	// Uncompress / Parse Buffer
	while (inPacket.length() >= 2)
	{
		len = (((unsigned int)(unsigned char)inPacket[0]) << 8) + (unsigned int)(unsigned char)inPacket[1];

		// wrong data package length
		if (len < 0)
		{
			closeSock();
			return -1;
		}

		if (inPacket.length() >= 2+len)
		{
			CBuffer buf = decompressBuffer(inPacket.copy(2, len));
			inPacket.remove(0, len+2);

			// sent empty package
			if (buf.length() <= 0)
			{
				closeSock();
				return -1;
			}

			// parse stuff
			while (buf.bytesLeft())
			{
				parsePacket(CPacket() << buf.readString("\n"));
			}
		}
		else break;
	}

	// Send Buffer
	compressOut();
	return 1;
}

/*
	End Function - Called on closeSock
*/
void TRCListSock::end()
{

}

/*
	Packet-Sending Functions
*/
void TRCListSock::compressOut()
{
	/* Input Data Check */
	if (outPacket.length() <= 0)
		return;

	/* Declare Variables */
	CBuffer buf, newPacket;

	/* Compress Data */
	buf = compressBuffer(outPacket);
	outPacket.clear();

	/* Pack Data & Send */
	newPacket.writeByte((char)((buf.length() >> 8) & 0xFF));
	newPacket.writeByte((char)(buf.length() & 0xFF));
	newPacket.writeBuffer(buf);
	sendOut(newPacket);
}

void TRCListSock::sendOut(CBuffer pPacket)
{
	int sendLength = 0;

	while (pPacket.length() > 0)
	{
		sendLength = min(pPacket.length(), 1024);
		int len = 0;
		if ((len = sendBytes(pPacket.text(), sendLength)) > 0)
			pPacket.remove(0, len);
		else break;
	}
}

void TRCListSock::sendPacket(CPacket pPacket)
{
	outPacket << pPacket;
	if (pPacket[pPacket.length() - 1] != '\n')
		outPacket << "\n";

	if (outPacket.length() > 4*1024)
		compressOut();
}

/*
	Packet-Parsing Functions
*/
void TRCListSock::parsePacket(CPacket& pPacket)
{
	/* Input Data Check */
	if (pPacket.length() <= 0)
		return;

	/* Declare Variables */
	int packetId = (unsigned char)pPacket.readByte1();

	/* Call Function */
	(*this.*msgFuncs[packetId])(pPacket);
}

void TRCListSock::msgSERVERLIST_SL(CPacket& pPacket)
{
	printf("packet 0: %s\n", pPacket.text()+1);
	ServerWindow->setServers(pPacket);
}

void TRCListSock::msgSERVERLIST_1(CPacket& pPacket)
{
	printf("packet 1: %s\n", pPacket.text()+1);
}

void TRCListSock::msgSERVERLIST_2(CPacket& pPacket)
{
	printf("packet 2: %s\n", pPacket.text()+1);
}

void TRCListSock::msgSERVERLIST_HOMEURL(CPacket& pPacket)
{
	printf("packet 3: %s\n", pPacket.text()+1);
}

void TRCListSock::msgSERVERLIST_ERROR(CPacket& pPacket)
{
	if (pPacket.length() < 1)
		return;

	// back to login screen
	LoginWindow->setVisible(true);
	ServerWindow->setVisible(false);

	// warning box
	QMessageBox::warning(NULL, QString("Serverlist Error"), QString(pPacket.text()+1), QMessageBox::Ok);
}

void TRCListSock::msgSERVERLIST_PAYURL(CPacket& pPacket)
{
	printf("packet 5: %s\n", pPacket.text()+1);
}
