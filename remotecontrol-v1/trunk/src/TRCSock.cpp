#include "main.h"
#include "TRCSock.h"

/*
	Pointer-Functions for Packets
*/
std::vector<TRCSockFunc> funcs;

void createRCFunc()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 200; i++)
		funcs.push_back(&TRCSock::msgRCI_NULL);

	// now set non-nulls
	funcs[RCI_OPLPROPS] = &TRCSock::msgRCI_OPLPROPS;
	funcs[RCI_DISCMSG] = &TRCSock::msgRCI_DISCMSG;
	funcs[RCI_SIGNATURE] = &TRCSock::msgRCI_SIGNATURE;
	funcs[RCI_SERVERFLAGS] = &TRCSock::msgRCI_SERVERFLAGS;
	funcs[RCI_GRAALTIME] = &TRCSock::msgRCI_GRAALTIME;
	funcs[RCI_ADDPLAYER] = &TRCSock::msgRCI_ADDPLAYER;
	funcs[RCI_DELPLAYER] = &TRCSock::msgRCI_DELPLAYER;
	funcs[RCI_CHATLOG] = &TRCSock::msgRCI_CHATLOG;
	funcs[RCI_OPENATTRS] = &TRCSock::msgRCI_OPENATTRS;
	funcs[RCI_GETSVROPTS] = &TRCSock::msgRCI_GETSVROPTS;
	funcs[RCI_GETFLDRCFG] = &TRCSock::msgRCI_GETFLDRCFG;
}

/*
	Constructor / Destructor
*/

TRCSock::TRCSock() : TTabWidget()
{
	sockList.add(this);
}

TRCSock::~TRCSock()
{
	for (unsigned int i = 0; i < windowList.size(); i++)
		delete windowList[i];
	for (unsigned int i = 0; i < playerList.size(); i++)
		delete playerList[i];

	playerList.clear();
	windowList.clear();
	sockList.remove(this);
}

/*
	Create Socket
*/

TRCSock* TRCSock::createSock(CBuffer pTabName, CBuffer pIp, int pPort)
{
	TRCSock *sock = new TRCSock();
	sock->setSync(false);
	if (!sock->connectSock(pIp.text(), pPort))
	{
		delete sock;
		return NULL;
	}

	sock->setType(STYPE_SERVER);
	sock->setWindowTitle(pTabName.text());
	return sock;
}

/*
	Main Loop - Read/Send Packets etc.
*/
int TRCSock::main()
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
void TRCSock::end()
{
	RCWindow->removeTab(this);
}

/*
	RC-Functions
*/

QWidget* TRCSock::createWindow(int pWindowType)
{
	QWidget *newWindow = NULL;

	switch (pWindowType)
	{
		case RCW_VIEWACCOUNT:
			newWindow = new TViewAccount(this);
		break;

		case RCW_TEXTEDITOR:
			newWindow = new TTextEditor(false);
		break;

		case RCW_TEXTEDITORH:
			newWindow = new TTextEditor(true);
		break;
	}

	if (newWindow != NULL)
		windowList.push_back(newWindow);

	return newWindow;
}

TPlayer* TRCSock::findPlayer(int pId)
{
	for (unsigned int i = 0; i < playerList.size(); i++)
	{
		if (playerList[i]->plyrId == pId)
			return playerList[i];
	}

	return NULL;
}

void TRCSock::openFolderConfig()
{
	sendPacket(CPacket() << (char)RCO_GETFLDRCFG);
}

void TRCSock::openPlayerList()
{
	playerWindow.setVisible(true);
	playerWindow.raise();
}

void TRCSock::openServerOptions()
{
	sendPacket(CPacket() << (char)RCO_GETSVROPTS);
}

void TRCSock::sendText(CBuffer pText)
{
	if (pText == "/exit")
	{
		delete this;
		return;
	}

	sendPacket(CPacket() << (char)RCO_CHATLOG << pText);
}

/*
	Packet-Sending Functions
*/
void TRCSock::compressOut()
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

void TRCSock::sendOut(CBuffer pPacket)
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

void TRCSock::sendPacket(CPacket pPacket)
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
void TRCSock::parsePacket(CPacket& pPacket)
{
	/* Input Data Check */
	if (pPacket.length() <= 0)
		return;

	/* Declare Variables */
	int packetId = (unsigned char)pPacket.readByte1();
	if (packetId == 28)
		return;

	/* Call Function */
	(*this.*funcs[packetId])(pPacket);
}

void TRCSock::msgRCI_NULL(CPacket& pPacket)
{
	pPacket.setRead(0);
	printf("Unknown Packet: %i (%s)\n", pPacket.readByte1(), pPacket.text()+1);
}

void TRCSock::msgRCI_OPLPROPS(CPacket& pPacket)
{
    TPlayer *pl = findPlayer(pPacket.readByte2());
	if (pl == NULL)
		return;

	pl->setProps(pPacket);
}

void TRCSock::msgRCI_DISCMSG(CPacket& pPacket)
{
	pPacket.setRead(0);
	printf("Known Packet: %i (%s)\n", pPacket.readByte1(), pPacket.text()+1);
}

void TRCSock::msgRCI_SIGNATURE(CPacket& pPacket)
{
	RCWindow->addTab((void *)this, windowTitle(), STYPE_SERVER);
	RCWindow->setTab((void *)this);
	pPacket.clear();
}

void TRCSock::msgRCI_SERVERFLAGS(CPacket& pPacket)
{
	pPacket.setRead(0);
	printf("Known Packet: %i (%s)\n", pPacket.readByte1(), pPacket.text()+1);
}

void TRCSock::msgRCI_GRAALTIME(CPacket& pPacket)
{
	pPacket.setRead(0);
	printf("Known Packet: %i (%s)\n", pPacket.readByte1(), pPacket.text()+1);
}

void TRCSock::msgRCI_ADDPLAYER(CPacket& pPacket)
{
	TPlayer *player = new TPlayer(pPacket.readByte2());
	player->acctName = pPacket.readChars(pPacket.readByteU1());
	player->setProps(pPacket);
	playerList.push_back(player);

	playerWindow.addItem(player->getItem());
}

void TRCSock::msgRCI_DELPLAYER(CPacket& pPacket)
{
	int packetId = pPacket.readByte2();

	for (unsigned int i = 0; i < playerList.size(); i++)
	{
		if (playerList[i]->plyrId == packetId)
		{
			delete playerList[i];
			playerList.erase(playerList.begin()+i);
			break;
		}
	}
}

void TRCSock::msgRCI_CHATLOG(CPacket& pPacket)
{
	// Declare Variable
	int pos = -1;

	// Move mouse to End
	textEdit->moveCursor(QTextCursor::End);

	// Parse Text for Color
	if ((pos = pPacket.find(':')) >= 0)
	{
		textEdit->setTextColor(QColor(0, 255, 0));
		textEdit->insertPlainText(pPacket.copy(1, pos).text());
		textEdit->setTextColor(QColor(0, 0, 0));
		textEdit->insertPlainText(pPacket.copy(pos+1).text());
	}
		else
	{
		textEdit->setTextColor(QColor(0, 0, 0));
		textEdit->insertPlainText(pPacket.text()+1);
	}

	// New Line
	textEdit->insertPlainText("\n");
}

void TRCSock::msgRCI_OPENATTRS(CPacket& pPacket)
{
	TViewAccount *window = (TViewAccount *)createWindow(RCW_VIEWACCOUNT);
	if (window == NULL)
		return;

	window->setData(pPacket);
	window->setVisible(true);
}

void TRCSock::msgRCI_GETSVROPTS(CPacket& pPacket)
{
	TTextEditor *window = (TTextEditor *)createWindow(RCW_TEXTEDITORH);
	if (window == NULL)
		return;

	CBuffer sOptionsBuffer = pPacket.text()+1;
	sOptionsBuffer.untokenize();
	//sOptionsBuffer.replaceAll(",\"\",", ",,").untokenize();

	window->textEdit->setText(sOptionsBuffer.text());
	window->setWindowTitle(QString() + "Server Options of: " + windowTitle());
	window->setVisible(true);
	window->raise();
}

void TRCSock::msgRCI_GETFLDRCFG(CPacket& pPacket)
{
	TTextEditor *window = (TTextEditor *)createWindow(RCW_TEXTEDITOR);
	if (window == NULL)
		return;

	CBuffer sOptionsBuffer = pPacket.text()+1;
	sOptionsBuffer.untokenize();
	//sOptionsBuffer.replaceAll(",\"\",", ",,").untokenize();

	window->textEdit->setText(sOptionsBuffer.text());
	window->setWindowTitle(QString() + "Folder Configuration of: " + windowTitle());
	window->setVisible(true);
	window->raise();
}
