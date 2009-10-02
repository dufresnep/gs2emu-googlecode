#ifndef TNPC_H
#define TNPC_H
#include <math.h>
#include <vector>
#include "ICommon.h"
#include "CString.h"
#include "gmThread.h"
#include "gmCall.h"
#include "TPlayer.h"
#include "TNPCServer.h"
#include "MiscFunctions.h"

class TNPCServer;
//#include "TLevel.h"



//#define npcpropcount 48
#define npcpropcount 77

enum
{
	NPCSTATUS_DRAWOVERPLAYER	= 0x02,
};

class TNPCServer;
class TLevel;
class TNPC
{
	public:
		TNPC(int npcId, TNPCServer* pServer, TLevel* pLevel,gmMachine * npcMachine, bool pLevelNPC = true);
		~TNPC();

		// prop functions
		CString getProp(unsigned char pId, int clientVersion = CLVER_2_17) const;
		CString getProps(time_t newTime, int clientVersion = CLVER_2_17) const;
		void setProps(CString& pProps, int clientVersion = CLVER_2_17);
		void SendProp(int pPropId);

		// set functions
		void setId(unsigned int pId)	{ id = pId; }
		void decTimer();
		void setTimer(float timer)	{timeout = timer;}

		// get functions
		unsigned int getId() const		{ return id; }
		TLevel* getLevel()				{ return level; }
		CString getWeaponName() const	{ return weaponName; }
		CString getServerScript() const	{ return serverScript; }
		CString getFullScript() const	{ return fullScript; }
		float getY() {return y;}
		float getX() {return x;}
		time_t getPropModTime(unsigned char pId);
		TNPCServer * GetServer()	{return server;}
		gmMachine * GetMachine()	{return machine;}
		float getTimer()			{return timeout;}
		
		
	
		//Script functions
		void parseServerside();
		void compileScript();
		void runScript();
		void callFunction(CString function, TPlayer * pPlayer, CString params);
		void setVar(CString prop, gmVariable value);
		gmVariable * getThisVar(CString prop){return thisVariables.find(prop) != thisVariables.end() ? thisVariables[prop] : 0;}
		static void TNPCGetDot(gmThread * a_thread, gmVariable * a_operands);
		static void TNPCSetDot(gmThread * a_thread, gmVariable * a_operands);
		static gmType s_typeId;
		static gmType s_typeIdForVariables;

		float width,height;

	private:
		bool levelNPC;
		time_t modTime[npcpropcount];
		float x, y, hurtX, hurtY,timeout;
		int x2, y2;
		
		unsigned char gmaplevelx, gmaplevely;
		unsigned int id;
		int rupees;
		unsigned char darts, bombs, glovePower, bombPower, swordPower, shieldPower;
		unsigned char visFlags, blockFlags, sprite, colors[5], power, ap;
		CString gAttribs[30];
		CString image, swordImage, shieldImage, headImage, bodyImage, horseImage, bowImage, gani;
		CString nickName, imagePart, chatMsg, weaponName;
		CString serverScript, fullScript,gameMonkeyScript;
		std::map<CString, gmVariable *> thisVariables;
		
	
		unsigned char saves[10];
		TLevel* level;
		TNPCServer* server;

		//GameMonkey
		gmMachine * machine;
		
		int threadID;
		std::vector<int> threads;
};

inline
time_t TNPC::getPropModTime(unsigned char pId)
{
	if (pId < npcpropcount) return modTime[pId];
	return 0;
}

#endif
