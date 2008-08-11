#ifndef TLEVELHORSE_H
#define TLEVELHORSE_H

#include "ICommon.h"
#include "CString.h"
#include "CTimeout.h"

class TLevelHorse
{
	public:
		// constructor - destructor
		TLevelHorse(const CString& pImage, float pX, float pY) : image(pImage), x(pX), y(pY) { timeout.setTimeout(180); }

		CString getHorseStr() const;

		// get private variables
		CString getImage() const	{ return image; }
		float getX() const			{ return x; }
		float getY() const			{ return y; }

		CTimeout timeout;

	private:
		CString image;
		float x, y;
};


#endif // TLEVELHORSE_H
