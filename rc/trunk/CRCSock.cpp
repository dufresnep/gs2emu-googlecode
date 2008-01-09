#include "CRCSock.h"
#include "main.h"

CRCSock::CRCSock(int type, CString server)
{
	this->type = type;
	this->server = server;
	
	RC->addSock(this);
	Connections.add(this);
}

CRCSock::~CRCSock()
{
	if (type == SERVER || type == IRC)
	{
		emit TabRem(this);
	}
	
	Connections.remove(this);
	delete RCSock;
}

bool CRCSock::connect(char* pServer, int pPort)
{
	RCSock = new CSocket();
	RCSock->sockStart();
	RCSock->setSync(false);
	
	if ((connected = RCSock->connect(pServer, pPort)) == false)
	{
		emit createError("Couldn't connect to the " + (QString)(type == SERVERLIST ? "Serverlist" : (type == SERVER ? "Server" : "IRC")));
		return connected;
	}
	
	CPacket packet;
	
	switch (type)
	{
		case IRC:
			packet << "USER " << cNickname << " . . :GRAALRC\r\nNICK " << cNickname << "\r\n\0";
		break;
		
		case SERVERLIST:
			packet << (char)0 << "rc\n" << (char)1 << (char)cAccount.length() << cAccount << (char)cPassword.length() << cPassword << "\n";
		break;
		
		case SERVER:
			packet << (char)1 << "GSRVRC1" << (char)0 << (char)cAccount.length() << cAccount << (char)cPassword.length() << cPassword << "\n";
		break;
	}
	
	sendPacket(packet);
	return connected;
}

bool CRCSock::main()
{
	if (!connected)
		return true;
	
    char uncompressedData[9001];
	CBuffer receiveBuff;
    if (RCSock->receiveBytes(receiveBuff, 9000) < 0)
    {
        if (type == SERVER)
			return false;
    }
	
	if (type == IRC)
	{
		while (receiveBuff.length() > 0)
		{
			CBuffer line = receiveBuff.readString("\n");
			if (!line.length())
				break;
			parsePacket((CPacket&)line);
		}
	}
		else
	{
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
				return false;
			}
		}
		

		/*
		while (receiveBuff.length() > 0)
		{
			CBuffer line = receiveBuff.readString("\n");
			if (!line.length())
				break;
			parsePacket((CPacket&)line);
		}
		*/
	}
	
	return true;
}

void CRCSock::parsePacket(CPacket& pPacket)
{
	switch (type)
	{
		case SERVERLIST:	
			emit ListPacket(this, pPacket);
		break;
		
		case SERVER:
			emit LivePacket(this, pPacket);
		break;
		
		case IRC:
			emit IrcPacket(this, pPacket);
		break;
	}
}

/*
			    CPacket otherProps;
				otherProps << (char)SADDPLAYER << (short)other->id;
				otherProps << (char)other->accountName.length() << other->accountName;
                otherProps << (char)CURLEVEL << other->getProp(CURLEVEL);
                otherProps << (char)NICKNAME << other->getProp(NICKNAME);
                otherProps << (char)HEADGIF << other->getProp(HEADGIF);
				otherProps << (char)BODYIMG << other->getProp(BODYIMG);
				sendPacket(otherProps);
*/

void CRCSock::sendPacket(CPacket& pPacket)
{
	if (type == IRC)
	{
		printf("Out Buffer: %s", pPacket.text());
		RCSock->sendBuffer(pPacket);
	}
		else
	{
		char buffer[65535];
		unsigned long clen = 65535;
		compress((unsigned char*)buffer,&clen,(unsigned char*)pPacket.text(), pPacket.length());
		CBuffer sendString;
		sendString.writeByte((char)((clen >> 8) & 0xFF));
		sendString.writeByte((char)(clen & 0xFF));
		sendString.writeBytes(buffer, clen);
		RCSock->sendBuffer(sendString);
	}
}