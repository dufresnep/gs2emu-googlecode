#ifndef TSERVERLIST_H
#define TSERVERLIST_H

#include <vector>
#include "CBuffer.h"
#include "TServer.h"

class TServerList
{
	public:
		~TServerList();

		bool inRange(int pIndex);

		TServer* get(int pIndex);
		void add(TServer *pServer);
		void clear();
		void remove(TServer *pServer);
		void remove(int pIndex);

		int count()
		{
			return (int)list.size();
		}

		TServer* operator[](int pIndex)
		{
			return get(pIndex);
		}

	private:
		std::vector<TServer *> list;
};

#endif // TSERVERLIST_H

