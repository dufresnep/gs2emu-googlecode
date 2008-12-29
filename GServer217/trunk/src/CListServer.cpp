/* GraalReborn Server
	$Id$
 (C) GraalReborn 2007 */

#include "CListServer.h"
#include "main.h"
#include "zlib.h"

extern bool serverRunning;
CSocket listServer;
bool setSock = false;
bool nextIsRaw = false;
int rawPacketSize = 0;

void ListServer_Connect()
{
	if ( serverRunning == false ) return;
	if ( setSock == false )
	{
		setSock = true;
		listServer.setDescription( "listserver" );
		listServer.setProtocol( SOCKET_PROTOCOL_TCP );
		listServer.setType( SOCKET_TYPE_CLIENT );
		listServer.setOptions( SOCKET_OPTION_NONBLOCKING );
	}
	CString ip(findKey("listip")), port(findKey("listport"));
	listServer.init( ip, port );
	int val = listServer.connect();
	if (val == 0 || val == SOCKET_ALREADY_CONNECTED) lsConnected = true;
	else lsConnected = false;
	if ( !lsConnected )
	{
		errorOut("errorlog.txt", CBuffer() << "Unable to connect to list server.", true);
		listServer.disconnect();
		return;
	}

	errorOut("serverlog.txt", CBuffer() << "Connected to the list server successfully.", true);

	// send gserver info to listserver
	ListServer_Send(CPacket() << (char)SLSNAME << listServerFields[0] << "\n"
		<< (char)SLSDESC << listServerFields[1] << "\n"
		<< (char)SLSLANG << listServerFields[2] << "\n"
		<< (char)SLSVER  << listServerFields[3] << "\n"
		<< (char)SLSURL  << listServerFields[4] << "\n"
		<< (char)SLSIP   << listServerFields[5] << "\n");
	CString localip(listServer.getLocalIp());
	if (localip == CString("127.0.1.1") || localip == CString("127.0.0.1"))
		errorOut("serverlog.txt", CBuffer() << "[WARNING] Socket returned " << localip << " for its local ip!  Not sending local ip to serverlist.", true);
	else
		ListServer_Send(CPacket() << (char)SLSSETLOCALIP << listServer.getLocalIp() << "\n");
	ListServer_Send(CPacket() << (char)SLSPORT << serverPort << "\n");

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

	listServer.disconnect();
}

void ListServer_Main()
{
	static CBuffer packetBuffer;
	CStringList lines;

	if ( listServerFields[5] == "localhost" ) return;
	if (!lsConnected) return;

	// Read any new data into the socket.
	if ( listServer.getData() == -1 )
	{
		errorOut( "serverlog.txt", "Disconnected from list server." );
		lsConnected = false;
		return;
	}

	// Grab all the data from the socket buffer.
	packetBuffer << listServer.getBuffer();
	listServer.getBuffer().clear();

	// Search for a packet.  If none is found, break out of the loop.
	while (packetBuffer.length() != 0)
	{
		CPacket line;
		if (!nextIsRaw)
		{
			int lineEnd = packetBuffer.find( '\n' );
			if ( lineEnd == -1 ) return;

			// Copy the packet out and remove the \n
			line = packetBuffer.copy( 0, lineEnd + 1 );
			packetBuffer.remove(0, line.length());
			line.remove(line.length() - 1, 1);
		}
		else
		{
			if (packetBuffer.length() < rawPacketSize) return;
			line.writeBytes(packetBuffer.readChars(rawPacketSize), rawPacketSize);
			packetBuffer.remove(0, line.length());
			line.remove(line.length() - 1, 1);
			nextIsRaw = false;
		}
		packetBuffer.setRead(0);

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

					if (player->accountName.comparei(accountName))
					{
						// The serverlist will return case sensitive account names.
						// This helps case sensitive file systems open/save the correct
						// acount.
						player->accountName = accountName;
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
				CString shortName = line.readChars(line.readByte1());
				CString fileName = CString() << dataDir << "global" << fSep << shortName.text();
				CPlayer *player = (CPlayer *)playerIds[line.readByte2()];

				if (player)
				{
					player->fileList.add(new COutFile(shortName, 0));
					switch (line.readByte1())
					{
						case 0: // head
							player->headImage = shortName;
							player->updateProp(HEADGIF);
						break;

						case 1: // body
							player->bodyImage = shortName;
							player->updateProp(BODYIMG);
						break;

						case 2: // sword
							player->swordImage = shortName;
							player->updateProp(SWORDPOWER);
						break;

						case 3: // shield
							player->shieldImage = shortName;
							player->updateProp(SHIELDPOWER);
						break;
					}
				}
				break;
			}

			case GSVFILES:
			{
				CString fileData, fileName, newData, shortName;
				shortName = line.readChars(line.readByte1());
				fileName = CString() << dataDir << "global" << fSep << shortName.text();
				newData = line.readString("");

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

			case GSVFILESTART2:
			{
				CString fileData, fileName = CString() << dataDir << "global" << fSep << line.readString("");
				fileData.save(fileName.text());
				break;
			}

			case GSVFILEDATA2:
			{
				CString fileData, fileName, newData, shortName;
				shortName = line.readChars(line.readByte1());
				fileName = CString() << dataDir << "global" << fSep << shortName.text();
				newData.writeBytes(line.readChars(line.bytesLeft()), line.bytesLeft());

				fileData.load(fileName.text());
				fileData << newData;
				fileData.save(fileName.text());
				break;
			}

			case GSVFILEEND2:
			{
				CPlayer *player = (CPlayer *)playerIds[line.readByte2()];
				int type = line.readByte1();
				char doCompress = line.readByte1();
				time_t modTime = line.readByte5();
				int fileLength = line.readByte5();
				CString shortName = line.readString("");
				CString fileName = CString() << dataDir << "global" << fSep << shortName.text();

				// If the file was sent compressed, we need to uncompress it.
				if (doCompress == 1)
				{
					// Open the file so we can uncompress it.
					CString fileData;
					fileData.load(fileName.text());

					// Uncompress the file.
					char* buffer = new char[fileLength];
					memset((void*)buffer, 0, fileLength);
					int cLen = fileLength;
					int error = uncompress((Bytef*)buffer,(uLongf*)&cLen,(const Bytef*)fileData.text(), fileData.length());
					if (error != Z_OK) printf("Failed to decompress file: %s\n", shortName.text());

					// Save the file now.
					fileData.clear();
					fileData.writeBytes(buffer, cLen);
					fileData.save(fileName.text());
					delete [] buffer;
				}

				// Set the file mod time.
				if (setFileModTime(fileName.text(), modTime) == false)
					printf("** [WARNING] Could not set modification time on file %s\n", shortName.text());

				if (player)
				{
					player->fileList.add(new COutFile(shortName, 0));
					switch (type)
					{
						case 0: // head
							player->headImage = shortName;
							player->updateProp(HEADGIF);
						break;

						case 1: // body
							player->bodyImage = shortName;
							player->updateProp(BODYIMG);
						break;

						case 2: // sword
							player->swordImage = shortName;
							player->updateProp(SWORDPOWER);
						break;

						case 3: // shield
							player->shieldImage = shortName;
							player->updateProp(SHIELDPOWER);
						break;
					}
				}
				break;
			}

			case GSVPING:
				// Sent every 60 seconds, do nothing.
			break;

			case GSVRAWDATA:
				nextIsRaw = true;
				rawPacketSize = line.readByte3();
			break;

			default:
				printf("[%s] Invalid List Server Message: %i\n", getTimeStr(1).text(), messageId);
			break;
		}
	}
}

void ListServer_Send(CPacket &pPacket)
{
	if ( serverRunning == false ) return;
	if ( listServerFields[5] == "localhost" ) return;

	if (!lsConnected)
	{
		ListServer_Connect();
		if (!lsConnected)
			return;
	}

	// Make sure a newline is sent after the message.
	if ( pPacket[ pPacket.length() - 1 ] != '\n' )
		pPacket << "\n";

	if ( listServer.sendData( pPacket ) < 0 )
	{
		errorOut("serverlog.txt", "Disconnected from list server.");
		lsConnected = false;
	}
}
