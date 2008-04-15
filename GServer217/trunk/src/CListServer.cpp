/* GraalReborn Server
    $Id$
 (C) GraalReborn 2007 */

#include "CListServer.h"
#include "main.h"

CSocket listServer;

void ListServer_Connect()
{
        if ((lsConnected = listServer.connectSock(findKey("listip"), atoi(findKey("listport")))) == false)
        {
            errorOut("errorlog.txt", "Unable to connect to list server.", true);
            return;
        }

        errorOut("serverlog.txt", "Connected to the list server successfully.", true);
        listServer.setSync(false);

        // send gserver info to listserver
        ListServer_Send(CPacket() << (char)SLSNAME << listServerFields[0]  << "\n" << (char)SLSDESC << listServerFields[1]  << "\n" << (char)SLSLANG << listServerFields[2]  << "\n" << (char)SLSVER  << listServerFields[3]  << "\n" << (char)SLSURL  << listServerFields[4]  << "\n" << (char)SLSIP   << listServerFields[5]  << "\n" << (char)SLSPORT << toString(serverPort) << "\n");

        // send players to listserver
        CPacket pPacket;
        for (int i = 0; i < playerList.count(); i++)
        {
            CPlayer *player = (CPlayer *)playerList[i];
            pPacket << (char)player->accountName.length() << player->accountName << player->getProp(NICKNAME) << player->getProp(CURLEVEL) << player->getProp(PLAYERX) << player->getProp(PLAYERY) << player->getProp(PALIGNMENT) << (char)player->type;
        }

        ListServer_Send(CPacket() << (char)SLSCOUNT << (char)playerList.count() << pPacket << "\n");
}

void ListServer_End()
{
	if ( listServerFields[5] == "localhost" ) return;
	if (!lsConnected) return;

    listServer.closeSock();
}

void ListServer_Main()
{
	if ( listServerFields[5] == "localhost" ) return;
    if (!lsConnected) return;

	static CBuffer packetBuffer;
	CBuffer receiveBuff;
	if (listServer.receiveBytes(receiveBuff, 65536) < 0)
	{
		errorOut("serverlog.txt", "Disconnected from list server.");
		lsConnected = false;
		return;
	}
	packetBuffer << receiveBuff;

	// Loop grabbing all the packets we can.
	CStringList lines;
	while ( packetBuffer.length() > 0 )
	{
		// Search for a packet.  If none is found, break out of the loop.
		int lineEnd = packetBuffer.find( '\n' );
		if ( lineEnd == -1 ) break;

		// Copy the packet out and remove the \n
		CBuffer line( packetBuffer.copy(0, lineEnd + 1) );
		line.removeAll( "\n" );
		packetBuffer.remove(0, lineEnd + 1);

		// Add the packet to the list of packets to parse.
		lines.add( line );
	}

	for (int i = 0; i < lines.count(); i++)
	{
		CPacket line = CPacket() << lines[i];
		int messageId = line.readByte1();

		switch (messageId)
		{
			case GSVOLD:
			{
				printf("[%s] SERVER VERSION CHECK - Current: %i, Latest: %i - Old version, please upgrade.\n", getTimeStr(1).text(), GSERVER_BUILD, line.readByte2());
				break;
			}

			case GSVCURRENT:
			{
				printf("[%s] SERVER VERSION CHECK - Current: %i, Latest: %i - You are up to date :)\n", getTimeStr(1).text(), GSERVER_BUILD, GSERVER_BUILD);
				break;
			}

			case GSVACCOUNT:
			{
				CString accountName = line.readChars(line.readByte1());
				CString errorMsg = line.readString("");

				for (int i = 0; i < newPlayers.count(); i++)
				{
					CPlayer *player = (CPlayer *)newPlayers[i];

					if (player->accountName == accountName)
					{
						if (errorMsg == "SUCCESS")
						{
							player->sendAccount();
						}
						else
						{
							player->sendPacket(CPacket() << (char)DISMESSAGE << errorMsg);
							player->deleteMe = true;
						}

						break;
					}
				}

				break;
			}

			case GSVGUILD:
			{
				int playerId = line.readByte2();
				CPlayer *player = (CPlayer *)playerIds[playerId];

				if (player != NULL)
				{
					CString nick = line.readChars((unsigned char)line.readByte1());
					CString guild = nick.copy( nick.findl( '(' ) ).remove( ")" );

					if ( globalGuilds == false )
					{
						if ( globalGuildList.find( guild ) != -1 )
							player->setNick(nick, false);
					}
					else
						player->setNick(nick, false);
				}

				break;
			}

			case GSVFILEC:
			{
				CString fileData, fileName = CString() << dataDir << "global" << fSep << line.readChars(line.readByte1());
				fileData.save(fileName.text());

				break;
			}

			case GSVFILED:
			{
				CString fileName = line.readChars(line.readByte1());
				CPlayer *player = (CPlayer *)playerIds[line.readByte2()];

				switch (line.readByte1())
				{
					case 0: // head
						player->headImage = fileName;
						player->updateProp(HEADGIF);
					break;

					case 1: // body
						player->bodyImage = fileName;
						player->updateProp(BODYIMG);
					break;

					case 2: // sword
						player->swordImage = fileName;
						player->updateProp(SWORDPOWER);
					break;

					case 3: // shield
						player->shieldImage = fileName;
						player->updateProp(SHIELDPOWER);
					break;
				}

				break;
			}

			case GSVFILES:
			{
				CString fileData, fileName, newData, shortName;
				shortName = line.readChars(line.readByte1());
				int pos = shortName.find("Revision");

				if (pos >= 0)
				{
					#ifdef WIN32
						fileName = CString() << "GServer-NEW.exe";
					#else
						fileName = CString() << "GServer-NEW";
					#endif
					newData = line.readString("");
				}
				else
				{
					fileName = CString() << dataDir << "global" << fSep << shortName.text();
					newData = line.readString("");
				}

				fileData.load(fileName.text());
				fileData << newData.B64_Decode();
				fileData.save(fileName.text());

				break;
			}

			case GSVPROFILE: /* Unsure if this works, temp */
			{
				CPacket profile;
				CPlayer *player1 = (CPlayer *)playerIds[line.readByte2()];
				CPlayer *player2 = findPlayerId(line.readChars(line.readByte1()));
				if (player1 == NULL || player2 == NULL)
					return;

				profile << (char)player2->accountName.length() << player2->accountName << line.readString("");

				int time = player2->onlineSecs;
				CString line2;
				//Online time
				line2 << toString((int)time/3600) << " hrs ";
				line2 << toString((int)(time/60)%60) << " mins ";
				line2 << toString((int)time%60) << " secs";
				profile << (char)line2.length() << line2;

				for (int i = 0; i < profileList.count(); i++)
				{
					CStringList a;
					a.load(profileList[i].text(), ":=");
					if (a[0].length() < 1)
						continue;

					CString n;

					if (a[1] == "playerkills")
						n = toString(player2->kills);
					else if (a[1] == "playerdeaths")
						n = toString(player2->deaths);
					else if (a[1] == "playerfullhearts")
					{
						if ( (float)(int)player2->maxPower == (float)player2->maxPower )
							n = toString((int)player2->maxPower);
						else
						{
							n = toString(player2->maxPower);
							n = n.copy( 0, n.length() - 1 );
						}
					}
					else if (a[1] == "playerrating")
						n = toString((int)player2->rating) << "/" << toString((int)player2->deviation);
					else if (a[1] == "playerap")
						n = toString(player2->ap);
					else if (a[1] == "playerrupees")
						n = toString(player2->rubins);
					else if (a[1] == "playerswordpower")
						n = toString(player2->swordPower);
					else if (a[1] == "canspin")
						n = (player2->status & 64 ? "true" : "false");
					else if (a[1] == "playerhearts")
					{
						if ( (float)(int)player2->power == (float)player2->power )
							n = toString((int)player2->power);
						else
						{
							n = toString(player2->power);
							n = n.copy( 0, n.length() - 1 );
						}
					}
					else if (a[1] == "playerdarts")
						n = toString(player2->darts);
					else if (a[1] == "playerbombs")
						n = toString(player2->bombs);
					else if (a[1] == "playermp")
						n = toString(player2->magicPoints);
					else if (a[1] == "playershieldpower")
						n = toString(player2->shieldPower);
					else if (a[1] == "playerglovepower")
						n = toString(player2->glovePower);
					else
					{
						for (int i = 0; i < player2->myFlags.count(); i++)
						{
							CStringList b;
							b.load(player2->myFlags[i].text(), "=");
							if (b[0] == a[1])
							{
								n = b[1];
								break;
							}
						}
					}

					profile << (char)(a[0].length() + n.length() + 2) << a[0] << ":=" << n;
				}

				player1->sendPacket(CPacket() << (char)DPROFILE << profile);

				break;
			}

			case GSVMSG:
				printf("[%s] %s\n", getTimeStr(1).text(), line.readString(""));
			break;

			case GSVPING:
				// Sent every 60 seconds, do nothing.
			break;

			default:
				printf("[%s] Invalid List Server Message: %i\n", getTimeStr(1).text(), messageId);
			break;
		}
	}
}

void ListServer_Send(CPacket &pPacket)
{
	if ( listServerFields[5] == "localhost" ) return;

	if (!lsConnected)
    {
        ListServer_Connect();
        if (!lsConnected)
            return;
    }

    listServer.sendBuffer(pPacket);
}
