#include <signal.h>
#include <stdlib.h>
#include <map>
#include "main.h"
#include "ICommon.h"
#include "IUtil.h"
#include "CLog.h"
#include "CSocket.h"
#include "TServer.h"
#include "TPlayer.h"
#include "TServerList.h"

// Function pointer for signal handling.
typedef void (*sighandler_t)(int);

bool running = true;
std::map<CString, TServer *> serverList;
CLog serverlog("logs/serverlog.txt");

// Home path of the gserver.
CString homepath;
static void getBasePath();

int main(int argc, char* argv[])
{
	// Shut down the server if we get a kill signal.
	signal(SIGINT, (sighandler_t) shutdownServer);
	signal(SIGTERM, (sighandler_t) shutdownServer);

	// Seed the random number generator with the current time.
	srand((unsigned int)time(0));

	// Grab the base path to the server executable.
	getBasePath();

	serverlog.out("Starting server\n");

	// Load Server Settings
	CSettings serversettings(CString() << homepath << "servers.txt");
	if (!serversettings.isOpened())
	{
		serverlog.out("[Error] Could not open settings.txt.\n");
		return ERR_SETTINGS;
	}

	// Make sure we actually have a server.
	if (serversettings.getInt("servercount", 0) == 0)
	{
		serverlog.out("[Error] Incorrect settings.txt file.\n");
		return ERR_SETTINGS;
	}

	// Load servers.
	for (int i = 0; i < serversettings.getInt("servercount"); ++i)
	{
		CString name = serversettings.getStr(CString() << "server_" << CString(i), "default");
		TServer* server = new TServer(name);

		// Make sure doubles don't exist.
		if (serverList.find(name) != serverList.end())
			delete serverList[name];

		// Initialize the server.
		if (server->init() != 0)
		{
			delete server;
			continue;
		}
		serverList[name] = server;
	}

	// Create Packet-Functions
	createPLFunctions();
	createSLFunctions();

	// Main Loop
	serverlog.out("Main loop\n");
	while (running)
	{
		// Run each server.
		// TODO: If the server fails, try to restart it instead of deleting it.
		for (std::map<CString, TServer*>::iterator i = serverList.begin(); i != serverList.end(); )
		{
			TServer* server = (TServer*)i->second;
			if (server->doMain() == false)
			{
				delete server;
				i = serverList.erase(i);
			}
			else ++i;
		}

		// Wait
		wait(100);
	}

	// Destroy the sockets.
	//CSocket::socketSystemDestroy();

	return ERR_SUCCESS;
}

/*
	Extra-Cool Functions :D
*/

void shutdownServer(int signal)
{
	serverlog.out("Server is now shutting down...\n");
	running = false;
}


void getBasePath()
{
	#if defined(_WIN32) || defined(_WIN64)
	// Get the path.
	char path[ MAX_PATH ];
	GetModuleFileNameA(0, path, MAX_PATH);

	// Find the program exe and remove it from the path.
	// Assign the path to homepath.
	homepath = path;
	int pos = homepath.findl('\\');
	if (pos == -1) homepath.clear();
	else if (pos != (homepath.length() - 1))
		homepath.removeI(++pos, homepath.length());
#else
	// Get the path to the program.
	char path[260];
	memset((void*)path, 0, 260);
	readlink("/proc/self/exe", path, sizeof(path));

	// Assign the path to homepath.
	char* end = strrchr(path, '/');
	if (end != 0)
	{
		end++;
		if (end != 0) *end = '\0';
		homepath = path;
	}
#endif
}
