#ifndef ICOMMON_H
#define ICOMMON_H
/*
#if defined(_WIN32) || defined(_WIN64)
	#ifndef WIN32_LEAN_AND_MEAN
		//#define WIN32_LEAN_AND_MEAN
	#endif

	#ifndef WINVER
		#define WINVER 0x0501
	#endif
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <windows.h>
	#include <stdarg.h>
    #include <stdio.h>
#endif
*/
#include "main.h"
#undef wait

enum
{
	NPCPROP_IMAGE			= 0,
	NPCPROP_SCRIPT			= 1,
	NPCPROP_X				= 2,
	NPCPROP_Y				= 3,
	NPCPROP_POWER			= 4,
	NPCPROP_RUPEES			= 5,
	NPCPROP_ARROWS			= 6,
	NPCPROP_BOMBS			= 7,
	NPCPROP_GLOVEPOWER		= 8,
	NPCPROP_BOMBPOWER		= 9,
	NPCPROP_SWORDIMAGE		= 10,
	NPCPROP_SHIELDIMAGE		= 11,
	NPCPROP_GANI			= 12,	// NPCPROP_BOWGIF in pre-2.x
	NPCPROP_VISFLAGS		= 13,
	NPCPROP_BLOCKFLAGS		= 14,
	NPCPROP_MESSAGE			= 15,
	NPCPROP_HURTDXDY		= 16,
	NPCPROP_ID				= 17,
	NPCPROP_SPRITE			= 18,
	NPCPROP_COLORS			= 19,
	NPCPROP_NICKNAME		= 20,
	NPCPROP_HORSEIMAGE		= 21,
	NPCPROP_HEADIMAGE		= 22,
	NPCPROP_SAVE0			= 23,
	NPCPROP_SAVE1			= 24,
	NPCPROP_SAVE2			= 25,
	NPCPROP_SAVE3			= 26,
	NPCPROP_SAVE4			= 27,
	NPCPROP_SAVE5			= 28,
	NPCPROP_SAVE6			= 29,
	NPCPROP_SAVE7			= 30,
	NPCPROP_SAVE8			= 31,
	NPCPROP_SAVE9			= 32,
	NPCPROP_ALIGNMENT		= 33,
	NPCPROP_IMAGEPART		= 34,
	NPCPROP_BODYIMAGE		= 35,
	NPCPROP_GATTRIB1		= 36,
	NPCPROP_GATTRIB2		= 37,
	NPCPROP_GATTRIB3		= 38,
	NPCPROP_GATTRIB4		= 39,
	NPCPROP_GATTRIB5		= 40,
	NPCPROP_GMAPLEVELX		= 41,
	NPCPROP_GMAPLEVELY		= 42,
	NPCPROP_EMPTY43			= 43,
	NPCPROP_GATTRIB6		= 44,
	NPCPROP_GATTRIB7		= 45,
	NPCPROP_GATTRIB8		= 46,
	NPCPROP_GATTRIB9		= 47,

// Does the client not send gani attribs > 9?
	NPCPROP_GATTRIB10		= 48,
	NPCPROP_GATTRIB11		= 49,
	NPCPROP_GATTRIB12		= 50,
	NPCPROP_GATTRIB13		= 51,
	NPCPROP_GATTRIB14		= 52,
	NPCPROP_GATTRIB15		= 53,
	NPCPROP_GATTRIB16		= 54,
	NPCPROP_GATTRIB17		= 55,
	NPCPROP_GATTRIB18		= 56,
	NPCPROP_GATTRIB19		= 57,
	NPCPROP_GATTRIB20		= 58,
	NPCPROP_GATTRIB21		= 59,
	NPCPROP_GATTRIB22		= 60,
	NPCPROP_GATTRIB23		= 61,
	NPCPROP_GATTRIB24		= 62,
	NPCPROP_GATTRIB25		= 63,
	NPCPROP_GATTRIB26		= 64,
	NPCPROP_GATTRIB27		= 65,
	NPCPROP_GATTRIB28		= 66,
	NPCPROP_GATTRIB29		= 67,
	NPCPROP_GATTRIB30		= 68,
	NPCPROP_CLASS			= 74,	// NPC-Server class.  Possibly also join scripts.
	NPCPROP_X2				= 75,
	NPCPROP_Y2				= 76,
	NPCLEVEL				= 77,
};


enum
{
	PROP_UNKOWN = 0,
	PROP_INT ,
	PROP_FLOAT,
	PROP_STRING,
	//Only for returning values
	PROP_CHAR,
};

enum
{
	PLI_LEVELWARP		= 0,
	PLI_BOARDMODIFY		= 1,
	PLI_PLAYERPROPS		= 2,
	PLI_NPCPROPS		= 3,
	PLI_BOMBADD			= 4,
	PLI_BOMBDEL			= 5,
	PLI_TOALL			= 6,
	PLI_HORSEADD		= 7,
	PLI_HORSEDEL		= 8,
	PLI_ARROWADD		= 9,
	PLI_FIRESPY			= 10,
	PLI_THROWCARRIED	= 11,
	PLI_ITEMADD			= 12,
	PLI_ITEMDEL			= 13,
	PLI_CLAIMPKER		= 14,
	PLI_BADDYPROPS		= 15,
	PLI_BADDYHURT		= 16,
	PLI_BADDYADD		= 17,
	PLI_FLAGSET			= 18,
	PLI_FLAGDEL			= 19,
	PLI_OPENCHEST		= 20,
	PLI_PUTNPC			= 21,
	PLI_NPCDEL			= 22,
	PLI_WANTFILE		= 23,
	PLI_SHOWIMG			= 24,
	PLI_EMPTY25			= 25,
	PLI_HURTPLAYER		= 26,
	PLI_EXPLOSION		= 27,
	PLI_PRIVATEMESSAGE	= 28,
	PLI_NPCWEAPONDEL	= 29,
	PLI_LEVELWARPMOD	= 30,
	PLI_PACKETCOUNT		= 31,
	PLI_ITEMTAKE		= 32,
	PLI_WEAPONADD		= 33,
	PLI_UPDATEFILE		= 34,
	PLI_ADJACENTLEVEL	= 35,
	PLI_HITOBJECTS		= 36,
	PLI_LANGUAGE		= 37,
	PLI_TRIGGERACTION	= 38,
	PLI_MAPINFO			= 39,
	PLI_SHOOT			= 40,
	PLI_SERVERWARP		= 41,
	PLI_PROCESSLIST		= 44,
	PLI_UNKNOWN46		= 46,	// Always is 1.  Might be a player count for the gmap level.
	PLI_UNKNOWN47		= 47,	// Seems to tell the server the modTime of update files.  Used for client updates.
	PLI_RAWDATA			= 50,
	PLI_RC_SERVEROPTIONSGET		= 51,
	PLI_RC_SERVEROPTIONSSET		= 52,
	PLI_RC_FOLDERCONFIGGET		= 53,
	PLI_RC_FOLDERCONFIGSET		= 54,
	PLI_RC_RESPAWNSET			= 55,
	PLI_RC_HORSELIFESET			= 56,
	PLI_RC_APINCREMENTSET		= 57,
	PLI_RC_BADDYRESPAWNSET		= 58,
	PLI_RC_PLAYERPROPSGET		= 59,
	PLI_RC_PLAYERPROPSSET		= 60,
	PLI_RC_DISCONNECTPLAYER		= 61,
	PLI_RC_UPDATELEVELS			= 62,
	PLI_RC_ADMINMESSAGE			= 63,
	PLI_RC_PRIVADMINMESSAGE		= 64,
	PLI_RC_LISTRCS				= 65,
	PLI_RC_DISCONNECTRC			= 66,
	PLI_RC_APPLYREASON			= 67,
	PLI_RC_SERVERFLAGSGET		= 68,
	PLI_RC_SERVERFLAGSSET		= 69,
	PLI_RC_ACCOUNTADD			= 70,
	PLI_RC_ACCOUNTDEL			= 71,
	PLI_RC_ACCOUNTLISTGET		= 72,
	PLI_RC_PLAYERPROPSGET2		= 73,	// Gets by player ID
	PLI_RC_PLAYERPROPSGET3		= 74,	// Gets by player account name.
	PLI_RC_PLAYERPROPSRESET		= 75,
	PLI_RC_PLAYERPROPSSET2		= 76,
	PLI_RC_ACCOUNTGET			= 77,
	PLI_RC_ACCOUNTSET			= 78,
	PLI_RC_CHAT					= 79,
	PLI_PROFILEGET				= 80,
	PLI_PROFILESET				= 81,
	PLI_RC_WARPPLAYER			= 82,
	PLI_RC_PLAYERRIGHTSGET		= 83,
	PLI_RC_PLAYERRIGHTSSET		= 84,
	PLI_RC_PLAYERCOMMENTSGET	= 85,
	PLI_RC_PLAYERCOMMENTSSET	= 86,
	PLI_RC_PLAYERBANGET			= 87,
	PLI_RC_PLAYERBANSET			= 88,
	PLI_RC_FILEBROWSER_START	= 89,
	PLI_RC_FILEBROWSER_CD		= 90,
	PLI_RC_FILEBROWSER_END		= 91,
	PLI_RC_FILEBROWSER_DOWN		= 92,
	PLI_RC_FILEBROWSER_UP		= 93,
	PLI_NPCSERVERQUERY			= 94,
	PLI_RC_FILEBROWSER_MOVE		= 96,
	PLI_RC_FILEBROWSER_DELETE	= 97,
	PLI_RC_FILEBROWSER_RENAME	= 98,
	PLI_NC_NPCGET				= 103,	// {103}{INT id}
	PLI_NC_NPCDELETE			= 104,	// {104}{INT id}
	PLI_NC_NPCRESET				= 105,	// {105}{INT id}
	PLI_NC_NPCSCRIPTGET			= 106,	// {106}{INT id}
	PLI_NC_NPCWARP				= 107,	// {107}{INT id}{CHAR x*2}{CHAR y*2}{level}
	PLI_NC_NPCFLAGSGET			= 108,	// {108}{INT id}
	PLI_NC_NPCSCRIPTSET			= 109,	// {109}{INT id}{GSTRING script}
	PLI_NC_NPCFLAGSSET			= 110,	// {110}{INT id}{GSTRING flags}
	PLI_NC_NPCADD				= 111,	// {111}{GSTRING info}  - (info) name,id,type,scripter,starting level,x,y
	PLI_NC_CLASSEDIT			= 112,	// {112}{class}
	PLI_NC_CLASSADD				= 113,	// {113}{CHAR name length}{name}{GSTRING script}
	PLI_NC_LOCALNPCSGET			= 114,	// {114}{level}
	PLI_NC_WEAPONLISTGET		= 115,	// {115}
	PLI_NC_WEAPONGET			= 116,	// {116}{weapon}
	PLI_NC_WEAPONADD			= 117,	// {117}{CHAR weapon length}{weapon}{CHAR image length}{image}{code}
	PLI_NC_WEAPONDELETE			= 118,	// {118}{weapon}
	PLI_NC_CLASSDELETE			= 119,	// {119}{class}
	PLI_NC_LEVELLISTGET			= 150,	// {150}
	PLI_NC_LEVELLISTSET			= 151,	// {151}{GSTRING levels}

	PLI_UNKNOWN152				= 152,	// Gets a value from the GraalEngine (or a server-side NPC?) (probably a database)
	PLI_UNKNOWN154				= 154,	// Sets a value on the GraalEngine (or a server-side NPC?) (probably a database)

	PLI_RC_LARGEFILESTART		= 155,
	PLI_RC_LARGEFILEEND			= 156,

	PLI_UNKNOWN157				= 157,	// Something to do with ganis.
	PLI_UPDATESCRIPT			= 158,	// {158}{script} Requests a script from the server.
	PLI_RC_FOLDERDELETE			= 160,
};

enum
{
	PLO_LEVELBOARD		= 0,
	PLO_LEVELLINK		= 1,
	PLO_BADDYPROPS		= 2,
	PLO_NPCPROPS		= 3,
	PLO_LEVELCHEST		= 4,
	PLO_LEVELSIGN		= 5,
	PLO_LEVELNAME		= 6,
	PLO_BOARDMODIFY		= 7,
	PLO_OTHERPLPROPS	= 8,
	PLO_PLAYERPROPS		= 9,
	PLO_ISLEADER		= 10,
	PLO_BOMBADD			= 11,
	PLO_BOMBDEL			= 12,
	PLO_TOALL			= 13,
	PLO_PLAYERWARP		= 14,
	PLO_WARPFAILED		= 15,
	PLO_DISCMESSAGE		= 16,
	PLO_HORSEADD		= 17,
	PLO_HORSEDEL		= 18,
	PLO_ARROWADD		= 19,
	PLO_FIRESPY			= 20,
	PLO_THROWCARRIED	= 21,
	PLO_ITEMADD			= 22,
	PLO_ITEMDEL			= 23,
	PLO_NPCMOVED		= 24,	// What does this do?
	PLO_SIGNATURE		= 25,
	PLO_NPCACTION		= 26,	// What does this do?
	PLO_BADDYHURT		= 27,
	PLO_FLAGSET			= 28,
	PLO_NPCDEL			= 29,
	PLO_FILESENDFAILED	= 30,
	PLO_FLAGDEL			= 31,
	PLO_SHOWIMG			= 32,
	PLO_NPCWEAPONADD	= 33,
	PLO_NPCWEAPONDEL	= 34,
	PLO_RC_ADMINMESSAGE	= 35,
	PLO_EXPLOSION		= 36,
	PLO_PRIVATEMESSAGE	= 37,
	PLO_PUSHAWAY		= 38,	// What does this do?
	PLO_LEVELMODTIME	= 39,
	PLO_HURTPLAYER		= 40,
	PLO_STARTMESSAGE	= 41,
	PLO_NEWWORLDTIME	= 42,
	PLO_DEFAULTWEAPON	= 43,
	PLO_HASNPCSERVER	= 44,	// If sent, the client won't update npc props.
	PLO_FILEUPTODATE	= 45,
	PLO_HITOBJECTS		= 46,
	PLO_STAFFGUILDS		= 47,
	PLO_TRIGGERACTION	= 48,
	PLO_PLAYERWARP2		= 49,	// Bytes 1-3 are x/y/z. 4 = level x in gmap, 5 = level y in gmap.
	PLO_RC_ACCOUNTADD			= 50,	// Deprecated
	PLO_RC_ACCOUNTSTATUS		= 51,	// Deprecated
	PLO_RC_ACCOUNTNAME			= 52,	// Deprecated
	PLO_RC_ACCOUNTDEL			= 53,	// Deprecated
	PLO_RC_ACCOUNTPROPS			= 54,	// Deprecated
	PLO_ADDPLAYER				= 55,
	PLO_DELPLAYER				= 56,
	PLO_RC_ACCOUNTPROPSGET		= 57,	// Deprecated
	PLO_RC_ACCOUNTCHANGE		= 58,	// Deprecated
	PLO_RC_PLAYERPROPSCHANGE	= 59,	// Deprecated
	PLO_EMPTY60					= 60,
	PLO_RC_SERVERFLAGSGET		= 61,
	PLO_RC_PLAYERRIGHTSGET		= 62,
	PLO_RC_PLAYERCOMMENTSGET	= 63,
	PLO_RC_PLAYERBANGET			= 64,
	PLO_RC_FILEBROWSER_DIRLIST	= 65,
	PLO_RC_FILEBROWSER_DIR		= 66,
	PLO_RC_FILEBROWSER_MESSAGE	= 67,
	PLO_LARGEFILESTART			= 68,
	PLO_LARGEFILEEND			= 69,
	PLO_RC_ACCOUNTLISTGET		= 70,
	PLO_RC_PLAYERPROPS			= 71,	// Deprecated
	PLO_RC_PLAYERPROPSGET		= 72,
	PLO_RC_ACCOUNTGET			= 73,
	PLO_RC_CHAT					= 74,
	PLO_PROFILE					= 75,
	PLO_RC_SERVEROPTIONSGET		= 76,
	PLO_RC_FOLDERCONFIGGET		= 77,
	PLO_NPCSERVERADDR			= 79,	// Bytes 1-2 are 0 and 2, followed by a string formatted as <ipaddr>,<port>.
	PLO_NC_LEVELLIST			= 80,	// {80}{GSTRING levels}
	PLO_UNKNOWN82				= 82,	// Answers PLI_UNKNOWN152's request.
	PLO_LARGEFILESIZE			= 84,
	PLO_RAWDATA					= 100,
	PLO_BOARDPACKET				= 101,
	PLO_FILE					= 102,
	PLO_NPCBYTECODE				= 131,	// Compiled Torque-script for an NPC. {131}{INT3 id}{code}
	PLO_NPCDEL2					= 150,	// {150}{CHAR level_length}{level}{INT3 npcid}
	PLO_HIDENPCS				= 151,
	PLO_SAY2					= 153,	// Also used for signs. {153}{text}
	PLO_FREEZEPLAYER2			= 154,	// Blank.
	PLO_UNFREEZEPLAYER			= 155,	// Blank.
	PLO_SETACTIVELEVEL			= 156,	// Sets the level to receive chests, baddies, NPCs, etc.
	PLO_NC_NPCATTRIBUTES		= 157,	// {157}{GSTRING attributes}
	PLO_NC_NPCADD				= 158,	// {158}{INT id}{CHAR 50}{CHAR name length}{name}{CHAR 51}{CHAR type length}{type}{CHAR 52}{CHAR level length}{level}
	PLO_NC_NPCDELETE			= 159,	// {159}{INT id}
	PLO_NC_NPCSCRIPT			= 160,	// {160}{INT id}{GSTRING script}
	PLO_NC_NPCFLAGS				= 161,	// {161}{INT id}{GSTRING flags}
	PLO_NC_CLASSGET				= 162,	// {162}{CHAR name length}{name}{GSTRING script}
	PLO_NC_CLASSADD				= 163,	// {163}{classname}
	PLO_NC_LEVELDUMP			= 164,
	PLO_NC_WEAPONLISTGET		= 167,	// {167}{CHAR name1 length}{name1}{CHAR name2 length}{name2}...
	PLO_EMPTY168				= 168,	// Login server sends this.  Blank packet.
	PLO_GHOSTMODE				= 170,
	PLO_BIGMAP					= 171,
	PLO_MINIMAP					= 172,	// [172] zodiacminimap.txt,zodiacworldminimap3.png,10,10
	PLO_GHOSTTEXT				= 173,	// {173}{text}  Shows static text in lower-right corner of screen only when in ghost mode.
	PLO_GHOSTICON				= 174,	// Pass 1 to enable the ghost icon
	PLO_SHOOT					= 175,
	PLO_FULLSTOP				= 176,	// Sending this causes the entire client to not respond to normal input and it hides the HUD.
	PLO_FULLSTOP2				= 177,	// Sending this causes the entire client to not respond to normal input and it hides the HUD.
	PLO_SERVERWARP				= 178,
	PLO_RPGWINDOW				= 179,
	PLO_STATUSLIST				= 180,
	PLO_LISTPROCESSES			= 182,
	PLO_NC_CLASSDELETE			= 188,	// {188}{class}
	PLO_EMPTY190				= 190,	// Was blank.  Sent before weapon list.
	PLO_NC_WEAPONGET			= 192,	// {192}{CHAR name length}{name}{CHAR image length}{image}{script}
	PLO_EMPTY194				= 194,	// Was blank.  Sent before weapon list.
	PLO_EMPTY195				= 195,	// Something to do with ganis.  [195] )twiz-icon"SETBACKTO "

	// Seems to register NPCs or something on the client.
	// Also is related to PLI_UPDATESCRIPT as it sends the last modification time of the NPC/weapon.  The v5 client stores weapon scripts offline.
	PLO_EMPTY197				= 197,	// Seems to register npcs on the client.  Also is used by client to see if it needs to get a newer version of the offline cache of the NPC.
};

enum
{
	PLTYPE_AWAIT		= (int)(-1),
	PLTYPE_CLIENT		= (int)(1 << 0),
	PLTYPE_RC			= (int)(1 << 1),
	PLTYPE_NPCSERVER	= (int)(1 << 2),
	PLTYPE_NC			= (int)(1 << 3),
	PLTYPE_CLIENT2		= (int)(1 << 4),
	PLTYPE_CLIENT3		= (int)(1 << 5),
	PLTYPE_RC2			= (int)(1 << 6),
	PLTYPE_ANYCLIENT	= (int)(PLTYPE_CLIENT | PLTYPE_CLIENT2 | PLTYPE_CLIENT3),
	PLTYPE_ANYRC		= (int)(PLTYPE_RC | PLTYPE_RC2),
	PLTYPE_ANYNC		= (int)(PLTYPE_NC),
};

enum
{
	GI_LEVELLINK		= 1,
	GI_NPCPROPS			= 3,
	GI_LEVELSIGN		= 5,
	GI_LEVELNAME		= 6,
	GI_OTHERPLPROPS		= 8,
	GI_PLAYERPROPS		= 9,
	GI_FLAGSET			= 28,
	GI_NPCDEL			= 29,
	GI_WEAPONADD		= 33, //Gserver Sending weapon
	GI_PRIVATEMESSAGE	= 37,
	GI_LEVELMODTIME		= 39,
	GI_NEWWORLDTIME		= 42,
	GI_TRIGGERACTION	= 48,
	GI_PLAYER_RIGHTS	= 62,
	GI_NC_REQUEST		= 78,
	GI_NPCDEL2			= 150,
	GI_NPCADDCLASS		= 198,
	GI_LEVELLIST		= 199,
	
};

enum
{
	GO_PLAYERPROPS			= 2,
	GO_NPCPROPS				= 3,
	GO_FLAGSET				= 18,
	GO_RC_CHAT				= 79,
	GO_REQUEST_RIGHTS		= 83,
	GO_NC_QUERY				= 103,
};

enum
{
	NCO_NPCLOG				= 0,
	NCO_GETWEAPONS			= 1,
	NCO_GETLEVELS			= 2,
	NCO_SENDPM				= 3,
	NCO_SENDTORC			= 4,
	NCO_WEAPONADD			= 5,
	NCO_WEAPONDEL			= 6,
	NCO_PLAYERPROPSSET		= 7,
	NCO_PLAYERWEAPONSGET	= 8,
	NCO_PLAYERPACKET		= 9,
	NCO_PLAYERWEAPONADD		= 10,
	NCO_PLAYERWEAPONDEL		= 11,
	NCO_LEVELGET			= 12,
	NCO_NPCPROPSSET			= 13,
	NCO_NPCWARP				= 14,
	NCO_SENDRPGMESSAGE		= 15,
};

enum
{
	NCI_PLAYERWEAPONS		= 0,
	NCI_PLAYERWEAPONADD		= 1,
	NCI_PLAYERWEAPONDEL		= 2,
};



enum
{
	PLPROP_NICKNAME			= 0,
	PLPROP_MAXPOWER			= 1,
	PLPROP_CURPOWER			= 2,
	PLPROP_RUPEESCOUNT		= 3,
	PLPROP_ARROWSCOUNT		= 4,
	PLPROP_BOMBSCOUNT		= 5,
	PLPROP_GLOVEPOWER		= 6,
	PLPROP_BOMBPOWER		= 7,
	PLPROP_SWORDPOWER		= 8,
	PLPROP_SHIELDPOWER		= 9,
	PLPROP_GANI				= 10,	// PLPROP_BOWGIF in pre-2.x
	PLPROP_HEADGIF			= 11,
	PLPROP_CURCHAT			= 12,
	PLPROP_COLORS			= 13,
	PLPROP_ID				= 14,
	PLPROP_X				= 15,
	PLPROP_Y				= 16,
	PLPROP_SPRITE			= 17,
	PLPROP_STATUS			= 18,
	PLPROP_CARRYSPRITE		= 19,
	PLPROP_CURLEVEL			= 20,
	PLPROP_HORSEGIF			= 21,
	PLPROP_HORSEBUSHES		= 22,
	PLPROP_EFFECTCOLORS		= 23,
	PLPROP_CARRYNPC			= 24,
	PLPROP_APCOUNTER		= 25,
	PLPROP_MAGICPOINTS		= 26,
	PLPROP_KILLSCOUNT		= 27,
	PLPROP_DEATHSCOUNT		= 28,
	PLPROP_ONLINESECS		= 29,
	PLPROP_IPADDR			= 30,
	PLPROP_UDPPORT			= 31,
	PLPROP_ALIGNMENT		= 32,
	PLPROP_ADDITFLAGS		= 33,
	PLPROP_ACCOUNTNAME		= 34,
	PLPROP_BODYIMG			= 35,
	PLPROP_RATING			= 36,
	PLPROP_GATTRIB1			= 37,
	PLPROP_GATTRIB2			= 38,
	PLPROP_GATTRIB3			= 39,
	PLPROP_GATTRIB4			= 40,
	PLPROP_GATTRIB5			= 41,
	PLPROP_UNKNOWN42		= 42,
	PLPROP_GMAPLEVELX		= 43,
	PLPROP_GMAPLEVELY		= 44,
	PLPROP_Z				= 45,
	PLPROP_GATTRIB6			= 46,
	PLPROP_GATTRIB7			= 47,
	PLPROP_GATTRIB8			= 48,
	PLPROP_GATTRIB9			= 49,
	PLPROP_JOINLEAVELVL		= 50,
	PLPROP_PCONNECTED		= 51,
	PLPROP_PLANGUAGE		= 52,
	PLPROP_PSTATUSMSG		= 53,
	PLPROP_GATTRIB10		= 54,
	PLPROP_GATTRIB11		= 55,
	PLPROP_GATTRIB12		= 56,
	PLPROP_GATTRIB13		= 57,
	PLPROP_GATTRIB14		= 58,
	PLPROP_GATTRIB15		= 59,
	PLPROP_GATTRIB16		= 60,
	PLPROP_GATTRIB17		= 61,
	PLPROP_GATTRIB18		= 62,
	PLPROP_GATTRIB19		= 63,
	PLPROP_GATTRIB20		= 64,
	PLPROP_GATTRIB21		= 65,
	PLPROP_GATTRIB22		= 66,
	PLPROP_GATTRIB23		= 67,
	PLPROP_GATTRIB24		= 68,
	PLPROP_GATTRIB25		= 69,
	PLPROP_GATTRIB26		= 70,
	PLPROP_GATTRIB27		= 71,
	PLPROP_GATTRIB28		= 72,
	PLPROP_GATTRIB29		= 73,
	PLPROP_GATTRIB30		= 74,
	PLPROP_OSTYPE			= 75,	// 2.19+
	PLPROP_TEXTCODEPAGE		= 76,	// 2.19+
	PLPROP_UNKNOWN77		= 77,
	PLPROP_X2				= 78,
	PLPROP_Y2				= 79,
	PLPROP_Z2				= 80,
	PLPROP_UNKNOWN81		= 81,

	// In Graal v5, where players have the Graal######## accounts, this is their chosen account alias (community name.)
	PLPROP_COMMUNITYNAME	= 82,
};

#endif
