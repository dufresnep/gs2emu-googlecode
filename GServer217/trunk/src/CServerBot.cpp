// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#include "CServerBot.h"

#ifdef WIN32
    #include "zlib.h"
#endif

#ifndef WIN32
    #include <zlib.h>
#endif


bool CServerBot::connect(char* pServer, int pPort)
{
	CString username = "SERVBOT";
	CString pass = "SERVBOT";

	botSock = new CSocket();
	if(!botSock->connectSock(pServer, pPort))
		return false;
	botSock->setSync(false);
	CPacket packet;
	packet << (char)2 << "GSRVBOT" << (char)0 <<
		(char)username.length() << username << (char)pass.length() << pass << "\n";
	sendPacket(packet);
	return true;
}
bool CServerBot::main()
{
	/*
	char uncompressedData[9001];
	CBuffer receiveBuff;
	if (botSock->receiveBytes(receiveBuff, 9000) < 0)
	{
		return false;
	}

	for (int i = 0; i < receiveBuff.length(); i+= 2)
	{
		int error, cLen = 9000;
		int len = (((unsigned char)receiveBuff[i]) << 8) + (unsigned char)receiveBuff[i+1];
		if ((error = uncompress((Bytef*)uncompressedData,(uLongf*)&cLen,(const Bytef*)&receiveBuff.text()[i+2], len)) == Z_OK)
		{
			i += len;
			CBuffer encrypted;
			encrypted.writeBytes(uncompressedData, cLen);
			while (encrypted.bytesLeft())
			{
				CBuffer line = encrypted.readString("\n");
				if (!line.length())
					break;
				parsePacket((CPacket&)line);
			}
		}
		else
		{
			printf("Compression error in server bot\n");
			return false;
		}
	}*/
	CBuffer receiveBuff;
	if(botSock->receiveBytes(receiveBuff, 9000) < 0)
		return false;
	return true;
}

void CServerBot::parsePacket(CPacket& pPacket)
{
	int messageId = pPacket.readByte1();
	printf("SERVERBOT: %i\n", messageId);

}
void CServerBot::sendPacket(CPacket& pPacket)
{
	char buffer[65535];
	unsigned long clen = 65535;
	compress((unsigned char*)buffer,&clen,(unsigned char*)pPacket.text(), pPacket.length());
	CBuffer sendString;
	sendString.writeByte((char)((clen >> 8) & 0xFF));
	sendString.writeByte((char)(clen & 0xFF));
	sendString.writeBytes(buffer, clen);
	botSock->sendBuffer(sendString);
}
