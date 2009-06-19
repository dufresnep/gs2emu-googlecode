#include "main.h"

/*
	Notes:
		- NPC-Server connects like a regular graal player,
		  so therefore we need to tweak the login packet a
		  bit with maybe a special key to connect with similar
		  to anope services with ircd. 3 keys that the gserver
		  is expecting or whatever, should be kept private so
		  it would go under the private server options section
		  or whatever.. throwing things out there, you know.

		- NPC-Server loads levels and npcs on start, but it's
		  also possible for the npc-server to load levels
		  while the server is running, you know.. incase say
		  someone uploads a new level? dur! throwing things out
		  there right now lol.
*/

int main(int argc, char *argv[])
{
	// load options.. skip!

	// connect to gserver.. skip!

	// load levels.. skip!

	// load npcs from database.. skip!

	// while the connection to the gserver is up
	while (true) // gserver.isconnected() or something..
	{
		// get data from gserver.. skip!

		// run all npc scripts.. skip!

		// send changed npc attributes to the players.. skip!
		
		// send changed player attributes to the gserver & players.. skip!

		// wait till 0.1 seconds are over
		sleep(100);
	}

	// finished..
	return 0;
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
