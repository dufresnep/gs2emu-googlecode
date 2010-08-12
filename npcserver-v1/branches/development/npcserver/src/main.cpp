#include <iostream>
#include <signal.h>
using namespace std;
#include "main.h"
#include "TNPCServer.h"

// Linux specific stuff.
#if !(defined(_WIN32) || defined(_WIN64))
        #ifndef SIGBREAK
                #define SIGBREAK SIGQUIT
        #endif
#endif

// Function pointer for signal handling.
typedef void (*sighandler_t)(int);
static void getBasePath();

CString homepath;

TNPCServer * npcServer;

bool sshutdown = false;

int main(int argc, char *argv[])
{
	// Shut down the server if we get a kill signal.
	signal(SIGINT, (sighandler_t) Shutdown);
	signal(SIGTERM, (sighandler_t) Shutdown);
	signal(SIGBREAK, (sighandler_t) Shutdown);
	signal(SIGABRT, (sighandler_t) Shutdown);

	//Get the path to npcserver
	getBasePath();

	npcServer = new TNPCServer();

	if (npcServer->startServer())
	{
		while(!sshutdown) 
		{
			if (!npcServer->doMain()) break;
			sleep(100);
		}
		delete npcServer;
	}
	return 0;
}

void Shutdown()
{
	sshutdown = true;
	return;
}

const CString getHomePath()
{
	return homepath;
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






















/*
1. The npc server program consist of following instructions:

- load options
- connect to gserver
  if connection can be established:
  - load levels
  - load npcs from database
  - while the connection to the gserver is up:
    - get data from gserver;
      this contains following type of data:
      - player attributes
      - temporary level data (bombs, explosions, bush removing, items, ...)
      - PMs
      - game time
      - levels to update/reload
      - server. flags added/deleted
    - run all npc scripts
    - send changed npc attributes to the players
    - send changed player attributes to the gserver & players
    - wait till 0.1 seconds are over
*/
