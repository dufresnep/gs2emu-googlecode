#ifndef MAIN_H
#define MAIN_H

/*
	QT-Runtime
*/
#include <QApplication>
#include <QtGui>
#include <QMessageBox>
#include <QString>
#include <QTimer>

/*
	Window-Headers
*/
#include "TLoginWindow.h"
#include "TPlayerWindow.h"
#include "TRCWindow.h"
#include "TServerWindow.h"
#include "TTextEditor.h"
#include "TViewAccount.h"

/*
	More-Headers
*/
#include "TRCSock.h"
#include "TRCListSock.h"
#include "TSockList.h"

/*
	Function Definitions
*/
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

/*
    Regular Definitions
*/
#define list_ip "listserver.graal.in"
#define list_port 14922

/*
	Enumerators
*/

// Server-Types
enum
{
	TYPE_CLASSIC,
	TYPE_GOLD,
	TYPE_HIDDEN,
	TYPE_HOSTED
};

// Socket-Types
enum
{
    STYPE_NONE,
    STYPE_IRC,
    STYPE_SERVER,
    STYPE_SERVERLIST
};

// Serverlist - Incoming Packet
enum
{
	SERVERLIST_0,       // 0
	SERVERLIST_1,       // 1
	SERVERLIST_2,       // 2
	SERVERLIST_HOMEURL, // 3
	SERVERLIST_ERROR,   // 4
	SERVERLIST_PAYURL,  // 5
};

// RC - Incoming Packet
enum
{
	RCI_OPLPROPS = 8,  // other-player props
	RCI_OSPLPROPS = 9, // rc-player props
	RCI_DISCMSG = 16,
	RCI_SIGNATURE = 25,
	RCI_SERVERFLAGS = 28,
	RCI_GRAALTIME = 42,
	RCI_ADDPLAYER = 55,
	RCI_DELPLAYER = 56,
	RCI_OPENRIGHT = 62,
	RCI_OPENCMMTS = 63,
	RCI_OPENATTRS = 72,
	RCI_OPENACCTS = 73,
	RCI_CHATLOG = 74,
	RCI_GETSVROPTS = 76,
	RCI_GETFLDRCFG = 77,
};

// RC - Outcoming Packet
enum
{
	RCO_PLPROPS = 2,
	RCO_GETSVROPTS = 51,
	RCO_GETFLDRCFG = 53,
	RCO_CHATLOG = 79,
};

// RC - Window-Types
enum
{
	RCW_VIEWACCOUNT,
	RCW_TEXTEDITOR,
	RCW_TEXTEDITORH,
};

// Player Props
enum
{
	NICKNAME,	  //  0
	MAXPOWER,	  //  1
	CURPOWER,	  //  2
	RUPEESCOUNT,   //  3
	ARROWSCOUNT,   //  4
	BOMBSCOUNT,	//  5
	GLOVEPOWER,	//  6
	BOMBPOWER,	 //  7
	SWORDPOWER,	//  8
	SHIELDPOWER,   //  9
	PLAYERANI,	 // 10
	HEADGIF,	   // 11
	CURCHAT,	   // 12
	PLAYERCOLORS,  // 13
	PLAYERID,	  // 14
	PLAYERX,	   // 15
	PLAYERY,	   // 16
	PLAYERSPRITE,  // 17
	STATUS,		// 18
	CARRYSPRITE,   // 19
	CURLEVEL,	  // 20
	HORSEGIF,	  // 21
	HORSEBUSHES,   // 22
	EFFECTCOLORS,  // 23
	CARRYNPC,	  // 24
	APCOUNTER,	 // 25
	MAGICPOINTS,   // 26
	KILLSCOUNT,	// 27
	DEATHSCOUNT,   // 28
	ONLINESECS,	// 29
	LASTIP,		// 30
	UDPPORT,	   // 31
	PALIGNMENT,	// 32
	PADDITFLAGS,   // 33
	PACCOUNTNAME,  // 34
	BODYIMG,	   // 35
	RATING,		// 36
	GATTRIB1,	  // 37
	GATTRIB2,	  // 38
	GATTRIB3,	  // 39
	GATTRIB4,	  // 40
	GATTRIB5,	  // 41
	PEMPTY42,	  // 42
	PEMPTY43,	  // 43
	PEMPTY44,	  // 44
	PLAYERZ,	   // 45
	GATTRIB6,	  // 46
	GATTRIB7,	  // 47
	GATTRIB8,	  // 48
	GATTRIB9,	  // 49
	PEMPTY50,	  // 50
	PCONNECTED,	// 51
	PLANGUAGE,	 // 52
	PSTATUSMSG,	// 53
	GATTRIB10,	 // 54
	GATTRIB11,	 // 55
	GATTRIB12,	 // 56
	GATTRIB13,	 // 57
	GATTRIB14,	 // 58
	GATTRIB15,	 // 59
	GATTRIB16,	 // 60
	GATTRIB17,	 // 61
	GATTRIB18,	 // 62
	GATTRIB19,	 // 63
	GATTRIB20,	 // 64
	GATTRIB21,	 // 65
	GATTRIB22,	 // 66
	GATTRIB23,	 // 67
	GATTRIB24,	 // 68
	GATTRIB25,	 // 69
	GATTRIB26,	 // 70
	GATTRIB27,	 // 71
	GATTRIB28,	 // 72
	GATTRIB29,	 // 73
	GATTRIB30,	 // 74
	UNKNOWN1 = 87// 87
};

/*
	External Functions
*/
CBuffer compressBuffer(CBuffer pBuffer);
CBuffer decompressBuffer(CBuffer pBuffer);
CBuffer toString(QString pBuffer);
void createColors();
void end();

/*
	External Variables
*/
extern TLoginWindow *LoginWindow;
extern TRCWindow *RCWindow;
extern TServerWindow *ServerWindow;

extern CBuffer graalAccount, graalNickname, graalPassword;
extern TSockList sockList;
extern QStringList colors;

#endif // MAIN_H

