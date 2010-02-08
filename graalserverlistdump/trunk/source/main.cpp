#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#	define WINDOWS
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <windows.h>
#else
#	include <unistd.h>
#endif

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "main.h"
#include "CEncryption.h"
#include "CSocket.h"
#include "CString.h"

bool Daemon = false;
bool SimpleMode = false;
bool Verbose = false;
CString OutputFile("serverlist.txt");

static void PrintHelp(const char* program_name);

int main(int argc, char *argv[])
{
	std::cout << "Graal Online Serverlist Dump Utility." << std::endl;
	std::cout << "Written by Nalin." << std::endl << std::endl;

	// Arguments!
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (strncmp("--", argv[i], 2) == 0)
			{
				if (strcmp("simple", argv[i] + 2) == 0)
					SimpleMode = true;
				else if (strcmp("daemon", argv[i] + 2) == 0)
					Daemon = true;
				else if (strcmp("verbose", argv[i] + 2) == 0)
					Verbose = true;
				else if (strcmp("file", argv[i] + 2) == 0 && (i + 1) < argc)
				{
					OutputFile = argv[i + 1];
					++i;
				}
				else if (strcmp("help", argv[i] + 2) == 0)
				{
					PrintHelp(argv[0]);
					return 0;
				}
			}
			else if (strncmp("-", argv[i], 1) == 0)
			{
				unsigned int slen = strlen(argv[i]);
				for (unsigned int j = 1; j < slen; ++j)
				{
					char o = *(argv[i] + j);
					switch (o)
					{
						case 's':
							SimpleMode = true;
							break;
						case 'd':
							Daemon = true;
							break;
						case 'v':
							Verbose = true;
							break;
						case 'f':
							if ((i + 1) < argc)
							{
								OutputFile = argv[i + 1];
								j = 0xFFFFFFFF - 1;
								++i;
							}
							break;
						default:
						case 'h':
							PrintHelp(argv[0]);
							return 0;
					}
				}
			}
		}
	}

	CSocketManager mgr;
	CConnection c;
	if (!c.init())
		return 1;

	do
	{
		// Register our class with the socket manager.
		if (Verbose) std::cout << "Registering with the socket manager." << std::endl;
		mgr.registerSocket(&c);
		mgr.update();

		// Run program.
		while (!mgr.empty())
			mgr.update(1, 0);

		if (!Verbose)
			std::cout << "Disconnected." << std::endl;

		if (Daemon)
		{
			#ifdef WINDOWS
			//Sleep(300000);
			Sleep(60000);
			#else
			sleep(300);
			#endif

			if (!c.init())
				Daemon = false;
		}
	}
	while (Daemon);

	return 0;
}

bool CConnection::init()
{
	rBuffer.clear();
	oBuffer.clear();
	sBuffer.clear();

	// Randomize the timer.
	std::srand(std::time(0));

	// Determine the key.
	unsigned char key = std::rand() % 220;
	if (Verbose) std::cout << "Key: " << (unsigned int)key << std::endl;

	// Initialize the encryption engine.
	in_codec.setGen(ENCRYPT_GEN_5);
	out_codec.setGen(ENCRYPT_GEN_5);
	in_codec.reset(key);
	out_codec.reset(key);

	// Initialize the socket.
	if (sock == 0)
	{
		sock = new CSocket();
		sock->setDescription("sock");
		sock->setType(SOCKET_TYPE_CLIENT);
		sock->setProtocol(SOCKET_PROTOCOL_TCP);
	}
	if (sock->init("listserver.graalonline.com", "14922") != SOCKET_OK)
	{
		std::cout << "Error: Could not initialize socket to listserver.graalonline.com." << std::endl;
		return false;
	}

	// Connect the socket.
	std::cout << "Logging into serverlist." << std::endl;
	if (sock->connect() != SOCKET_OK)
	{
		std::cout << "Error: Could not connect!" << std::endl;
		return false;
	}

	// Set up our login packets.
	CString keyPacket, accountPacket;
	keyPacket >> (char)7 >> (char)key << "GNW30123newmain" << "\n";
	accountPacket >> (char)1 >> (char)account.length() << account >> (char)pass.length() << pass;

	// Compress the encryption detail packet and add it to the out buffer.
	keyPacket.zcompressI();
	oBuffer << (short)keyPacket.length() << keyPacket;

	// Add the login packet to the send buffer.
	sendPacket(accountPacket);

	return true;
}

bool CConnection::onSend()
{
	if (sock == 0 || sock->getState() == SOCKET_STATE_DISCONNECTED)
	{
		if (Verbose) std::cout << "Sending while socket disconnected!" << std::endl;
		return false;
	}

	int compressionType = COMPRESS_UNCOMPRESSED;
	if (sBuffer.length() > 0x2000)	// 8KB
	{
		compressionType = COMPRESS_BZ2;
		sBuffer.bzcompressI();
	}
	else if (sBuffer.length() > 55)
	{
		compressionType = COMPRESS_ZLIB;
		sBuffer.zcompressI();
	}

	// Sanity check.
	if (sBuffer.length() > 0xFFFC)
	{
		if (Verbose) printf("** [ERROR] Trying to send a GEN_5 packet over 65532 bytes!  Tossing data.\n");
		return false;
	}

	// Encrypt the packet and add it to the out buffer.
	out_codec.limitFromType(compressionType);
	sBuffer = out_codec.encrypt(sBuffer);
	CString data = CString() << (short)(sBuffer.length() + 1) << (char)compressionType << sBuffer;
	oBuffer << data;
	unsigned int dsize = oBuffer.length();
	int size = sock->sendData(oBuffer.text(), &dsize);
	oBuffer.removeI(0, size);
	sBuffer.clear();

	return true;
}

bool CConnection::onRecv()
{
	if (sock == 0 || sock->getState() == SOCKET_STATE_DISCONNECTED)
	{
		if (Verbose) std::cout << "Receiving while socket disconnected!" << std::endl;
		return false;
	}

	unsigned int size = 0;
	char* data = sock->getData(&size);
	if (size != 0)
		rBuffer.write(data, size);
	else if (sock->getState() == SOCKET_STATE_DISCONNECTED)
	{
		if (Verbose) std::cout << "Socket disconnected in onRecv()." << std::endl;
		return false;
	}

	return run();
}

bool CConnection::onRegister()
{
	return true;
}

void CConnection::onUnregister()
{
	return;
}

SOCKET CConnection::getSocketHandle()
{
	if (sock)
		return sock->getHandle();
	else return ((SOCKET)-1);
}

bool CConnection::canRecv()
{
	return true;
}

bool CConnection::canSend()
{
	if (!sBuffer.isEmpty())
		return true;
	return false;
}


bool CConnection::run()
{
	// See if we can grab any packets.
	rBuffer.setRead(0);
	while (rBuffer.length() > 1)
	{
		unsigned short plen = rBuffer.readShort();
		if (plen > (unsigned short)rBuffer.length() - 2)
			break;

		CString unBuffer = rBuffer.readChars(plen);
		rBuffer.removeI(0, plen + 2);

		// Compression
		unsigned char ptype = (unsigned char)unBuffer.readChar();
		unBuffer.removeI(0, 1);

		// Decrypt data
		if (in_codec.limitFromType(ptype))
		{
			if (Verbose) std::cout << "** Packet compression type is not supported." << std::endl;
			return false;
		}
		in_codec.decrypt(unBuffer);

		// Uncompress if needed.
		const char* compression = "uncompressed";
		if (ptype == COMPRESS_ZLIB)
		{
			unBuffer.zuncompressI();
			compression = "zlib";
		}
		else if (ptype == COMPRESS_BZ2)
		{
			unBuffer.bzuncompressI();
			compression = "bz2";
		}

		if (Verbose)
		{
			std::cout << std::endl;// << std::endl;
			std::cout << "~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
			std::cout << "Chunk Length: " << plen << ", data: " << unBuffer.length() << std::endl;
			std::cout << "Chunk Compression: " << compression << " (" << (unsigned int)ptype << ")" << std::endl;
			std::cout << std::endl;
		}

		int i = 0;
		while (!unBuffer.isEmpty())
		{
			unBuffer.setRead(0);
			CString unBuffer2 = unBuffer.readString("\n");
			unBuffer.removeI(0, unBuffer2.length() + 1);

			unsigned char pid = unBuffer2.readGUChar();

			if (Verbose)
				std::cout << "Packet " << i++ << ": " << (unBuffer2.text() + 1) << std::endl;

			if (pid == 0)
			{
				FILE* f = fopen(OutputFile.text(), "wb");
				if (f)
				{
					if (!Verbose)
						std::cout << "Got serverlist, writing to " << OutputFile.text() << std::endl;

					unsigned char server_count = unBuffer2.readGUChar();
					for (int j = 0; j < server_count; ++j)
					{
						unsigned char type = unBuffer2.readGUChar();
						CString name = unBuffer2.readChars(unBuffer2.readGUChar());
						CString language = unBuffer2.readChars(unBuffer2.readGUChar());
						CString description = unBuffer2.readChars(unBuffer2.readGUChar());
						CString url = unBuffer2.readChars(unBuffer2.readGUChar());
						CString version = unBuffer2.readChars(unBuffer2.readGUChar());
						CString player_count = unBuffer2.readChars(unBuffer2.readGUChar());
						CString ip = unBuffer2.readChars(unBuffer2.readGUChar());
						CString port = unBuffer2.readChars(unBuffer2.readGUChar());

						if (!SimpleMode)
						{
							fprintf(f, "----------------------------------\r\n");
							fprintf(f, "Type: %d\r\n", type);
							fprintf(f, "Name: %s\r\n", name.text());
							fprintf(f, "Language: %s\r\n", language.text());
							fprintf(f, "Description: %s\r\n", description.text());
							fprintf(f, "URL: %s\r\n", url.text());
							fprintf(f, "Version: %s\r\n", version.text());
							fprintf(f, "Player count: %s\r\n", player_count.text());
							fprintf(f, "IP: %s\r\n", ip.text());
							fprintf(f, "Port: %s\r\n", port.text());
						}
						else fprintf(f, "%s\r\n", name.text());
					}
					fclose(f);
				}
			}
		}

		if (Verbose)
			std::cout << "~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	}

	return true;
}

void CConnection::sendPacket(const CString& pBuffer, bool appendNL)
{
	// Is Data?
	if (pBuffer.isEmpty())
		return;

	// Add to buffer.
	sBuffer << pBuffer;

	// Append '\n'.
	if (appendNL)
	{
		if (pBuffer[pBuffer.length()-1] != '\n')
			sBuffer << "\n";
	}
}

void PrintHelp(const char* program_name)
{
	std::cout << "Usage: " << program_name << " [options]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -h,--help\t\tThis message." << std::endl;
	std::cout << "  -d,--daemon\t\tRuns the program in daemon mode." << std::endl;
	std::cout << "  -s,--simple\t\tRuns the program in simple mode." << std::endl;
	std::cout << "  -f,--file filename\tSets the output file. (default: serverlist.txt)" << std::endl;
	std::cout << "  -v,--verbose\t\tEnables console spam." << std::endl;
	std::cout << std::endl;
}
