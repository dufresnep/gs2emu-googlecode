#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "TServer.h"
#include "CLog.h"
#include "FProfile.h"
#include "CFileSystem.h"

#ifndef NO_MYSQL
	extern CMySQL *mySQL;
#endif
extern CSettings *settings;
extern std::vector<TServer *> serverList;
extern CLog serverlog;
extern CFileSystem filesystem;

/*
	Pointer-Functions for Packets
*/
std::vector<TSVSock> svfunc;

void createSVFunctions()
{
	// kinda like a memset-ish thing y'know
	for (int i = 0; i < 20; i++)
		svfunc.push_back(&TServer::msgSVI_NULL);

	// now set non-nulls
	svfunc[SVI_SETNAME] = &TServer::msgSVI_SETNAME;
	svfunc[SVI_SETDESC] = &TServer::msgSVI_SETDESC;
	svfunc[SVI_SETLANG] = &TServer::msgSVI_SETLANG;
	svfunc[SVI_SETVERS] = &TServer::msgSVI_SETVERS;
	svfunc[SVI_SETURL]  = &TServer::msgSVI_SETURL;
	svfunc[SVI_SETIP]   = &TServer::msgSVI_SETIP;
	svfunc[SVI_SETPORT] = &TServer::msgSVI_SETPORT;
	svfunc[SVI_SETPLYR] = &TServer::msgSVI_SETPLYR;
	svfunc[SVI_VERIACC] = &TServer::msgSVI_VERIACC;
	svfunc[SVI_VERIGLD] = &TServer::msgSVI_VERIGLD;
	svfunc[SVI_GETFILE] = &TServer::msgSVI_GETFILE;
	svfunc[SVI_NICKNAME] = &TServer::msgSVI_NICKNAME;
	svfunc[SVI_GETPROF] = &TServer::msgSVI_GETPROF;
	svfunc[SVI_SETPROF] = &TServer::msgSVI_SETPROF;
	svfunc[SVI_PLYRADD] = &TServer::msgSVI_PLYRADD;
	svfunc[SVI_PLYRREM] = &TServer::msgSVI_PLYRREM;
	svfunc[SVI_SVRPING] = &TServer::msgSVI_SVRPING;
	svfunc[SVI_VERIACC2] = &TServer::msgSVI_VERIACC2;
}

/*
	Constructor - Deconstructor
*/
TServer::TServer(CSocket *pSocket)
: sock( pSocket ), type( 0 )
{
	language = "English";
	lastPing = time(0);
}

TServer::~TServer()
{
	// clean playerlist
	for (unsigned int i = 0; i < playerList.size(); i++)
		delete playerList[i];
	playerList.clear();

	// Delete server from SQL serverlist.
#ifndef NO_MYSQL
	CString query;
	query << "DELETE FROM " << settings->getStr("serverlist") << " WHERE name='" << name.escape() << "'";
	mySQL->query( query );
#endif

	// delete socket
	delete sock;
}

/*
	Loops
*/
bool TServer::doMain()
{
	// sock exist?
	if (sock == NULL)
		return false;

	// definitions
	CString line, unBuffer;
	int lineEnd;//, size;

	// receive
	if ( sock->getData() == -1 )
		return false;

	// append new data
	sockBuffer.write( sock->getBuffer().text(), sock->getBuffer().length() );
	sock->getBuffer().clear();

	if (!sockBuffer.length())
		return true;

	// parse data
	if ((lineEnd = sockBuffer.findl('\n')) == -1)
		return true;

	line = sockBuffer.subString(0, lineEnd + 1);
	sockBuffer.removeI(0, line.length());

	std::vector<CString> lines = line.tokenize("\n");
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		if (!parsePacket(lines[i]))
			return false;

		// Update the data timeout.
		lastData = time(0);
	}

	// Send a ping every 30 seconds.
	if ( (int)difftime( time(0), lastPing ) >= 30 )
	{
		lastPing = time(0);
		sendPacket( CString() >> (char)SVO_PING );
	}

	// send out buffer
	sendCompress();
	return true;
}

/*
	Kill Client
*/
void TServer::kill()
{
	// Send Out-Buffer
	sendCompress();

	// Delete
	serverList.erase(std::find(serverList.begin(), serverList.end(), this));
	delete this;
}

/*
	Get-Value Functions
*/
const CString& TServer::getDescription()
{
	return description;
}

const CString& TServer::getIp()
{
	return ip;
}

const CString& TServer::getLanguage()
{
	return language;
}

const CString& TServer::getName()
{
	return name;
}

const CString& TServer::getPCount()
{
	return (pcount = playerList.size());
}

const CString& TServer::getPort()
{
	return port;
}

const CString TServer::getType()
{
	CString ret;
	switch ( type )
	{
		case TYPE_GOLD:
			ret = "P ";
			break;
		case TYPE_HOSTED:
			ret = "H ";
			break;
		case TYPE_HIDDEN:
			ret = "U ";
			break;
	}
	return ret;
}

const CString& TServer::getUrl()
{
	return url;
}

const CString& TServer::getVersion()
{
	return version;
}

const CString TServer::getServerPacket()
{
	return CString() >> (char)8 >> (char)(getName().length() + getType().length()) << getType() << getName() >> (char)getLanguage().length() << getLanguage() >> (char)getDescription().length() << getDescription() >> (char)getUrl().length() << getUrl() >> (char)getVersion().length() << getVersion() >> (char)getPCount().length() << getPCount() >> (char)getIp().length() << getIp() >> (char)getPort().length() << getPort();
}

/*
	Send-Packet Functions
*/
void TServer::sendCompress()
{
	// empty buffer?
	if (sendBuffer.isEmpty())
	{
		// If we still have some data in the out buffer, try sending it again.
		if (outBuffer.isEmpty() == false)
			sock->sendData(outBuffer);
		return;
	}

	// add the send buffer to the out buffer
	outBuffer << sendBuffer;

	// send buffer
	sock->sendData(outBuffer);

	// clear buffer
	sendBuffer.clear();
}

void TServer::sendPacket(CString pPacket)
{
	// empty buffer?
	if (pPacket.isEmpty())
		return;

	// append '\n'
	if (pPacket[pPacket.length()-1] != '\n')
		pPacket.writeChar('\n');

	// append buffer
	sendBuffer.write(pPacket);
}

/*
	Packet-Functions
*/
bool TServer::parsePacket(CString& pPacket)
{
	// read id & packet
	unsigned char id = pPacket.readGUChar();

	// id exists?
	if (id >= (unsigned char)svfunc.size())
		return true;

	// valid packet, call function
	return (*this.*svfunc[id])(pPacket);
}

bool TServer::msgSVI_NULL(CString& pPacket)
{
	pPacket.setRead(0);
	serverlog.out("Unknown Server Packet: %i (%s)\n", pPacket.readGUChar(), pPacket.text()+1);
	return true;
}

bool TServer::msgSVI_SETNAME(CString& pPacket)
{
	name = pPacket.readString("");

	// make sure they don't have a Gold/Hosted type
	while (name.subString(0, 2) == "P " || name.subString(0, 2) == "H " || name.subString(0, 2) == "3 ")
		name.removeI(0, 2);

	// check for underconstruction
	if (name.subString(0, 2) == "U ")
	{
		name.removeI(0, 2);
		type = TYPE_HIDDEN;
	}

	// check for duplicates
	for (unsigned int i = 0; i < serverList.size(); i++)
	{
		if (serverList[i]->getName() == name && serverList[i] != this)
		{
			serverList[i]->sendPacket(CString() >> (char)SVO_ERRMSG << "Servername is already in use!");
			return false;
		}
	}

	serverlog.out(CString() << "New Server: " << name << "\n");
	return true;
}

bool TServer::msgSVI_SETDESC(CString& pPacket)
{
	description = pPacket.readString("");
	return true;
}

bool TServer::msgSVI_SETLANG(CString& pPacket)
{
	language = pPacket.readString("");
	return true;
}

bool TServer::msgSVI_SETVERS(CString& pPacket)
{
	CString ver(pPacket.readString(""));
	int verNum = atoi(ver.text());
	if (verNum == 0)
		version = CString("Custom build: ") << ver;
	else if (verNum <= 52)
		version = CString("Revision ") << CString(abs(verNum));
	else
		version = CString("Build ") << verNum;
	return true;
}

bool TServer::msgSVI_SETURL(CString& pPacket)
{
	url = pPacket.readString("");
	return true;
}

bool TServer::msgSVI_SETIP(CString& pPacket)
{
	ip = pPacket.readString("");
	ip = (ip == "AUTO" ? sock->tcpIp() : ip);
	return true;
}

bool TServer::msgSVI_SETPORT(CString& pPacket)
{
	port = pPacket.readString("");

	// This should be the last packet sent when the server is initialized.
	// Add it to the database now.
#ifndef NO_MYSQL
	if ( name.length() > 0 )
	{
		CString query;
		query << "INSERT INTO " << settings->getStr("serverlist") << " ";
		query << "(name, ip, port, playercount, description, url, language, type, version) ";
		query << "VALUES ('"
			<< name.escape() << "','"
			<< ip.escape() << "','"
			<< port.escape() << "','"
			<< CString((int)playerList.size()) << "','"
			<< description.escape() << "','"
			<< url.escape() << "','"
			<< language.escape() << "','"
			<< getType().escape() << "','"
			<< version.escape() << "'"
			<< ")";
		mySQL->query(query);
	}
#endif

	return true;
}

bool TServer::msgSVI_SETPLYR(CString& pPacket)
{
	// definitions
	unsigned int count = 0;

	// clear list
	for (unsigned int i = 0; i < playerList.size(); i++)
		delete playerList[i];
	playerList.clear();

	// grab new playercount
	count = pPacket.readGUChar();

	// remake list
	for (unsigned int i = 0; i < count; i++)
	{
		player *pl = new player();
			pl->account = pPacket.readChars(pPacket.readGUChar());
			pl->nick = pPacket.readChars(pPacket.readGUChar());
			pl->level = pPacket.readChars(pPacket.readGUChar());
			pl->x = (float)pPacket.readGChar() / 2;
			pl->y = (float)pPacket.readGChar() / 2;
			pl->ap = pPacket.readGChar();
			pl->type = pPacket.readGChar();
		playerList.push_back(pl);
	}

	return true;
}

bool TServer::msgSVI_VERIACC(CString& pPacket)
{
	// definitions
	CString account = pPacket.readChars(pPacket.readGUChar());
	CString password = pPacket.readChars(pPacket.readGUChar());

	// send verification
	sendPacket(CString() >> (char)SVO_VERIACC >> (char)account.length() << account << getAccountError(verifyAccount(account, password)));
	return true;
}

bool TServer::msgSVI_VERIGLD(CString& pPacket)
{
	unsigned short playerid = pPacket.readGUShort();
	CString account = pPacket.readChars(pPacket.readGUChar());
	CString nickname = pPacket.readChars(pPacket.readGUChar());
	CString guild = pPacket.readChars(pPacket.readGUChar());

	if (verifyGuild(account, nickname, guild))
	{
		nickname << " (" << guild << ")";
		sendPacket(CString() >> (char)SVO_VERIGLD >> (short)playerid >> (char)nickname.length() << nickname);
	}

	return true;
}

bool TServer::msgSVI_GETFILE(CString& pPacket)
{
	unsigned short pId = pPacket.readGUShort();
	unsigned char pTy = pPacket.readGUChar();
	CString shortName = pPacket.readChars(pPacket.readGUChar());
	CString fileData = filesystem.load(shortName);

	if (fileData.length() != 0)
	{
		sendPacket(CString() >> (char)SVO_FILESTART >> (char)shortName.length() << shortName << "\n");
		fileData = fileData.B64_Encode();

		while (fileData.length() > 0)
		{
			CString temp = fileData.subString(0, (fileData.length() > 0x10000 ? 0x10000 : fileData.length()));
			fileData.removeI(0, temp.length());
			sendPacket(CString() >> (char)SVO_FILEDATA >> (char)shortName.length() << shortName << temp << "\n");
		}

		sendPacket(CString() >> (char)SVO_FILEEND >> (char)shortName.length() << shortName >> (short)pId >> (char)pTy << "\n");
	}

	return true;
}

bool TServer::msgSVI_NICKNAME(CString& pPacket)
{
	CString accountname = pPacket.readChars( pPacket.readGUChar() );
	CString nickname = pPacket.readChars( pPacket.readGUChar() );

	// Find the player and adjust his nickname.
	for ( unsigned int i = 0; i < playerList.size(); ++i )
	{
		player* pl = playerList[i];
		if ( pl->account == accountname )
			pl->nick = nickname;
	}

	return true;
}

bool TServer::msgSVI_GETPROF(CString& pPacket)
{
	unsigned short playerid = pPacket.readGUShort();
	CString accountName = pPacket.readChars( pPacket.readGUChar() );

	CString replyPacket;
	if ( getProfile( accountName, replyPacket ) )
		sendPacket(CString() >> (char)SVO_PROFILE >> (short)playerid >> (char)accountName.length() << accountName << replyPacket);
	return true;
}

bool TServer::msgSVI_SETPROF(CString& pPacket)
{
	pPacket.readGUChar();		// Throwaway data.
	CString accountName = pPacket.readChars( pPacket.readGUChar() );

	// Set profile.
	for (unsigned int i = 0; i < playerList.size(); i++)
	{
		player* pl = playerList[i];
		if ( pl->account == accountName )
		{
			setProfile(accountName, pPacket);
			break;
		}
	}
	return true;
}

bool TServer::msgSVI_PLYRADD(CString& pPacket)
{
	player *pl = new player();
		pl->account = pPacket.readChars(pPacket.readGUChar());
		pl->nick = pPacket.readChars(pPacket.readGUChar());
		pl->level = pPacket.readChars(pPacket.readGUChar());
		pl->x = (float)pPacket.readGChar() / 2;
		pl->y = (float)pPacket.readGChar() / 2;
		pl->ap = pPacket.readGChar();
		pl->type = pPacket.readGChar();
	playerList.push_back(pl);
	return true;
}

bool TServer::msgSVI_PLYRREM(CString& pPacket)
{
	if ( playerList.size() == 0 )
		return true;

	unsigned char type = pPacket.readGUChar();
	CString accountname = pPacket.readString("");

	// Find the player and remove him.
	std::vector<player*>::iterator iter;
	for ( iter = playerList.begin(); iter != playerList.end(); )
	{
		player* pl = (player*)*iter;
		if ( pl->account == accountname && pl->type == type )
		{
			delete pl;
			iter = playerList.erase( iter );
		}
		else
			++iter;
	}

	return true;
}

bool TServer::msgSVI_SVRPING(CString& pPacket)
{
	// Do nothing.  It is just a ping.  --  1 per minute.
	return true;
}

// Secure login password:
//	{transaction}{CHAR \xa7}{password}
bool TServer::msgSVI_VERIACC2(CString& pPacket)
{
#ifndef NO_MYSQL
	// definitions
	CString account = pPacket.readChars(pPacket.readGUChar());
	CString password = pPacket.readChars(pPacket.readGUChar());
	unsigned short id = pPacket.readGUShort();
	unsigned char type = pPacket.readGUChar();

	// Verify the account.
	int ret = verifyAccount(account, password);

	// send verification
	sendPacket(CString() >> (char)SVO_VERIACC2
		>> (char)account.length() << account
		>> (short)id >> (char)type
		<< getAccountError(ret));
#endif

	return true;
}
