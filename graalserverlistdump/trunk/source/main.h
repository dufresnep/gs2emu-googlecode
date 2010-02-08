#ifndef MAIN_H
#define MAIN_H

#include "CString.h"
#include "CEncryption.h"
#include "CSocket.h"

class CConnection : public CSocketStub
{
	public:
		CConnection() : account("guest"), pass("7Y3bdzjf"), sock(0) {}
		~CConnection()
		{
			if (sock)
				delete sock;
		}

		bool onSend();
		bool onRecv();
		bool onRegister();
		void onUnregister();
		SOCKET getSocketHandle();
		bool canRecv();
		bool canSend();

		CSocket* socket()	{ return sock; }
		bool init();
		bool run();

		void sendPacket(const CString& pBuffer, bool appendNL = true);

	private:
		CString account;
		CString pass;
		CString rBuffer;
		CString sBuffer;
		CString oBuffer;
		CEncryption in_codec;
		CEncryption out_codec;
		CSocket* sock;
};

int main(int argc, char *argv[]);

#endif