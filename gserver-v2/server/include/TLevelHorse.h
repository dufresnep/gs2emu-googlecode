#ifndef TLEVELHORSE_H
#define TLEVELHORSE_H

#include "ICommon.h"
#include "CString.h"

class TLevelHorse
{
	public:
		// constructor - destructor
		TLevelHorse(const CString& pImage, float pX, float pY);

		// get private variables
		const CString& getImage();
		float getX();
		float getY();
		int getCounter();
		int getDir();

	private:
		CString image;
		float x, y;
		int counter, dir;
};


#endif // TLEVELHORSE_H
