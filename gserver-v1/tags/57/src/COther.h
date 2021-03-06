/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#ifndef COTHERH
#define COTHERH


#if defined(WIN32)
	#include <windows.h>
#elif defined(PSPSDK)
	#include <pspthreadman.h>
#else
	#include <unistd.h>
#endif

#include "CBuffer.h"

inline bool CHECK_BOOL(char *a)
{
	return (stricmp(a, "true") == 0 || stricmp(a, "1") == 0);
}

inline int MIN(int a, int b)
{
	return (a < b? a : b);
}
inline int MAX(int a, int b)
{
	return (a < b? b : a);
}

inline float MIN(float a, float b)
{
	return (a < b? a : b);
}
inline float MAX(float a, float b)
{
	return (a < b? b : a);
}

inline int CLIP(int value, int min, int max)
{
	return MAX(MIN(value, max), min);
}

inline float CLIP(float value, float min, float max)
{
	return MAX(MIN(value, max), min);
}

inline void wait(int pMilliseconds)
{
	#if defined(WIN32)
		Sleep(pMilliseconds);
	#elif defined(PSPSDK)
		sceKernelDelayThread(pMilliseconds*1000);
	#else
		usleep(pMilliseconds*1000);
	#endif
}


#endif
