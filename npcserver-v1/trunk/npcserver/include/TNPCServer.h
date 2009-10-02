#ifndef NSERVER_H
#define NSERVER_H

#include <math.h>
#include <algorithm>
#include <vector>
#include "gmSystemLib.h"
#include "CString.h"
#include "IUtil.h"
#include "CSocket.h"
#include "TGserverConn.h"
#include "TPlayerNC.h"
#include "TPlayer.h"
#include "TScriptWeapon.h"
#include "TScriptClass.h"
#include "TLevel.h"
#include "MiscFunctions.h"
#include "PlayerFunctions.h"
#include "NPCFunctions.h"
#include "CSettings.h"


class TPlayerNC;
class TPlayer;
class TScriptClass;
class TScriptWeapon;
class TGserverConn;

struct pPropTable_Data
{
	bool readOnly;
	int propEnumValue;
	int valueType;
	int packetType;

	void Set(bool setReadOnly,int setPropEnumValue, int setValueType, int setPacketType)  //Set data
	{
		readOnly = setReadOnly;
		propEnumValue = setPropEnumValue;
		valueType = setValueType;
		packetType = setPacketType;
	}
};

class TLevel;


class TNPCServer : public CSocketStub
{
	public:
		// Required by CSocketStub.
		bool onRecv();
		bool onSend()				{ return true; }
		bool onRegister()			{ return true; }
		void onUnregister()			{}
		SOCKET getSocketHandle()	{ return playerSock.getHandle(); }
		bool canRecv()				{ return true; }
		bool canSend()				{ return false; }

		//Constructors
		TNPCServer();

		//Deconstructor
		~TNPCServer();

		//Startup and Loop
		bool startServer();
		bool doMain();
		void doTimedEvents();

		//Socket functions
		void acceptNewPlayers(CSocket& pSocket);
		bool sendPacket(CString pPacket);
		bool isGserverConnected();

		//Get Functions
		std::map<CString, TScriptWeapon *>	* getWeaponList()	{ return &mWeapons; }
		std::map<CString, TScriptClass  *>	* getClassList()	{ return &mClasses; }
		std::map<int, TPlayer *> * getGraalPlayers()			{ return &mGraalPlayers; }
		TGserverConn  * getGserverConn()						{ return &mGserverConnection; }
		std::vector<TLevel *>* getLevelList()					{ return &mLevelList; }
		CString getVersion()									{ return m_version; }
		CString getCurrentLevel()								{ return currentLevel; }
		gmMachine * getMachine()								{ return &npcMachine; }
		CString getMOTD()										{ return motd; }
		TScriptClass  * getClass(const CString& className);
		TScriptWeapon * getWeapon(const CString& weaponName);
		TPlayer	* getGraalPlayer(int id);
		TPlayer	* getGraalPlayer(CString account);
		int getPlayerCount();
		TLevel* getLevel(const CString& pLevel);
		TNPC * getNPC(int id);
		TPlayerNC * getNC(CString account);

		//Misc functions
		void sendToAllNC(CString& pPacket);
		void rcChat(CString message);
		void ncChat(CString message);
		void updateTime(int time);
		void addGraalPlayer(int id, CString props);
		void triggerAction(int playerid, int npcid,float x, float y,CString action,CString params);
		void triggerAction(int playerId,CString action,CString params);
		void sendToGserver(CString pPacket);
		void sendPlayerPM(CString account,CString message);
		void RequestLevel(CString level);
		void AddLevel(CString level);
		void UpdateCurrentLevel(CString level);
		void addNPC(int npcId, CString props);
		void deleteNPC(TNPC * npc);
		void SetupPlayerPropAccess();
		void SetupNPCPropAccess();
		void RegisterFunctions();
		void setNCRights(CString account, std::map<CString, CString> rights);
		
		//Weapon Functionality
		bool NC_AddWeapon(TPlayerNC * pPlayerNC,CString name, CString image,CString code);
		bool NC_DeleteWeapon(TPlayerNC * pPlayerNC, CString weapon);
		
		//Class Functionality
		bool NC_AddClass(TPlayerNC * pPlayerNC,CString className,CString scriptData);
		bool NC_DeleteClass(TPlayerNC * pPlayerNC, CString className);

		//PropListAccess and Corrisponding prop enum #
		std::map<CString, pPropTable_Data> playerPropAccess;
		std::map<CString, pPropTable_Data> npcPropAccess;

	private:
		CString mHost,mGserverHost,mAccount,mPassword,m_version,motd,nickname;
		int mPort,mGserverPort,newWorldTime;
		time_t lastTimer;
		CSettings serverSettings;

		//GameMonkey
		gmMachine npcMachine;

		//Scripts
		std::map<CString, TScriptWeapon *> mWeapons;
		std::map<CString, TScriptClass *> mClasses;

		//Flags
		std::map<CString, CString> mServerFlags;
		
		//Connection to Gserver
		TGserverConn mGserverConnection;

		//Players
		std::vector <TPlayerNC *> mNCPlayers;
		std::map<int ,TPlayer *> mGraalPlayers;

		//Levels
		std::vector<TLevel *> mLevelList;
		CString currentLevel;

		//npcs
		std::map<int,TNPC *> npcList;
		
		//Socket Stuff
		CFileQueue fileQueue;
		CSocketManager sockManager;
		CSocket playerSock;
		CString rBuffer, sBuffer, oBuffer;
		unsigned int dataSize;
};
#endif
