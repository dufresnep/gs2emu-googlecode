// GraalReborn Server - Revision 38
// (C) GraalReborn 2007

#ifndef COTHERH
#define COTHERH


#ifdef WIN32

	#include <windows.h>

#else

	#include <unistd.h>

#endif

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
	#ifdef WIN32
	Sleep(pMilliseconds);
	#else
	usleep(pMilliseconds*1000);
	#endif
}


#endif
