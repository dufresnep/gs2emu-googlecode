#ifndef MAIN_H
#define MAIN_H

#include <QThread>
#include "CList.h"
#include "CRemoteControl.h"

extern CList Connections, Serverlist;
extern CRemoteControl *RC;
extern CString cAccount, cNickname, cPassword;
extern QStringList colors;

int ReturnID(CRCSock *sock);
int ReturnID(CRCSock *sock, CString tab);
CPlayer* FindPlayerID(CRCSock *sock, int pId);
CRCChat* ReturnTab(CRCSock *sock);
CRCChat* ReturnTab(CRCSock *sock, QString tab);
QListWidgetItem* ReturnItem(CRCSock *sock, int pId);
void loadClothes();

// RemoteControl Thread Class
class CRCThread : public QThread
{
	public:
		void run();
};

// Half this shit wont be used, so keep it towards the bottom.

// Window Types
enum
{
	VIEWACCOUNT,
	FILEBROWSER,
};

// Socket Types
enum
{
	SERVERLIST,
	SERVER,
	IRC
};

// Serverlist Types
enum
{
	HOSTED,
	CLASSIC,
	GOLD
};

// Serverlist package types
enum {
  RSERVERLIST,   // 0
  RNULL1,        // 1
  RNULL2,        // 2
  RHOMEURL,      // 3
  RERROR,        // 4
  RPAYURL,       // 5
};

// Server package types
enum {
  LEVELBOARD,   //  0
  LEVELLINK,    //  1
  SBADDYPROPS,  //  2
  SNPCPROPS,    //  3
  LEVELCHEST,   //  4
  LEVELSIGN,    //  5
  LEVELNAME,    //  6
  SBOARDMODIFY, //  7
  OTHERPLPROPS, //  8
  SPLAYERPROPS, //  9
  ISLEADER,     // 10
  SADDBOMB,     // 11 right (stimmt)
  SDELBOMB,     // 12
  STOALLMSG,    // 13 right (stimmt)
  PLAYERWARPED, // 14
  LEVELFAILED,  // 15
  DISMESSAGE,   // 16
  SADDHORSE,    // 17
  SDELHORSE,    // 18
  SADDARROW,    // 19 right (stimmt)
  SFIRESPY,     // 20
  SCARRYTHROWN, // 21
  SADDEXTRA,    // 22 right (stimmt)
  SDELEXTRA,    // 23
  NPCMOVED,     // 24
  UNLIMITEDSIG, // 25
  NPCACTION,    // 26
  SHURTBADDY,   // 27
  SSETFLAG,     // 28
  SDELNPC,      // 29
  GIFFAILED,    // 30
  SUNSETFLAG,   // 31
  SNPCWEAPONIMG,// 32
  SADDNPCWEAPON,// 33
  SDELNPCWEAPON,// 34
  SADMINMSG,    // 35
  SEXPLOSION,   // 36 right (stimmt)
  SPRIVMESSAGE, // 37 right (stimmt)
  PUSHAWAY,     // 38
  LEVELMODTIME, // 39
  SPLAYERHURT,  // 40
  STARTMESSAGE, // 41
  NEWWORLDTIME, // 42
  SADDDEFWEAPON,// 43
  SEMPTY44,     // 44
  SEMPTY45,     // 45
  SEMPTY46,     // 46
  SEMPTY47,     // 47
  STRIGGERACTION,  // 48
  SEMPTY49,     // 49 something with level ? (Irgendwas mit Level ?)
  SADDACCOUNT,  // 50 ftp when sending the file id of the sender (ftp  beim senden von datei id des senders)
  EMPTY51,
  EMPTY52,
  SEMPTY53,  // 53  folder button (folder knopf)
  SEMPTY54,    // 54  folder send rc->server
  SADDPLAYER,   // 55
  SDELPLAYER,   // 56
  SACCPLPROPS,  // 57
  SCHPLACCOUNT, // 58
  SCHPLPROPS,   // 59
  SEMPTY60,     // 60
  SSERVERFLAGS, // 61
  SSENDRIGHTS,  // 62
  SSENDCOM,     // 63 return of the account comments to RC (rückgabe der Account Commentare zum RC schnupp)
  SSENDBAN,     // 64 return of the ban to RC (rückgabe der Ban zum RC schnupp)
  SFOLDERFTP,     // 65  show ftp folder (ftp ordner anzeigen)
  SSENDFTP,     // 66  Found by Twizted =o send FTP shit to rc
  STEXTFTP,     // 67  bottom ftp box (ftp feld unten)
  SEMPTY68,     // 68
  SEMPTY69,     // 69
  DACCLIST,     // 70
  DPLPROPS,     // 71
  DACCPLPROPS,  // 72
  DACCOUNT,     // 73
  DRCLOG,       // 74
  DPROFILE,     // 75
  DSENDSOPTIONS,// 76 is to send server options to rc...not rights... *Twizted*
  SSENDFOLDER,  // 77 Folder Config Schnuppie
  EMPTY78,
  EMPTY79,
  EMPTY80,
  EMPTY81,
  EMPTY82,
  EMPTY83,
  EMPTY84,
  EMPTY85,
  EMPTY86,
  EMPTY87,
  EMPTY88,
  EMPTY89,
  EMPTY90,
  EMPTY91,
  EMPTY92,
  EMPTY93,
  DNPCFLAG = 157,
  DCLASS = 162,
  DLEVELVIEW = 164,
  DWEPLIST = 167,
  SSHOOT = 175
 };

// Client package types
enum {
  LEVELWARP,    //  0
  BOARDMODIFY,  //  1
  PLAYERPROPS,  //  2
  NPCPROPS,     //  3
  ADDBOMB,      //  4
  DELBOMB,      //  5
  TOALLMSG,     //  6
  ADDHORSE,     //  7
  DELHORSE,     //  8
  ADDARROW,     //  9
  FIRESPY,      // 10
  CARRYTHROWN,  // 11
  ADDEXTRA,     // 12
  DELEXTRA,     // 13
  CLAIMPKER,    // 14
  BADDYPROPS,   // 15
  HURTBADDY,    // 16
  ADDBADDY,     // 17
  SETFLAG,      // 18
  UNSETFLAG,    // 19
  OPENCHEST,    // 20
  ADDNPC,       // 21
  DELNPC,       // 22
  WANTFILE,      // 23
  NPCWEAPONIMG, // 24
  EMPTY25,      // 25
  HURTPLAYER,   // 26
  EXPLOSION,    // 27
  PRIVMESSAGE,  // 28
  DELNPCWEAPON, // 29
  MODLEVELWARP, // 30
  PACKCOUNT,    // 31
  TAKEEXTRA,    // 32
  ADDWEAPON2,   // 33
  UPDATEFILE,      // 34 - Believed to be mainsocket; Do not edit its dynamic?
  OUTTERMAP,    // 35
  EMPTY36,      // 36
  LANGUAGE,     // 37
  TRIGGERACTION,      // 38
  MAPINFO,      // 39
  CSHOOT,      // 40
  EMPTY41,      // 41
  EMPTY42,      // 42
  EMPTY43,      // 43
  EMPTY44,      // 44
  EMPTY45,      // 45
  EMPTY46,      // 46
  SSTAFFGUILDS,      // 47
  EMPTY48,      // 48
  EMPTY49,      // 49
  EMPTY50,      // 50
  SWANTSOPTIONS, // 51
  SSETOPTIONS,  // 52
  WANTRCFOLDERS,// 53
  SETRCFOLDERS,   // 54
  SETRESPAWN,   // 55
  SETHORSELIFE, // 56
  SETAPINCRTIME,// 57
  SETBADDYRESP, // 58
  WANTPLPROPS,  // 59
  SETPLPROPS,   // 60
  DISPLAYER,    // 61
  UPDLEVELS,    // 62
  ADMINMSG,     // 63
  PRIVADMINMSG, // 64
  LISTRCS,      // 65
  DISCONNECTRC, // 66
  APPLYREASON,  // 67
  LISTSFLAGS,   // 68
  SETSFLAGS,    // 69
  DADDACCOUNT,    // 70
  DDELACCOUNT,    // 71
  DWANTACCLIST,   // 72
  DWANTPLPROPS,   // 73
  DWANTACCPLPROPS,// 74
  DRESETPLPROPS,  // 75
  DSETACCPLPROPS, // 76
  DWANTACCOUNT,   // 77
  DSETACCOUNT,    // 78
  DRCCHAT,        // 79
  DGETPROFILE,    // 80
  DSETPROFILE,    // 81
  WARPPLAYER,     // 82
  DWANTRIGHTS,    // 83
  DSETRIGHTS,     // 84 set rights schnuppie  .. RC -> Server (rights setzen schnuppie  .. RC -> Server)
  DWANTCOM,       // 85 request comments (kommentare anfordern)
  DSETCOM,		  // 86 change comments (kommentare ändern)
  DEDITBAN,       // 87 request ban (ban anfordern)
  DSETBAN,        // 88 set ban (ban setzen)
  DWANTFTP,       //89 found by twitzed
  DCHANGEFTP,     // 90 ftp folder change (ftp ordner change)
  DENDFTP,        // 91 ftp data transfer done (ftp datentransfer ende)
  DFILEFTPDOWN,   // 92 request file download (anfrage file download)
  DFILEFTPUP,     // 93 file upload
  DNPCSERVER,     // 94
  EMPTY95,        // 95
  EMPTY96,        // 96
  EMPTY97,        // 97
  EMPTY98,        // 98
  EMPTY99,        // 99
  EMPTY100,       // 100
  EMPTY101,       // 101
  EMPTY102,       // 102
  EMPTY103,       // 103
  EMPTY104,       // 104
  EMPTY105,       // 105
  EMPTY106,       // 106
  EMPTY107,       // 107
  EMPTY108,       // 108
  EMPTY109,       // 109
  EMPTY110,       // 110
  SADDNPC,        // 111
  EMPTY112,       // 112
  SADDCLASS,      // 113
  SLOCALNPC,      // 114
  SWANTWEAPON,    // 115
  SEDITWEAPON,    // 116
  SADDWEAPON,     // 117
  SDELWEAPON,     // 118
};
#define clientpackages 117

// Player properties types
enum {
  NICKNAME,     //  0
  MAXPOWER,     //  1
  CURPOWER,     //  2
  RUPEESCOUNT,  //  3
  ARROWSCOUNT,  //  4
  BOMBSCOUNT,   //  5
  GLOVEPOWER,   //  6
  BOMBPOWER,    //  7
  SWORDPOWER,   //  8
  SHIELDPOWER,  //  9
  PLAYERANI,    // 10
  HEADGIF,      // 11
  CURCHAT,      // 12
  PLAYERCOLORS, // 13
  PLAYERID,     // 14
  PLAYERX,      // 15
  PLAYERY,      // 16
  PLAYERSPRITE, // 17
  STATUS,       // 18
  CARRYSPRITE,  // 19
  CURLEVEL,     // 20
  HORSEGIF,     // 21
  HORSEBUSHES,  // 22
  EFFECTCOLORS, // 23
  CARRYNPC,     // 24
  APCOUNTER,    // 25
  MAGICPOINTS,  // 26
  KILLSCOUNT,   // 27
  DEATHSCOUNT,  // 28
  ONLINESECS,   // 29
  LASTIP,       // 30
  UDPPORT,      // 31
  PALIGNMENT,   // 32
  PADDITFLAGS,  // 33
  PACCOUNTNAME, // 34
  BODYIMG,      // 35
  RATING,     // 36
  GATTRIB1,     // 37
  GATTRIB2,     // 38
  GATTRIB3,     // 39
  GATTRIB4,     // 40
  GATTRIB5,     // 41
  PEMPTY42,     // 42
  PEMPTY43,     // 43
  PEMPTY44,     // 44
  PLAYERZ,     // 45
  PEMPTY46,     // 46
  PEMPTY47,     // 47
  PEMPTY48,     // 48
  PEMPTY49,     // 49
  PEMPTY50,   // 50
  PCONNECTED,     // 51
  PLANGUAGE,     // 52
  PSTATUSMSG,   // 53
  UNKNOWN1 = 87	// 87
};
#define propscount 54

// Admin rights
enum {
	RIGHT1  = 1,
	RIGHT2  = 2,
	RIGHT3  = 4,
	RIGHT4  = 8,
	RIGHT5  = 16,
	RIGHT6  = 32,
	RIGHT7  = 64,
	RIGHT8  = 128,
	RIGHT9  = 256,
	RIGHT10 = 512,
	RIGHT11 = 1024,
	RIGHT12 = 2048,
	RIGHT13 = 4096,
	RIGHT14 = 8192,
	RIGHT15 = 16384,
	RIGHT16 = 32768,
	RIGHT17 = 65536,
	RIGHT18 = 131072,
	RIGHT19 = 262144,
	RIGHT20 = 524288,
};

// Client types
enum {
  CLIENTPLAYER, // 0
  CLIENTRC,     // 1
  CLIENTNPC     // 2
};

#endif