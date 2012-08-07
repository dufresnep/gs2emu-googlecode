#ifndef TSOCKLIST_H
#define TSOCKLIST_H

#include <vector>
#include "CSocket.h"

class TSockList
{
	public:
		~TSockList();

		bool inRange(int pIndex);

		CSocket* get(int pIndex);
		void add(CSocket *pSocket);
		void clear();
		void remove(CSocket *pSocket);
		void remove(int pIndex);
		void run();

		CSocket* operator[](int pIndex)
		{
			return get(pIndex);
		}

	private:
		std::vector<CSocket *> list;
};

#endif // TSOCKLIST_H

