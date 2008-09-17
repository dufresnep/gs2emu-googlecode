#ifndef MAIN_H
#define MAIN_H

/*
	Header-Includes
*/
#include "LS2CompileConfig.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
#endif

/*
	Custom-Class Includes
*/
#ifndef NO_MYSQL
	#include "CMySQL.h"
#endif
#include "CSettings.h"
#include "CSocket.h"
#include "CString.h"

/*
	Defiine Functions
*/
#if defined(_WIN32)
	#define wait(a) Sleep(a)
#else
	#define wait(a) usleep(a*1000)
#endif

CString getAccountError(int pErrorId);
CString getServerList();
int getPlayerCount();
int getServerCount();
int verifyAccount(const CString& pAccount, const CString& pPassword);
int verifyGuild(const CString& pAccount, const CString& pNickname, const CString& pGuild);
void acceptSock(CSocket& pSocket, int pType);
void shutdownServer( int signal );
int main(int argc, char *argv[]);

/*
	Definitions
*/
enum // Return-Errors
{
	ERR_SUCCESS = 0,
	ERR_SETTINGS = -1,
	ERR_SOCKETS = -2,
	ERR_MYSQL = -3,
	ERR_LISTEN = -4,
};

enum // Socket Type
{
	SOCK_PLAYER,
	SOCK_SERVER,
};

enum // Account Status
{
	ACCSTAT_NORMAL,
	ACCSTAT_NONREG,
	ACCSTAT_BANNED,
	ACCSTAT_INVALID,
};

enum // Guild Status
{
	GUILDSTAT_DISALLOWED,
	GUILDSTAT_ALLOWED,
};

#endif // MAIN_H
