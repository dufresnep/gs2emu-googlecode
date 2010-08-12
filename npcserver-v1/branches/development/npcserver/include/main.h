#ifndef MAIN_H
#define MAIN_H

/*
	Header-Includes
*/
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "CString.h"
#include "IUtil.h"
#include "CSocket.h"
#include "CSettings.h"


/*
	Define Functions
*/
bool doMain();
void Shutdown();
const CString getHomePath();
//void shutdownServer(int signal);

#endif // MAIN_H
