#include "main.h"
#include "TPlayer.h"

/*
	External
*/
extern std::vector<TPlayer *> playerIds, playerList;

/*
	Global Definitions
*/
// Enum per Attr
int __attrPackets[] = { 37, 38, 39, 40, 41, 46, 47, 48, 49, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74 };

// Sent on Login
bool __sendLogin[] =
{
	false, true,  true,  true,  true,  true,  // 0-5
	true , true,  true,  true,  true,  true,  // 6-11
	false, true,  false, true,  true,  true,  // 12-17
	true , true,  true,  true,  true,  false, // 18-23
	false, true,  true,  true,  false, false, // 24-29
	false, false, true,  false, false, true,  // 30-35
	true,  true,  true,  true,  true,  true,  // 36-41
	false, false, false, false, false, false, // 42-47
	false, false, false, false, false, false, // 48-53
	true,  true,  true,  true,  true,  true,  // 54-59
	true,  true,  true,  true,  true,  true,  // 60-65
	true,  true,  true,  true,  true,  true,  // 66-71
	true,  true,  true,  false, false, false, // 72-77
};

/*
	Pointer-Functions for Packets
*/
std::vector<TPLSock> TPLFunc;

void createPLFunctions()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 200; i++)
		TPLFunc.push_back(&TPlayer::msgPLI_NULL);

	// now set non-nulls
	TPLFunc[PLI_LANGUAGE] = &TPlayer::msgPLI_LANGUAGE;
	TPLFunc[PLI_TRIGGER] = &TPlayer::msgPLI_TRIGGER;
}

/*
	Constructor - Deconstructor
*/
TPlayer::TPlayer(CSocket *pSocket) : TAccount()
{
	// Copy Socket-Pointer
	playerSock = pSocket;

	// Encryption Keys
	iterator = 0x04A80B38;
	key = 0;
	type = CLIENTTYPE_AWAIT;

	printf("Created for: %s\n", playerSock->tcpIp());
}

TPlayer::~TPlayer()
{
	if (id >= 0)
	{
		playerIds[id] = NULL;
		vecRemove(playerList, this);
		saveAccount();
	}

	printf("Destroyed for: %s\n", playerSock->tcpIp());
	delete playerSock;
}

/*
	TPlayer: Get Properties
*/
TLevel* TPlayer::getLevel()
{
	return level;
}

/*
	TPlayer: Set Properties
*/
void TPlayer::setLevel(const CString& pLevelName)
{
	// Open Level
	level = TLevel::findLevel(pLevelName);
	if (level == NULL)
		return;

	// Send Level
	sendPacket(CString() >> (char)PLO_PLAYERWARP << getProp(PLPROP_X) << getProp(PLPROP_Y) << level->getLevelName());
	sendPacket(CString() >> (char)PLO_LEVELNAME << level->getLevelName());
	sendPacket(CString() >> (char)100 >> (int)(64*64*2)+2);
	sendPacket(CString() << level->getBoardPacket());
	sendCompress();
}

void TPlayer::setNick(const CString& pNickName)
{
	nickName = pNickName;
}

/*
	TPlayer: Manage Account
*/
void TPlayer::sendLogin()
{
	// Load Player-Account
	loadAccount(accountName); // We don't need to check if this fails.. because the defaults have already been loaded :)

	// Send Player-Props
	sendProps(__sendLogin, sizeof(__sendLogin) / sizeof(bool));

	// Server Signature
	sendPacket(CString() >> (char)PLO_SIGNATURE >> (char)73);

	// Send Level
	setLevel("world/onlinestartlocal.nw");
}

/*
	TPlayer: Prop-Manipulation
*/
CString TPlayer::getProp(int pPropId)
{
	switch (pPropId)
	{
		case PLPROP_NICKNAME:
		return CString() >> (char)nickName.length() << nickName;

		case PLPROP_MAXPOWER:
		return CString() >> (char)maxPower;

		case PLPROP_CURPOWER:
		return CString() >> (char)(power * 2);

		case PLPROP_RUPEESCOUNT:
		return CString().writeGInt3(gralatc+1);

		case PLPROP_ARROWSCOUNT:
		return CString() >> (char)arrowc;

		case PLPROP_BOMBSCOUNT:
		return CString() >> (char)bombc;

		case PLPROP_GLOVEPOWER:
		return CString() >> (char)glovePower;

		case PLPROP_BOMBPOWER:
		return CString() >> (char)bombPower;

		case PLPROP_SWORDPOWER:
		return CString() >> (char)(swordPower+30) >> (char)swordImg.length() << swordImg;

		case PLPROP_SHIELDPOWER:
		return CString() >> (char)(shieldPower+10) >> (char)shieldImg.length() << shieldImg;

		case PLPROP_GANI:
		return CString() >> (char)gAni.length() << gAni;

		case PLPROP_HEADGIF:
		return CString() >> (char)(headImg.length()+100) << headImg;

		case PLPROP_CURCHAT:
		return CString() >> (char)chatMsg.length() << chatMsg;

		case PLPROP_COLORS:
		return CString() >> (char)colors[0] >> (char)colors[1] >> (char)colors[2] >> (char)colors[3] >> (char)colors[4];

		case PLPROP_ID:
		return CString() >> (short)id;

		case PLPROP_X:
		return CString() >> (char)(x * 2);

		case PLPROP_Y:
		return CString() >> (char)(y * 2);

		case PLPROP_Z:
		return CString() >> (char)(z * 2);

		case PLPROP_SPRITE:
		return CString() >> (char)sprite;

		case PLPROP_STATUS:
		return CString() >> (char)status;

		case PLPROP_CARRYSPRITE:
		return CString() >> (char)carrySprite;

		case PLPROP_CURLEVEL:
		return CString() >> (char)levelName.length() << levelName;

		case PLPROP_HORSEGIF:
		return CString() >> (char)horseImg.length() << horseImg;

		case PLPROP_HORSEBUSHES:
		return CString() >> (char)horsec;

		case PLPROP_EFFECTCOLORS:
		return CString() >> (char)0;

		case PLPROP_CARRYNPC:
		return CString().writeGInt3(0);

		case PLPROP_APCOUNTER:
		return CString() >> (short)(apCounter+1);

		case PLPROP_MAGICPOINTS:
		return CString() >> (char)mp;

		case PLPROP_KILLSCOUNT:
		return CString().writeGInt3(kills);

		case PLPROP_DEATHSCOUNT:
		return CString().writeGInt3(deaths);

		case PLPROP_ONLINESECS:
		return CString().writeGInt3(onlineTime);

		case PLPROP_LASTIP:
		return CString().writeGInt5(accountIp);

		case PLPROP_UDPPORT:
		return CString().writeGInt3(udpport);

		case PLPROP_ALIGNMENT:
		return CString() >> (char)ap;

		case PLPROP_ADDITFLAGS:
		return CString() >> (char)additionalFlags;

		case PLPROP_ACCOUNTNAME:
		return CString() >> (char)accountName.length() << accountName;

		case PLPROP_BODYIMG:
		return CString() >> (char)bodyImg.length() << bodyImg;

		case PLPROP_RATING:
		return CString().writeGInt3(-500); // 3

		case PLPROP_PCONNECTED:
		return CString();

		case PLPROP_PLANGUAGE:
		return CString() >> (char)language.length() << language;

		case PLPROP_PSTATUSMSG:
		return CString() >> (char)0; // 1
	}

	if (inrange(pPropId, 37, 41) || inrange(pPropId, 46, 49) || inrange(pPropId, 54, 74))
	{
		for (unsigned int i = 0; i < sizeof(__attrPackets) / sizeof(int); i++)
		{
			if (__attrPackets[i] == pPropId)
				return CString() >> (char)attrList[i].length() << attrList[i];
		}
	}

	return CString();
}

void TPlayer::setProps(CString& pPacket, bool pForward)
{
	CString globalBuff, levelBuff;
	int len = 0;

	while (pPacket.bytesLeft() > 0)
	{
		int propId = pPacket.readGUChar();
		printf("ID: %i\n", propId);
		switch (propId)
		{
			case PLPROP_NICKNAME:
				len = clip(pPacket.readGUChar(), 0, 250);
				setNick(pPacket.readChars(len));
				levelBuff >> (char)propId << getProp(propId);
			break;

			case PLPROP_MAXPOWER:
				maxPower = clip(pPacket.readGUChar(), 0, 20);
			break;

			case PLPROP_CURPOWER:
				power = clip(pPacket.readGUChar() / 2, 0, maxPower);
			break;

			case PLPROP_RUPEESCOUNT:
				gralatc = clip(pPacket.readGUInt3(), 0, 9999999);
			break;

			case PLPROP_ARROWSCOUNT:
				arrowc = clip(pPacket.readGUChar(), 0, 99);
			break;

			case PLPROP_BOMBSCOUNT:
				bombc = clip(pPacket.readGUChar(), 0, 99);
			break;

			case PLPROP_GLOVEPOWER:
				glovePower = clip(pPacket.readGUChar(), 0, 3);
			break;

			case PLPROP_BOMBPOWER:
				bombPower = clip(pPacket.readGUChar(), 0, 3);
			break;

			case PLPROP_SWORDPOWER:

			break;

			case PLPROP_SHIELDPOWER:

			break;

			case PLPROP_GANI:
				len = pPacket.readGUChar();
				if (len < 0)
					len = 0;

				gAni = pPacket.readChars(len);
			break;

			case PLPROP_HEADGIF:

			break;

			case PLPROP_CURCHAT:
				len = clip(pPacket.readGUChar(), 0, 220);
				chatMsg = pPacket.readChars(len);
				//processChat(chatMsg);
			break;

			case PLPROP_COLORS:
				for (unsigned int i = 0; i < sizeof(colors) / sizeof(char); ++i)
					colors[i] = pPacket.readGUChar();
			break;

			case PLPROP_ID:
				pPacket.readGUShort();
			break;

			case PLPROP_X:
				x = (float)(pPacket.readGUChar() / 2);
				status &= (-1-1);
			break;

			case PLPROP_Y:
				y = (float)(pPacket.readGUChar() / 2);
				status &= (-1-1);
			break;

			case PLPROP_Z:
				z = (float)(pPacket.readGUChar() / 2);
				status &= (-1-1);
			break;

			case PLPROP_SPRITE:
				sprite = pPacket.readGUChar();
			break;

			case PLPROP_STATUS:

			break;

			case PLPROP_CARRYSPRITE:
				carrySprite = pPacket.readGUChar();
			break;

			case PLPROP_CURLEVEL:
				len = pPacket.readGUChar();
				if (len >= 0)
					levelName = pPacket.readChars(len);
			break;

			case PLPROP_HORSEGIF:

			break;

			case PLPROP_HORSEBUSHES:
				horsec = pPacket.readGUChar();
			break;

			case PLPROP_EFFECTCOLORS:
				len = pPacket.readGUChar();
				if (len > 0)
					pPacket.readGInt();
			break;

			case PLPROP_CARRYNPC:
				pPacket.readGInt3();
			break;

			case PLPROP_APCOUNTER:
				apCounter = pPacket.readGUShort();
			break;

			case PLPROP_MAGICPOINTS:
				mp = clip(pPacket.readGUChar(), 0, 100);
			break;

			case PLPROP_KILLSCOUNT:
				pPacket.readGInt3();
			break;

			case PLPROP_DEATHSCOUNT:
				pPacket.readGInt3();
			break;

			case PLPROP_ONLINESECS:
				pPacket.readGInt3();
			break;

			case PLPROP_LASTIP:
				pPacket.readGInt5();
			break;

			case PLPROP_UDPPORT:
				udpport = pPacket.readGInt3();
			break;

			case PLPROP_ALIGNMENT:
				ap = clip(pPacket.readGUChar(), 0, 100);
			break;

			case PLPROP_ADDITFLAGS:
				additionalFlags = pPacket.readGUChar();
			break;

			case PLPROP_ACCOUNTNAME:
				len = pPacket.readGUChar();
				if (len >= 0)
					pPacket.readChars(len);
			break;

			case PLPROP_BODYIMG:
				len = pPacket.readGUChar();
				if (len >= 0)
					bodyImg = pPacket.readChars(len);
			break;

			case PLPROP_RATING:
				pPacket.readGInt3();
			break;

			case PLPROP_PEMPTY42:
				break;

			case PLPROP_PEMPTY43:
				break;

			case PLPROP_PEMPTY44:
				break;

			case PLPROP_PEMPTY50:
				break;

			case PLPROP_PCONNECTED:
				break;

			case PLPROP_PLANGUAGE:
				len = pPacket.readGUChar();
				if (len >= 0)
					language = pPacket.readChars(len);
			break;

			case PLPROP_PSTATUSMSG:
				statusMsg = pPacket.readGUChar();
				if (id == -1)
					break;

				sendPacketToAll(CString() >> (char)PLO_OTHERPLPROPS >> (short)id >> (char)PLPROP_PSTATUSMSG >> (char)statusMsg, this);
			break;

			case PLPROP_GATTRIB1:  attrList[0]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB2:  attrList[1]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB3:  attrList[2]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB4:  attrList[3]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB5:  attrList[4]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB6:  attrList[5]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB7:  attrList[6]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB8:  attrList[7]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB9:  attrList[8]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB10: attrList[9]  = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB11: attrList[10] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB12: attrList[11] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB13: attrList[12] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB14: attrList[13] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB15: attrList[14] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB16: attrList[15] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB17: attrList[16] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB18: attrList[17] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB19: attrList[18] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB20: attrList[19] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB21: attrList[20] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB22: attrList[21] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB23: attrList[22] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB24: attrList[23] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB25: attrList[24] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB26: attrList[25] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB27: attrList[26] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB28: attrList[27] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB29: attrList[28] = pPacket.readChars(pPacket.readGUChar()); break;
			case PLPROP_GATTRIB30: attrList[29] = pPacket.readChars(pPacket.readGUChar()); break;

			default:
				printf("Unidentified Packet: %i\n", propId);
			return;
		}

		if (pForward) // forwardLocal[index] == true
			globalBuff >> (char)propId << getProp(propId);
	}

	// Send Buffers Out
	if (isLoggedIn())
	{
		if (globalBuff.length() > 0)
			sendPacketToAll(globalBuff, this);
		if (levelBuff.length() > 0)
			sendPacketToLevel(levelBuff, getLevel());
	}
}

void TPlayer::sendProps(bool *pProps, int pCount)
{
	// Definition
	CString propPacket;

	// Create Props
	for (int i = 0; i < pCount; ++i)
	{
		if (pProps[i])
			propPacket >> (char)i << getProp(i);
	}

	// Send Packet
	sendPacket(CString() >> (char)PLO_PLAYERPROPS << propPacket);
}

/*
	Socket-Control Functions
*/
bool TPlayer::doMain()
{
	if (playerSock == NULL)
		return false;

	// definitions
	CString recvBuffer, unBuffer;
	int size;

	// receive
	if ((size = playerSock->receiveBytes(recvBuffer, 65535)) < 0)
		return false;

	// data recieved? parse it
	if (recvBuffer.length() > 0)
	{
		// append new data
		rBuffer.write(recvBuffer);

		// parse data
		while (rBuffer.length() >= 2)
		{
			// packet length
			unsigned int len = rBuffer.readShort();
			if (len > (unsigned int)rBuffer.length()-2)
				break;

			// uncompress
			unBuffer = rBuffer.subString(2, len).zuncompress();
			if (unBuffer.length() < 1)
				break;

			// remove read-data
			rBuffer.removeI(0, len+2);

			// well theres your buffer
			if (!parsePacket(unBuffer))
				return false;
		}
	}

	// send out buffer
	sendCompress();
	return true;
}

void TPlayer::decryptPacket(CString& pPacket)
{
	if (type != CLIENTTYPE_CLIENT)
		return;

	// Decrypt
	iterator *= 0x8088405;
	iterator += key;
	int pos = ((iterator & 0x0FFFF) % pPacket.length());
	pPacket.removeI(pos, 1);
}

void TPlayer::sendCompress()
{
	// empty buffer?
	if (sBuffer.isEmpty())
		return;

	// compress buffer
	sBuffer.zcompressI();

	// send buffer
	playerSock->sendBuffer(CString() << (short)sBuffer.length() << sBuffer);

	// clear buffer
	sBuffer.clear();
}

void TPlayer::sendPacket(CString& pPacket)
{
	// empty buffer?
	if (pPacket.isEmpty())
		return;

	// append '\n'
	if (pPacket[pPacket.length()-1] != '\n')
		pPacket.writeChar('\n');

	// append buffer
	sBuffer.write(pPacket);

	// Send Data
	if (sBuffer.length() > 4096)
		sendCompress();
}

/*
	Packet-Functions
*/
bool TPlayer::parsePacket(CString& pPacket)
{
	// first packet.. maybe
	if (type == CLIENTTYPE_AWAIT)
		msgPLI_LOGIN(CString() << pPacket.readString("\n"));

	while (pPacket.bytesLeft() > 0)
	{
		// grab packet
		CString curPacket = pPacket.readString("\n");

		// decrypt packet
		decryptPacket(curPacket);

		// read id & packet
		int id = curPacket.readGUChar();

		// check lengths
		if (id >= (unsigned char)TPLFunc.size())
			return false;

		// valid packet, call function
		if (!(*this.*TPLFunc[id])(curPacket))
			return false;
	}

	return true;
}

bool TPlayer::msgPLI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	printf("Unknown Serverlist Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
	return true;
}

bool TPlayer::msgPLI_LOGIN(CString& pPacket)
{
	printf("a\n");
	// Read Player-Ip
	accountIp = inet_addr(playerSock->tcpIp());

	// Read Client-Type
	type = pPacket.readGChar();

	// Get Iterator-Key
	if (type == CLIENTTYPE_CLIENT)
		key = pPacket.readGChar();

	// Read Client-Version

	graalVersion = pPacket.readChars(7);

	// Skip a Byte
	pPacket.readGChar();

	// Read Account & Password
	accountName = pPacket.readChars(pPacket.readGUChar());
	CString password = pPacket.readChars(pPacket.readGUChar());

	// Process Login
	sendLogin();
	return true;
}

bool TPlayer::msgPLI_LANGUAGE(CString& pPacket)
{
	language = pPacket.readChars(pPacket.readGUChar());
	return true;
}

bool TPlayer::msgPLI_TRIGGER(CString& pPacket)
{
	return true;
}
