#ifndef IUTIL_H
#define IUTIL_H

#include "CString.h"
#include <map>
#include <vector>

#define inrange(a, b, c) ((a) >= (b) && (a) <= (c))

#ifndef sleep
	#if defined(_WIN32) || defined(_WIN64)
		#define sleep(a) Sleep(a)
	#else
		#define sleep(a) usleep(a*1000)
	#endif
#endif

/*
	Map-Functions
*/
template <class T, class U>
bool mapRemove(std::map<T, U>& a, T b)
{
	typename std::map<T, U>::iterator i;
	for (i = a.begin(); i != a.end(); ++i)
	{
		if (i->first == b)
		{
			a.erase(i);
			return true;
		}
	}
	return false;
}

template <class T, class U>
bool mapRemove(std::map<T, U>& a, U b)
{
	typename std::map<T, U>::iterator i;
	for (i = a.begin(); i != a.end(); ++i)
	{
		if (i->second == b)
		{
			a.erase(i);
			return true;
		}
	}
	return false;
}

/*
	Vector-Functions
*/
template <class T>
int vecSearch(std::vector<T>& a, T b)
{
	for (unsigned int i = 0; i < a.size(); ++i)
	{
		if (a[i] == b)
			return i;
	}
	return -1;
}

template <class T>
bool vecRemove(std::vector<T>& a, T b)
{
	typename std::vector<T>::iterator i;
	for (i = a.begin(); i != a.end(); ++i)
	{
		if (*i == b)
		{
			a.erase(i);
			return true;
		}
	}
	return false;
}

template <class T>
int vecReplace(std::vector<T>& a, void *b, void *c)
{
	for (unsigned int i = 0; i < a.size(); i++)
	{
		if (a[i] == b)
		{
			a[i] = (T)c;
			return i;
		}
	}

	return -1;
}

CString removeExtension(const CString& file);

#endif
