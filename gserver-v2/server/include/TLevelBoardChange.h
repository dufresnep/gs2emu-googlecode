#ifndef TLEVELBOARDCHANGE_H
#define TLEVELBOARDCHANGE_H

#include <vector>
#include <time.h>
#include "ICommon.h"
#include "CString.h"

class TLevelBoardChange
{
	public:
		// constructor - destructor
		TLevelBoardChange(const int pX, const int pY, const int pWidth, const int pHeight,
			const CString& pTiles, const CString& pOldTiles, const int respawn = 15)
			: x(pX), y(pY), width(pWidth), height(pHeight),
			tiles(pTiles), oldTiles(pOldTiles), counter(respawn), modTime(time(0)) { }

		// functions
		CString getBoardStr(const CString ignore = "") const;
		void swapTiles();

		// get private variables
		int getX()					{ return x; }
		int getY()					{ return y; }
		int getWidth()				{ return width; }
		int getHeight()				{ return height; }
		CString getTiles()			{ return tiles; }
		int getRespawn()			{ return counter; }

		// set private variables
		void setRespawn(int respawn)	{ counter = respawn; }
		void setModTime(time_t ntime)	{ modTime = ntime; }

	private:
		int x, y, width, height;
		CString tiles, oldTiles;
		int counter;
		time_t modTime;
};

#endif // TLEVELBOARDCHANGE_H
