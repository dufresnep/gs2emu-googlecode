// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef CPLAYERH
#define CPLAYERH

#include "CString.h"
#include "CStringList.h"
#include "CSocket.h"
#include "CPacket.h"
#include "CLevel.h"
#include "CAccount.h"
#include "CMap.h"

#ifdef GSERV22
	#include "crypt.h"
#endif

#define maxAdmin 0xFDFFF
#define itemcount 25

class CPlayer;
typedef void (CPlayer::*pt2func)(CPacket&);

// Server package types
enum {
	LEVELBOARD,   //  0
	LEVELLINK,	//  1
	SBADDYPROPS,  //  2
	SNPCPROPS,	//  3
	LEVELCHEST,   //  4
	LEVELSIGN,	//  5
	LEVELNAME,	//  6
	SBOARDMODIFY, //  7
	OTHERPLPROPS, //  8
	SPLAYERPROPS, //  9
	ISLEADER,	 // 10
	SADDBOMB,	 // 11 right (stimmt)
	SDELBOMB,	 // 12
	STOALLMSG,	// 13 right (stimmt)
	PLAYERWARPED, // 14
	LEVELFAILED,  // 15
	DISMESSAGE,   // 16
	SADDHORSE,	// 17
	SDELHORSE,	// 18
	SADDARROW,	// 19 right (stimmt)
	SFIRESPY,	 // 20
	SCARRYTHROWN, // 21
	SADDEXTRA,	// 22 right (stimmt)
	SDELEXTRA,	// 23
	NPCMOVED,	 // 24
	UNLIMITEDSIG, // 25
	NPCACTION,	// 26
	SHURTBADDY,   // 27
	SSETFLAG,	 // 28
	SDELNPC,	  // 29
	GIFFAILED,	// 30
	SUNSETFLAG,   // 31
	SNPCWEAPONIMG,// 32
	SADDNPCWEAPON,// 33
	SDELNPCWEAPON,// 34
	SADMINMSG,	// 35
	SEXPLOSION,   // 36 right (stimmt)
	SPRIVMESSAGE, // 37 right (stimmt)
	PUSHAWAY,	 // 38
	LEVELMODTIME, // 39
	SPLAYERHURT,  // 40
	STARTMESSAGE, // 41
	NEWWORLDTIME, // 42
	SADDDEFWEAPON,// 43
	SEMPTY44,	 // 44
	SEMPTY45,	 // 45
	SEMPTY46,	 // 46
	SEMPTY47,	 // 47
	STRIGGERACTION,  // 48
	SEMPTY49,	 // 49 something with level ? (Irgendwas mit Level ?)
	SADDACCOUNT,  // 50 ftp when sending the file id of the sender (ftp  beim senden von datei id des senders)
	EMPTY51,
	EMPTY52,
	SEMPTY53,  // 53  folder button (folder knopf)
	SEMPTY54,	// 54  folder send rc->server
	SADDPLAYER,   // 55
	SDELPLAYER,   // 56
	SACCPLPROPS,  // 57
	SCHPLACCOUNT, // 58
	SCHPLPROPS,   // 59
	SEMPTY60,	 // 60
	SSERVERFLAGS, // 61
	SSENDRIGHTS,  // 62
	SSENDCOM,	 // 63 return of the account comments to RC (r�ckgabe der Account Commentare zum RC schnupp)
	SSENDBAN,	 // 64 return of the ban to RC (r�ckgabe der Ban zum RC schnupp)
	SFOLDERFTP,	 // 65  show ftp folder (ftp ordner anzeigen)
	SSENDFTP,	 // 66  Found by Twizted =o send FTP shit to rc
	STEXTFTP,	 // 67  bottom ftp box (ftp feld unten)
	SEMPTY68,	 // 68
	SEMPTY69,	 // 69
	DACCLIST,	 // 70
	DPLPROPS,	 // 71
	DACCPLPROPS,  // 72
	DACCOUNT,	 // 73
	DRCLOG,	   // 74
	DPROFILE,	 // 75
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
	SSHOOT = 175
 };

// Client package types
enum {
	LEVELWARP,	//  0
	BOARDMODIFY,  //  1
	PLAYERPROPS,  //  2
	NPCPROPS,	 //  3
	ADDBOMB,	  //  4
	DELBOMB,	  //  5
	TOALLMSG,	 //  6
	ADDHORSE,	 //  7
	DELHORSE,	 //  8
	ADDARROW,	 //  9
	FIRESPY,	  // 10
	CARRYTHROWN,  // 11
	ADDEXTRA,	 // 12
	DELEXTRA,	 // 13
	CLAIMPKER,	// 14
	BADDYPROPS,   // 15
	HURTBADDY,	// 16
	ADDBADDY,	 // 17
	SETFLAG,	  // 18
	UNSETFLAG,	// 19
	OPENCHEST,	// 20
	ADDNPC,	   // 21
	DELNPC,	   // 22
	WANTFILE,	  // 23
	NPCWEAPONIMG, // 24
	EMPTY25,	  // 25
	HURTPLAYER,   // 26
	EXPLOSION,	// 27
	PRIVMESSAGE,  // 28
	DELNPCWEAPON, // 29
	MODLEVELWARP, // 30
	PACKCOUNT,	// 31
	TAKEEXTRA,	// 32
	ADDWEAPON2,   // 33
	UPDATEFILE,	  // 34 - Believed to be mainsocket; Do not edit its dynamic?
	OUTTERMAP,	// 35
	EMPTY36,	  // 36
	LANGUAGE,	 // 37
	TRIGGERACTION,	  // 38
	MAPINFO,	  // 39
	CSHOOT,	  // 40
	EMPTY41,	  // 41
	EMPTY42,	  // 42
	EMPTY43,	  // 43
	SLISTPROCESSES,	  // 44
	EMPTY45,	  // 45 - Packet 183 makes this come from client?
	EMPTY46,	  // 46
	SSTAFFGUILDS,	  // 47
	EMPTY48,	  // 48
	EMPTY49,	  // 49
	EMPTY50,	  // 50
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
	DISPLAYER,	// 61
	UPDLEVELS,	// 62
	ADMINMSG,	 // 63
	PRIVADMINMSG, // 64
	LISTRCS,	  // 65
	DISCONNECTRC, // 66
	APPLYREASON,  // 67
	LISTSFLAGS,   // 68
	SETSFLAGS,	// 69
	DADDACCOUNT,	// 70
	DDELACCOUNT,	// 71
	DWANTACCLIST,   // 72
	DWANTPLPROPS,   // 73
	DWANTACCPLPROPS,// 74
	DRESETPLPROPS,  // 75
	DSETACCPLPROPS, // 76
	DWANTACCOUNT,   // 77
	DSETACCOUNT,	// 78
	DRCCHAT,		// 79
	DGETPROFILE,	// 80
	DSETPROFILE,	// 81
	WARPPLAYER,	 // 82
	DWANTRIGHTS,		// 83
	DSETRIGHTS,		// 84 set rights schnuppie  .. RC -> Server (rights setzen schnuppie  .. RC -> Server)
	DWANTCOM,		// 85 request comments (kommentare anfordern)
	DSETCOM,		// 86 change comments (kommentare �ndern)
	DEDITBAN,		// 87 request ban (ban anfordern)
	DSETBAN,		// 88 set ban (ban setzen)
	DWANTFTP,		//89 found by twitzed
	DCHANGEFTP,	 // 90 ftp folder change (ftp ordner change)
	DENDFTP,	  // 91 ftp data transfer done (ftp datentransfer ende)
	DFILEFTPDOWN,	 // 92 request file download (anfrage file download)
	DFILEFTPUP,	 // 93 file upload
	DFILEFTPMOV = 96, // 96 file move
	DFILEFTPDEL = 97, // 97 file delete
	DFILEFTPREN = 98, // 98 file rename
	SHIDENPCS = 151,
	SSAY2 = 153,	 // 153 Say2 Command
	SFREEZE = 154,   // 154 Freeze Player
	SUNFREEZE = 155, // 155 Unfreeze Player
	SGHOST = 170,  // Turns into ghost
	SGHOSTICON, // 174 Ghost icon - shows when a ghost is in your level
	EMPTY175,   // 175 Hides GUI / Disables Weapons / Disables Movement
	EMPTY176,   // 176 Hides GUI / Disables Weapons / Disables Movement
	EMPTY177,   // 177 Hides GUI / Disables Weapons / Disables Movement
	EMPTY178,   // 178
	SRPGWINDOW = 179, // 179 RPG Window
	SSTATUSLIST = 180, // 180 Client status list
	SPROCCESSES = 182 // use for hack-detection?
};
#define clientpackages 100
// Player properties types
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
#define propscount 75
// Client types
enum {
  CLIENTPLAYER, // 0
  CLIENTRC	  // 1
};
//Admin rights

enum {
	CANWARPXY = 1,
	CANWARPTOPLAYER = 2,
	CANWARPPLAYER = 4,
	CANUPDATELEVEL = 8,
	CANDISCONNECT = 16,
	CANVIEWATTRIBS = 32,
	CANSETATTRIBS = 64,
	CANSETOWNATTRIBS = 128,
	CANRESETATTRIBS = 256,
	CANADMINMSG = 512,
	CANCHANGERIGHTS = 1024,
	CANBAN = 2048,
	CANCHANGECOMMENTS = 4096,
	CANINVISONLIST = 8192,
	CANCHANGESTAFFACC = 16384,
	CANSETSERVERFLAG = 32768,
	CANEDITSERVEROPTION = 65536,
	CANEDITFOLDEROPTION = 131072,
	CANEDITFOLDERRIGHTS = 262144,
	CANNPCCONTROL = 524288,
};

class CPlayer : public CAccount
{
	public:
		bool firstPacket, deleteMe, firstLevel, allowBomb;
		CBuffer bowImage, chatMsg, guildName, language, outBuff, sendBuff, version;
		CLevel *level;
		CList enteredLevels, fileList;
		CSocket *playerSock;
		CStringList weaponSend;
		int additionalFlags, carrySprite, failAttempts, key, iterator, id, packCount, rating, statusMsg, udpPort;
		time_t lastCheck, lastData, lastMessage, lastMovement, lastNick, lastSave, loginTime;
		#ifdef GSERV22
			codec mcodec;
		#endif

		CPlayer(CSocket* pSocket);
		~CPlayer();
		void main();
		void updateProp(int pPropIndex);
		void parsePacket(CPacket& pPacket);
		void sendPacket(CPacket& pPacket);
		void processChat(CString& pMessage);
		void compressAndSend();
		void sendOutGoing();
		void dropItems();
		bool sendLevel(CString& pLevel, float pX, float pY, int pModTime);
		void warp(CString& pLevel, float pX, float pY, int pModTime = 0);
		void leaveLevel();
		void setNick(CString& pNewNick, bool pVerifyGuild);
		void sendWeapons();
		void sendFiles();
		time_t getLeavingTime(CLevel* pLevel);
		CPacket getProp(int pProp);
		void setProps(CPacket& pProps, bool pForward);
		void getItem(int pItem);
		void sendLocally(CPacket& pPacket);
		bool loadAccount();
		bool hasRight(int pRight);
		bool hasStaff();
		void sendServerFlag(CString& pFlag);
		void sendAccount();
		void processLogin(CPacket& pPacket);
		CPacket getAccPropsRC();
		void setAccPropsRc(CPacket& pPacket);

		/* Helper-Functions */
		void setBan(CBuffer &pReason, bool pBan, bool pAppend = true);

		/* Messages */
		static pt2func msgFuncs[];
		void msgLEVELWARP(CPacket&);
		void msgBOARDMODIFY(CPacket&);
		void msgPLAYERPROPS(CPacket&);
		void msgNPCPROPS(CPacket&);
		void msgADDBOMB(CPacket&);
		void msgDELBOMB(CPacket&);
		void msgTOALLMSG(CPacket&);
		void msgADDHORSE(CPacket&);
		void msgDELHORSE(CPacket&);
		void msgADDARROW(CPacket&);
		void msgFIRESPY(CPacket&);
		void msgCARRYTHROWN(CPacket&);
		void msgADDEXTRA(CPacket&);
		void msgDELEXTRA(CPacket&);
		void msgCLAIMPKER(CPacket&);
		void msgBADDYPROPS(CPacket&);
		void msgHURTBADDY(CPacket&);
		void msgADDBADDY(CPacket&);
		void msgSETFLAG(CPacket&);
		void msgUNSETFLAG(CPacket&);
		void msgOPENCHEST(CPacket&);
		void msgADDNPC(CPacket&);
		void msgDELNPC(CPacket&);
		void msgWANTFILE(CPacket&);
		void msgNPCWEAPONIMG(CPacket&);
		void msgEMPTY25(CPacket&);
		void msgHURTPLAYER(CPacket&);
		void msgEXPLOSION(CPacket&);
		void msgPRIVMESSAGE(CPacket&);
		void msgDELNPCWEAPON(CPacket&);
		void msgMODLEVELWARP(CPacket&);
		void msgPACKCOUNT(CPacket&);
		void msgTAKEEXTRA(CPacket&);
		void msgADDWEAPON2(CPacket&);
		void msgUPDATEFILE(CPacket&);
		void msgOUTTERMAP(CPacket&);
		void msgEMPTY36(CPacket&);
		void msgLANGUAGE(CPacket&);
		void msgTRIGGERACTION(CPacket&);
		void msgEMPTY39(CPacket&);
		void msgCSHOOT(CPacket&);
		void msgEMPTY41(CPacket&);
		void msgEMPTY42(CPacket&);
		void msgEMPTY43(CPacket&);
		void msgSLISTPROCESSES(CPacket&);
		void msgEMPTY45(CPacket&);
		void msgEMPTY46(CPacket&);
		void msgEMPTY47(CPacket&);
		void msgEMPTY48(CPacket&);
		void msgEMPTY49(CPacket&);
		void msgEMPTY50(CPacket&);
		void msgSWANTSOPTIONS(CPacket&);
		void msgSSETOPTIONS(CPacket&);
		void msgWANTRCFOLDERS(CPacket&);
		void msgSETRCFOLDERS(CPacket&);
		void msgSETRESPAWN(CPacket&);
		void msgSETHORSELIFE(CPacket&);
		void msgSETAPINCRTIME(CPacket&);
		void msgSETBADDYRESP(CPacket&);
		void msgWANTPLPROPS(CPacket&);
		void msgSETPLPROPS(CPacket&);
		void msgDISPLAYER(CPacket&);
		void msgUPDLEVELS(CPacket&);
		void msgADMINMSG(CPacket&);
		void msgPRIVADMINMSG(CPacket&);
		void msgLISTRCS(CPacket&);
		void msgDISCONNECTRC(CPacket&);
		void msgAPPLYREASON(CPacket&);
		void msgLISTSFLAGS(CPacket&);
		void msgSETSFLAGS(CPacket&);
		void msgDADDACCOUNT(CPacket&);
		void msgDDELACCOUNT(CPacket&);
		void msgDWANTACCLIST(CPacket&);
		void msgDWANTPLPROPS(CPacket&);
		void msgDWANTACCPLPROPS(CPacket&);
		void msgDRESETPLPROPS(CPacket&);
		void msgDSETACCPLPROPS(CPacket&);
		void msgDWANTACCOUNT(CPacket&);
		void msgDSETACCOUNT(CPacket&);
		void msgDRCCHAT(CPacket&);
		void msgDGETPROFILE(CPacket&);
		void msgDSETPROFILE(CPacket&);
		void msgWARPPLAYER(CPacket&);
		void msgDWANTRIGHTS(CPacket&);
		void msgDSETRIGHTS(CPacket&);
		void msgDWANTCOM(CPacket&);
		void msgDSETCOM(CPacket&);
		void msgDEDITBAN(CPacket&);
		void msgDSETBAN(CPacket&);
		void msgDWANTFTP(CPacket&);
		void msgDCHANGEFTP(CPacket&);
		void msgDENDFTP(CPacket&);
		void msgDFILEFTPDOWN(CPacket&);
		void msgDFILEFTPUP(CPacket&);
		void msgDFILEFTPMOV(CPacket&);
		void msgDFILEFTPDEL(CPacket&);
		void msgDFILEFTPREN(CPacket&);
};

class CEnteredLevel
{
	public:
		CLevel* level;
		time_t time;

		CEnteredLevel(CLevel* pLevel, time_t pTime)
		{
			level = pLevel;
			time = pTime;
		}
};

class COutFile
{
	public:
		CString fileName, longName;
		long long modTime;

		COutFile(CString& pFileName, long long pModTime)
		{
			fileName = pFileName;
			modTime = pModTime;
		}

		COutFile(CString& pLongName)
		{
			CStringList words;
			words.load(pLongName.text(), "/");
			fileName = words[words.count() - 1];
			longName = pLongName;
			modTime = 0;
		}
};
#endif // CPLAYERH
