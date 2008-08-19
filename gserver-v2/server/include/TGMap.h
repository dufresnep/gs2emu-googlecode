#ifndef TGMAP_H
#define TGMAP_H

#include <map>
#include "CString.h"

struct SGMapLevel
{
	SGMapLevel() : gmapx(-1), gmapy(-1) {}
	SGMapLevel(int x, int y) : gmapx(x), gmapy(y) {}

	SGMapLevel& operator=(const SGMapLevel& level)
	{
		gmapx = level.gmapx;
		gmapy = level.gmapy;
		return *this;
	}

	int gmapx;
	int gmapy;
};

class TServer;
class TGMap
{
	public:
		TGMap();
		TGMap(const CString& filename, TServer* server);

		bool load(const CString& filename, TServer* server);

		bool isLevelOnGMap(const CString& level) const;

		CString getLevelAt(int x, int y) const;
		int getLevelX(const CString& level) const;
		int getLevelY(const CString& level) const;
		CString getMapName() const			{ return mapName; }

	private:
		CString mapName;
		time_t modTime;
		int width;
		int height;
		CString mapImage;
		CString miniMapImage;
		//bool loadFullMap;
		std::map<CString, SGMapLevel> levels;
};

#endif
