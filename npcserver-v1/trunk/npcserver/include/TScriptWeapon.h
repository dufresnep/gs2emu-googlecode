#ifndef TWEAPON_H
#define TWEAPON_H

#include <math.h>
#include <vector>
#include "CString.h"
#include "gmThread.h"
#include "MiscFunctions.h"
#include "TNPCServer.h"
#include "TPlayer.h"
#include "gmCall.h"

class TNPCServer;

class TScriptWeapon
{
public:

	TScriptWeapon(CString newWeaponName,CString newWeaponImage, CString newWeaponScript,TNPCServer * pServer, gmMachine * weaponMachine);
	~TScriptWeapon();
	//Set functions
	void setScript(CString script);
	void update(CString weaponImage, CString weaponScript);

	//Script functions
	void parseServerside();
	void compileScript();
	void callFunction(CString function, TPlayer * pPlayer, CString params);

	static void GM_CDECL TScriptWeaponGetDot(gmThread * a_thread, gmVariable * a_operands);
	static void GM_CDECL TScriptWeaponSetDot(gmThread * a_thread, gmVariable * a_operands);
	
	//Get functions
	CString getImage() 		{ return mWeaponImage; }
	CString getName() 		{ return mWeaponName; }
	CString getFullScript()	{ return mWeaponFullScript; }

	void setVar(CString prop, gmVariable value);
	gmVariable * getThisVar(CString prop){return thisVariables.find(prop) != thisVariables.end() ? thisVariables[prop] : 0;}

	static gmType s_typeId;
	
private:
	CString mWeaponName;
	CString mWeaponImage;
	CString mWeaponFullScript;
	CString mWeaponServerScript;
	CString mWeaponGameMonkeyScript;
	TNPCServer * mServer;
	
	std::map<CString, gmVariable *> thisVariables;

	//GameMonkey
	gmMachine * machine;
	gmCall call;

	int threadID;
	std::vector<int> threads;

	int mIndex;
	
};

#endif