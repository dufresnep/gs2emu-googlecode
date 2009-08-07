#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include "main.h"
#include "IUtil.h"
#include "CFileSystem.h"
#include "TLevel.h"

// Linux specific stuff.
#if !(defined(_WIN32) || defined(_WIN64))
	#ifndef SIGBREAK
		#define SIGBREAK SIGQUIT
	#endif
#endif

// Function pointer for signal handling.
typedef void (*sighandler_t)(int);

CFileSystem fs;
std::map<CString, int> images;
std::map<CString, int> levels;
FILE* f = 0;

// Home path of the gserver.
CString homepath;
static void getBasePath();

int main(int argc, char* argv[])
{
	// Shut down the server if we get a kill signal.
	signal(SIGINT, (sighandler_t) shutdownServer);
	signal(SIGTERM, (sighandler_t) shutdownServer);
	signal(SIGBREAK, (sighandler_t) shutdownServer);
	signal(SIGABRT, (sighandler_t) shutdownServer);

	// Grab the base path to the server executable.
	getBasePath();

	// Program announcements.
	printf("Graal image search.\nProgrammed by Nalin.\n\n");

	// Create the file system.
	printf("Scanning directories... ");
	fs.addDir("");
	printf("%d files found.\n", fs.getFileList()->size());

	// Search for all levels.
	printf("Searching levels... ");
	float count = 0;
	float max = fs.getFileList()->size();
	for (std::map<CString, CString>::iterator i = fs.getFileList()->begin(); i != fs.getFileList()->end(); ++i)
	{
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bSearching levels... %d%c", (char)(((++count) / max) * 100), '%');
		//printf("\x1b[1FSearching levels... %d%c\n", (char)(((++count) / max) * 100), '%');
		CString ext = getExtension(i->first);
		if (ext == ".nw" || ext == ".graal" || ext == ".zelda")
		{
			TLevel l;
			l.loadLevel(i->second, images, levels);
		}
	}
	printf("\n");

	// Save all found images.
	printf("Saving all found images... ");
	f = fopen((CString(homepath) << "allimages.txt").text(), "wb");
	if (f)
	{
		for (std::map<CString, int>::iterator i = images.begin(); i != images.end();)
		{
			fprintf(f, "%s\r\n", i->first);
			if (!fs.find(i->first).isEmpty())
				images.erase(i++);
			else ++i;
		}
		fclose(f);
	}
	printf("done.\n");

	// Save all found levels.
	printf("Saving all found levels... ");
	f = fopen((CString(homepath) << "alllevels.txt").text(), "wb");
	if (f)
	{
		for (std::map<CString, int>::iterator i = levels.begin(); i != levels.end();)
		{
			fprintf(f, "%s\r\n", i->first);
			if (!fs.find(i->first).isEmpty())
				levels.erase(i++);
			else ++i;
		}
		fclose(f);
	}
	printf("done.\n");

	// Save all missing images.
	printf("Saving all missing images... ");
	f = fopen((CString(homepath) << "missingimages.txt").text(), "wb");
	if (f)
	{
		for (std::map<CString, int>::iterator i = images.begin(); i != images.end(); ++i)
		{
			fprintf(f, "%s\r\n", i->first);
		}
		fclose(f);
	}
	printf("done.\n");

	// Save all missing levels.
	printf("Saving all missing levels... ");
	f = fopen((CString(homepath) << "missinglevels.txt").text(), "wb");
	if (f)
	{
		for (std::map<CString, int>::iterator i = levels.begin(); i != levels.end(); ++i)
		{
			fprintf(f, "%s\r\n", i->first);
		}
		fclose(f);
	}
	printf("done.\n");

	return 0;
}

/*
	Extra-Cool Functions :D
*/
const CString getHomePath()
{
	return homepath;
}

void shutdownServer(int sig)
{
	printf("\nAborting...\n");
	if (f) fclose(f);
	exit(0);
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
