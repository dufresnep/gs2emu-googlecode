#ifndef MAIN_H
#define MAIN_H

/*
	Header-Includes
*/
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

#include "ICommon.h"
#include "IUtil.h"
#include "CString.h"

// Enums
enum
{
	SVI_SECURELOGIN		= 223,
};

enum
{
	SVO_SECURELOGIN		= 223,
};

/*
	Define Functions
*/
void doConnect();
bool doMain();
bool parsePacket(CString& pPacket);
void sendPacket(CString pPacket);
void sendCompress();

void createSVFunctions();
void shutdownClient(int signal);

bool msgSVI_NULL(CString& pPacket);
bool msgSVI_SECURELOGIN(CString& pPacket);

#endif // MAIN_H
