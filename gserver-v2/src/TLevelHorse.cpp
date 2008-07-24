#include "TLevelHorse.h"

/*
	TLevelHorse: Constructor - Deconstructor
*/
TLevelHorse::TLevelHorse(const CString& pImage, float pX, float pY)
{
	image = pImage;
	x = pX;
	y = pY;
	counter = 180;
}

/*
	TLevelHorse: Get Private Variables
*/
const CString& TLevelHorse::getImage()
{
	return image;
}

float TLevelHorse::getX()
{
	return x;
}

float TLevelHorse::getY()
{
	return y;
}

int TLevelHorse::getCounter()
{
	return counter;
}

int TLevelHorse::getDir()
{
	return dir;
}
