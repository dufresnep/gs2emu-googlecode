/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#ifndef CGMAPH
#define CGMAPH
#include "CString.h"
#include "CStringList.h"
#include "CList.h"
#include "CLevel.h"

class CMap
{
	int width, height;
	CStringList levelList;

	public:
		CString fileName;
		static CList mapList;
		//Load new gmap files
		bool loadGMap(CString& pMapName);
		//Load the old txt map files
		bool loadTXT(CString& pFileName);
		bool nextTo(int pPos1, int pPos2);
		void parseLevelNames(CString& pLevelNames);
		int getLevelpos(CString& pLevelname);
		static CMap* openMap(CString& pLevelName);
};


#endif
