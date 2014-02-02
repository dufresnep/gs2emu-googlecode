using System;
using System.Collections.Generic;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;
using OpenGraal.Common.Players;

namespace OpenGraal.GraalIM
{
	public class GServerConnection : CSocket
	{
		/// <summary>
		/// Enumerator -> Packet In
		/// </summary>
		public enum PacketIn
		{
			LEVELBOARD = 0,
			LEVELLINK = 1,
			BADDYPROPS = 2,
			NPCPROPS = 3,
			LEVELCHEST = 4,
			LEVELSIGN = 5,
			LEVELNAME = 6,
			BOARDMODIFY = 7,
			OTHERPLPROPS = 8,
			PLAYERPROPS = 9,
			ISLEADER = 10,
			BOMBADD = 11,
			BOMBDEL = 12,
			TOALL = 13,
			PLAYERWARP = 14,
			WARPFAILED = 15,
			DISCMESSAGE = 16,
			HORSEADD = 17,
			HORSEDEL = 18,
			ARROWADD = 19,
			FIRESPY = 20,
			THROWCARRIED = 21,
			ITEMADD = 22,
			ITEMDEL = 23,
			NPCMOVED = 24,
			// What does this do?
			SIGNATURE = 25,
			NPCACTION = 26,
			// What does this do?
			BADDYHURT = 27,
			FLAGSET = 28,
			NPCDEL = 29,
			FILESENDFAILED = 30,
			FLAGDEL = 31,
			SHOWIMG = 32,
			NPCWEAPONADD = 33,
			NPCWEAPONDEL = 34,
			RC_ADMINMESSAGE = 35,
			EXPLOSION = 36,
			PRIVATEMESSAGE = 37,
			PUSHAWAY = 38,
			// What does this do?
			LEVELMODTIME = 39,
			HURTPLAYER = 40,
			STARTMESSAGE = 41,
			NEWWORLDTIME = 42,
			DEFAULTWEAPON = 43,
			HASFramework = 44,
			// If sent, the client won't update npc props.
			FILEUPTODATE = 45,
			HITOBJECTS = 46,
			STAFFGUILDS = 47,
			TRIGGERACTION = 48,
			PLAYERWARP2 = 49,
			// Bytes 1-3 are x/y/z. 4 = level x in gmap, 5 = level y in gmap.
			RC_ACCOUNTADD = 50,
			// Deprecated
			RC_ACCOUNTSTATUS = 51,
			// Deprecated
			RC_ACCOUNTNAME = 52,
			// Deprecated
			RC_ACCOUNTDEL = 53,
			// Deprecated
			RC_ACCOUNTPROPS = 54,
			// Deprecated
			ADDPLAYER = 55,
			DELPLAYER = 56,
			RC_ACCOUNTPROPSGET = 57,
			// Deprecated
			RC_ACCOUNTCHANGE = 58,
			// Deprecated
			RC_PLAYERPROPSCHANGE = 59,
			// Deprecated
			UNKNOWN60 = 60,
			RC_SERVERFLAGSGET = 61,
			RC_PLAYERRIGHTSGET = 62,
			RC_PLAYERCOMMENTSGET = 63,
			RC_PLAYERBANGET = 64,
			RC_FILEBROWSER_DIRLIST = 65,
			RC_FILEBROWSER_DIR = 66,
			RC_FILEBROWSER_MESSAGE = 67,
			LARGEFILESTART = 68,
			LARGEFILEEND = 69,
			RC_ACCOUNTLISTGET = 70,
			RC_PLAYERPROPS = 71,
			// Deprecated
			RC_PLAYERPROPSGET = 72,
			RC_ACCOUNTGET = 73,
			RC_CHAT = 74,
			PROFILE = 75,
			RC_SERVEROPTIONSGET = 76,
			RC_FOLDERCONFIGGET = 77,
			NC_CONTROL = 78,
			// HIJACKED BY GR!  No clue as to its official purpose.
			FrameworkADDR = 79,
			// Bytes 1-2 are 0 and 2, followed by a string formatted as <ipaddr>,<port>.
			NC_LEVELLIST = 80,
			// {80}{GSTRING levels}
			UNKNOWN81 = 81,
			SERVERTEXT = 82,
			// Answer to PLI_REQUESTTEXT and PLI_SENDTEXT.
			UNKNOWN83 = 83,
			LARGEFILESIZE = 84,
			RAWDATA = 100,
			// {100}{INT3 length}
			BOARDPACKET = 101,
			FILE = 102,
			UNKNOWN = 103,
			NPCBYTECODE = 131,
			// Compiled Torque-script for an NPC. {131}{INT3 id}{code}
			UNKNOWN134 = 134,
			// Might be used for package downloads.
			NPCWEAPONSCRIPT = 140,
			// {140}{INT2 info_length}{script}
			NPCDEL2 = 150,
			// {150}{CHAR level_length}{level}{INT3 npcid}
			HIDENPCS = 151,
			SAY2 = 153,
			// Also used for signs. {153}{text}
			FREEZEPLAYER2 = 154,
			// Blank.
			UNFREEZEPLAYER = 155,
			// Blank.
			SETACTIVELEVEL = 156,
			// Sets the level to receive chests, baddies, NPCs, etc.
			NC_NPCATTRIBUTES = 157,
			// {157}{GSTRING attributes}
			NC_NPCADD = 158,
			// {158}{INT id}{CHAR 50}{CHAR name length}{name}{CHAR 51}{CHAR type length}{type}{CHAR 52}{CHAR level length}{level}
			NC_NPCDELETE = 159,
			// {159}{INT id}
			NC_NPCSCRIPT = 160,
			// {160}{INT id}{GSTRING script}
			NC_NPCFLAGS = 161,
			// {161}{INT id}{GSTRING flags}
			NC_CLASSGET = 162,
			// {162}{CHAR name length}{name}{GSTRING script}
			NC_CLASSADD = 163,
			// {163}{class}
			NC_LEVELDUMP = 164,
			MOVE = 165,
			NC_WEAPONLISTGET = 167,
			// {167}{CHAR name1 length}{name1}{CHAR name2 length}{name2}...
			UNKNOWN168 = 168,
			// Login server sends this.  Blank packet.
			GHOSTMODE = 170,
			BIGMAP = 171,
			MINIMAP = 172,
			// [172] zodiacminimap.txt,zodiacworldminimap3.png,10,10
			GHOSTTEXT = 173,
			// {173}{text}  Shows static text in lower-right corner of screen only when in ghost mode.
			GHOSTICON = 174,
			// Pass 1 to enable the ghost icon
			SHOOT = 175,
			FULLSTOP = 176,
			// Sending this causes the entire client to not respond to normal input and it hides the HUD.
			FULLSTOP2 = 177,
			// Sending this causes the entire client to not respond to normal input and it hides the HUD.
			SERVERWARP = 178,
			RPGWINDOW = 179,
			STATUSLIST = 180,
			LISTPROCESSES = 182,
			NC_CLASSDELETE = 188,
			// {188}{class}
			MOVE2 = 189,
			// {189}{INT id}...
			UNKNOWN190 = 190,
			// Was blank.  Sent before weapon list.
			NC_WEAPONGET = 192,
			// {192}{CHAR name length}{name}{CHAR image length}{image}{script}
			UNKNOWN194 = 194,
			// Was blank.  Sent before weapon list.
			UNKNOWN195 = 195,
			// Something to do with ganis.  [195] )twiz-icon"SETBACKTO "
			// Seems to register NPCs or something on the client.
			// Also is related to PLI_UPDATESCRIPT as it sends the last modification time of the NPC/weapon.  The v5 client stores weapon scripts offline.
			UNKNOWN197 = 197,
			// Seems to register npcs on the client.  Also is used by client to see if it needs to get a newer version of the offline cache of the NPC.
		};

		/// <summary>
		/// Enumerator -> Packet Out
		/// </summary>
		public enum PacketOut
		{
			LEVELWARP = 0,
			BOARDMODIFY = 1,
			PLAYERPROPS = 2,
			NPCPROPS = 3,
			BOMBADD = 4,
			BOMBDEL = 5,
			TOALL = 6,
			HORSEADD = 7,
			HORSEDEL = 8,
			ARROWADD = 9,
			FIRESPY = 10,
			THROWCARRIED = 11,
			ITEMADD = 12,
			ITEMDEL = 13,
			CLAIMPKER = 14,
			BADDYPROPS = 15,
			BADDYHURT = 16,
			BADDYADD = 17,
			FLAGSET = 18,
			FLAGDEL = 19,
			OPENCHEST = 20,
			PUTNPC = 21,
			NPCDEL = 22,
			WANTFILE = 23,
			SHOWIMG = 24,
			UNKNOWN25 = 25,
			HURTPLAYER = 26,
			EXPLOSION = 27,
			PRIVATEMESSAGE = 28,
			NPCWEAPONDEL = 29,
			LEVELWARPMOD = 30,
			PACKETCOUNT = 31,
			ITEMTAKE = 32,
			WEAPONADD = 33,
			UPDATEFILE = 34,
			ADJACENTLEVEL = 35,
			HITOBJECTS = 36,
			LANGUAGE = 37,
			TRIGGERACTION = 38,
			MAPINFO = 39,
			SHOOT = 40,
			SERVERWARP = 41,
			PROCESSLIST = 44,
			UNKNOWN46 = 46,
			// Always is 1.  Might be a player count for the gmap level.
			UNKNOWN47 = 47,
			// Seems to tell the server the modTime of update files.  Used for client updates.
			RAWDATA = 50,
			RC_SERVEROPTIONSGET = 51,
			RC_SERVEROPTIONSSET = 52,
			RC_FOLDERCONFIGGET = 53,
			RC_FOLDERCONFIGSET = 54,
			RC_RESPAWNSET = 55,
			RC_HORSELIFESET = 56,
			RC_APINCREMENTSET = 57,
			RC_BADDYRESPAWNSET = 58,
			RC_PLAYERPROPSGET = 59,
			RC_PLAYERPROPSSET = 60,
			RC_DISCONNECTPLAYER = 61,
			RC_UPDATELEVELS = 62,
			RC_ADMINMESSAGE = 63,
			RC_PRIVADMINMESSAGE = 64,
			RC_LISTRCS = 65,
			RC_DISCONNECTRC = 66,
			RC_APPLYREASON = 67,
			RC_SERVERFLAGSGET = 68,
			RC_SERVERFLAGSSET = 69,
			RC_ACCOUNTADD = 70,
			RC_ACCOUNTDEL = 71,
			RC_ACCOUNTLISTGET = 72,
			RC_PLAYERPROPSGET2 = 73,
			// Gets by player ID
			RC_PLAYERPROPSGET3 = 74,
			// Gets by player account name.
			RC_PLAYERPROPSRESET = 75,
			RC_PLAYERPROPSSET2 = 76,
			RC_ACCOUNTGET = 77,
			RC_ACCOUNTSET = 78,
			RC_CHAT = 79,
			PROFILEGET = 80,
			PROFILESET = 81,
			RC_WARPPLAYER = 82,
			RC_PLAYERRIGHTSGET = 83,
			RC_PLAYERRIGHTSSET = 84,
			RC_PLAYERCOMMENTSGET = 85,
			RC_PLAYERCOMMENTSSET = 86,
			RC_PLAYERBANGET = 87,
			RC_PLAYERBANSET = 88,
			RC_FILEBROWSER_START = 89,
			RC_FILEBROWSER_CD = 90,
			RC_FILEBROWSER_END = 91,
			RC_FILEBROWSER_DOWN = 92,
			RC_FILEBROWSER_UP = 93,
			RC_FILEBROWSER_QUERY = 94,
			RC_FILEBROWSER_MOVE = 96,
			RC_FILEBROWSER_DELETE = 97,
			RC_FILEBROWSER_RENAME = 98,
			NC_NPCGET = 103,
			// {103}{INT id}
			NC_NPCDELETE = 104,
			// {104}{INT id}
			NC_NPCRESET = 105,
			// {105}{INT id}
			NC_NPCSCRIPTGET = 106,
			// {106}{INT id}
			NC_NPCWARP = 107,
			// {107}{INT id}{CHAR x*2}{CHAR y*2}{level}
			NC_NPCFLAGSGET = 108,
			// {108}{INT id}
			NC_NPCSCRIPTSET = 109,
			// {109}{INT id}{GSTRING script}
			NC_NPCFLAGSSET = 110,
			// {110}{INT id}{GSTRING flags}
			NC_NPCADD = 111,
			// {111}{GSTRING info}  - (info) name,id,type,scripter,starting level,x,y
			NC_CLASSEDIT = 112,
			// {112}{class}
			NC_CLASSADD = 113,
			// {113}{CHAR name length}{name}{GSTRING script}
			NC_LOCALNPCSGET = 114,
			// {114}{level}
			NC_WEAPONLISTGET = 115,
			// {115}
			NC_WEAPONGET = 116,
			// {116}{weapon}
			NC_WEAPONADD = 117,
			// {117}{CHAR weapon length}{weapon}{CHAR image length}{image}{code}
			NC_WEAPONDELETE = 118,
			// {118}{weapon}
			NC_CLASSDELETE = 119,
			// {119}{class}
			NC_LEVELLISTGET = 150,
			// {150}
			NC_LEVELLISTSET = 151,
			// {151}{GSTRING levels}
			REQUESTTEXT = 152,
			// Gets a value from the server.
			SENDTEXT = 154,
			// Sets a value on the server.
			RC_LARGEFILESTART = 155,
			RC_LARGEFILEEND = 156,
			UNKNOWN157 = 157,
			// Something to do with ganis.
			UPDATESCRIPT = 158,
			// {158}{script} Requests a script from the server.
			RC_FOLDERDELETE = 160,
			UNKNOWN161 = 161,
			// {161}{INT5 modtime}{name} iPhone client.
		};

		public enum PLPROPS
		{
			NICKNAME = 0,
			MAXPOWER = 1,
			CURPOWER = 2,
			RUPEESCOUNT = 3,
			ARROWSCOUNT = 4,
			BOMBSCOUNT = 5,
			GLOVEPOWER = 6,
			BOMBPOWER = 7,
			SWORDPOWER = 8,
			SHIELDPOWER = 9,
			GANI = 10,
			// BOWGIF in pre-2.x
			HEADGIF = 11,
			CURCHAT = 12,
			COLORS = 13,
			ID = 14,
			X = 15,
			Y = 16,
			SPRITE = 17,
			STATUS = 18,
			CARRYSPRITE = 19,
			CURLEVEL = 20,
			HORSEGIF = 21,
			HORSEBUSHES = 22,
			EFFECTCOLORS = 23,
			CARRYNPC = 24,
			APCOUNTER = 25,
			MAGICPOINTS = 26,
			KILLSCOUNT = 27,
			DEATHSCOUNT = 28,
			ONLINESECS = 29,
			IPADDR = 30,
			UDPPORT = 31,
			ALIGNMENT = 32,
			ADDITFLAGS = 33,
			ACCOUNTNAME = 34,
			BODYIMG = 35,
			RATING = 36,
			GATTRIB1 = 37,
			GATTRIB2 = 38,
			GATTRIB3 = 39,
			GATTRIB4 = 40,
			GATTRIB5 = 41,
			ATTACHNPC = 42,
			GMAPLEVELX = 43,
			GMAPLEVELY = 44,
			Z = 45,
			GATTRIB6 = 46,
			GATTRIB7 = 47,
			GATTRIB8 = 48,
			GATTRIB9 = 49,
			JOINLEAVELVL = 50,
			PCONNECTED = 51,
			PLANGUAGE = 52,
			PSTATUSMSG = 53,
			GATTRIB10 = 54,
			GATTRIB11 = 55,
			GATTRIB12 = 56,
			GATTRIB13 = 57,
			GATTRIB14 = 58,
			GATTRIB15 = 59,
			GATTRIB16 = 60,
			GATTRIB17 = 61,
			GATTRIB18 = 62,
			GATTRIB19 = 63,
			GATTRIB20 = 64,
			GATTRIB21 = 65,
			GATTRIB22 = 66,
			GATTRIB23 = 67,
			GATTRIB24 = 68,
			GATTRIB25 = 69,
			GATTRIB26 = 70,
			GATTRIB27 = 71,
			GATTRIB28 = 72,
			GATTRIB29 = 73,
			GATTRIB30 = 74,
			OSTYPE = 75,
			// 2.19+
			TEXTCODEPAGE = 76,
			// 2.19+
			UNKNOWN77 = 77,
			X2 = 78,
			Y2 = 79,
			Z2 = 80,
			UNKNOWN81 = 81,
			// In Graal v5, where players have the Graal######## accounts, this is their chosen account alias (community name.)
			COMMUNITYNAME = 82,
		}

		/// <summary>
		/// Enumerator -> NC_NPCGET Packets
		/// </summary>
		public enum NCREQ
		{
			NPCLOG = 0,
			GETWEAPONS	= 1,
			GETLEVELS	= 2,
			SENDPM = 3,
			SENDTORC	= 4,
			WEAPONADD = 5,
			WEAPONDEL	= 6,
			PLSETPROPS	= 7,
			PLGETWEPS	= 8,
			PLSNDPACKET	= 9,
			PLADDWEP	= 10,
			PLDELWEP	= 11,
			LEVELGET	= 12,
			NPCPROPSET	= 13,
			NPCWARP = 14,
			PLRPGMSG	= 15,
			PLSETFLAG	= 16,
			PLMSGSIGN	= 17,
			PLSETSTATUS = 18,
			NPCMOVE = 19,
		};

		/// <summary>
		/// Member Variables
		/// </summary>
		private static Framework Server;
		//protected GraalPlayer NCPlayer;
		//protected GraalLevel ActiveLevel = null;
		private static PMWindowList PMWindowManager = PMWindowList.GetInstance();
		protected Abstraction form;
		public string Nickname;
		private static GServerConnection _instance = new GServerConnection();

		public static GServerConnection GetInstance()
		{
			if (_instance == null)
				_instance = new GServerConnection();

			return _instance;
		}

		/// <summary>
		/// Constructor
		/// </summary>
		private GServerConnection()
		{
			Server = Framework.GetInstance();
			this.form = Abstraction.GetInstance();
			//this.ReceiveData();
		}

		public void RunServer()
		{
			Server = Framework.GetInstance();
			this.form = Abstraction.GetInstance();
			this.Init();
			this.Setup();
		}

		public string GetMD5Hash(string input)
		{
			System.Security.Cryptography.MD5CryptoServiceProvider x = new System.Security.Cryptography.MD5CryptoServiceProvider();
			byte[] bs = System.Text.Encoding.UTF8.GetBytes(input);
			bs = x.ComputeHash(bs);
			System.Text.StringBuilder s = new System.Text.StringBuilder();
			foreach (byte b in bs)
			{
				s.Append(b.ToString("x2").ToLower());
			}
			string password = s.ToString();
			return password;
		}

		/// <summary>
		/// Send Login Information
		/// </summary>
		public void SendLogin(String Account, String Password, String Nickname)
		{
			//Set the nickname
			this.Nickname = Nickname;

			string versionStr = "GNW03014";
			int versionInt = 5;

			// Key Packet // GNW03014 // G3D14097 // G3D0208A // GSERV025
			if (this.form.isRC)
			{
				versionStr = "GSERV025";
				versionInt = 6;
			}

			CString keyPacket = new CString() + (byte)versionInt + (byte)mCodec.Key + versionStr + (byte)Account.Length + Account + (byte)Password.Length + Password + new CString("win").Tokenize() + ",";

			// Unknown hash.
			keyPacket += new CString(this.GetMD5Hash("stefanisabuttmunch")).Tokenize() + ",";

			// MD5 hash.
			keyPacket += new CString(this.GetMD5Hash("unixmadlovesboys")).Tokenize() + "," + new CString(this.GetMD5Hash("suckonthis")).Tokenize() + ",";
			keyPacket += new CString("10.4.0  Darwin Kernel Version 10.4.0: Wed Oct 20 20:14:45 PDT 2010; root:xnu-1504.58.28~3/RELEASE_ARM_S5L8930X").Tokenize();
			keyPacket += "\n";

			//Console.WriteLine(keyPacket.Text);
			keyPacket.ZCompress().PreLength();
			this.Send(keyPacket.Buffer);
		}

		/// <summary>
		/// Handle Received Data
		/// </summary>
		/// 
		protected override void HandleData(CString Packet)
		{
			// Check if the instance of Framework is null and get the instance if it is.
			if (Server == null)
				Server = Framework.GetInstance();

			if (this.form == null)
				this.form = Abstraction.GetInstance();

			int prevPacket = 0;
			uint readc = 0;

			while (Packet.BytesLeft > 0)
			{
				CString CurPacket;

				// Grab Single Packet
				if (prevPacket == 100)
					CurPacket = Packet.ReadChars2(readc);
				else
					CurPacket = Packet.ReadString('\n');

				// Read Packet Type
				int PacketId = CurPacket.ReadGUByte1();

				// Run Internal Packet Function
				switch ((PacketIn)PacketId)
				{
					
				// Packet 6 - Set Active Level & Clear
					case PacketIn.LEVELNAME:
						//ActiveLevel = Server.FindLevel(CurPacket.ReadString().Text);
						//ActiveLevel.Clear();
						break;

				// Paceket 1 - Level Links
					case PacketIn.LEVELLINK:
						break;

				// Packet 3 - Level NPC Props
					case PacketIn.NPCPROPS:
						{
							int npcId = CurPacket.ReadGByte3();
							//if (ActiveLevel != null)
							//	ActiveLevel.GetNPC(npcId).SetProps(CurPacket);
							break;
						}

				// Packet 5 - Level Signs
					case PacketIn.LEVELSIGN:
						break;

				// Packet 39 - Level Mod Time
					case PacketIn.LEVELMODTIME:
						uint modTime = CurPacket.ReadGUByte5();
						//if (ActiveLevel != null)
						//	ActiveLevel.SetModTime(modTime);
						break;
                    
					case PacketIn.SIGNATURE:
						{
							this.form.WriteText("Connected!");
							Server.SendGSPacket(new CString() + (byte)PacketOut.PLAYERPROPS + (byte)PLPROPS.NICKNAME + (byte)this.Nickname.Length + this.Nickname);
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,pmguilds,\"\"");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,pmservers,\"all\"");
							//Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,pmserverplayers,\"U Classic iPhone\"\n");
				
							Server.SendGSPacket(new CString() + (byte)PacketOut.SENDTEXT + "GraalEngine,lister,options,globalpms=true,buddytracking=true,showbuddies=true");
							Server.SendGSPacket(new CString() + (byte)PacketOut.SENDTEXT + "GraalEngine,lister,verifybuddies,1,1964252486");
					
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"unixmad\"");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"stefan\"");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"tig\"");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"skyld\"");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"xor\"");
							//Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "-ShopGlobal,lister,getglobalitems\n");

							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "-Serverlist,lister,list,all");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "-Serverlist,lister,upgradeinfo\n");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "-Serverlist,lister,subscriptions\n");
							Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "-Serverlist,lister,getlockcomputer");

							// Logging into IRC. 
							Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,login,-");
							Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,privmsg,IRCBot,!geteventbots");
							Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,join,#graalim");

							break;
						}
					

					case PacketIn.BOARDPACKET:
						{
							break;
						}
					
					case PacketIn.RAWDATA:
						readc = CurPacket.ReadGUByte3();
						Console.WriteLine(":::readc::::" + readc.ToString() + "::::");
						break;

                    
				// Packet 150 - Delete Level NPC
					case PacketIn.NPCDEL2:
						//Server.FindLevel(CurPacket.ReadChars(CurPacket.ReadGUByte1())).DeleteNPC(CurPacket.ReadGByte3());
						break;

				// Packet 156 - Set Active Level
					case PacketIn.SETACTIVELEVEL:
						//ActiveLevel = Server.FindLevel(CurPacket.ReadString().Text);
						break;

					case PacketIn.ADDPLAYER:
						{
							Player Player = (Player)Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());

							if (Player != null)
							{
								Player.Account = CurPacket.ReadChars(CurPacket.ReadGUByte1());
								Player.SetProps(CurPacket);
							}

							break;
						}

					case PacketIn.DELPLAYER:
						{
							short pId = CurPacket.ReadGByte2();
							Server.PlayerManager.DeletePlayer(pId);
							break;
						}

				// Add Player & Set Props
					case PacketIn.OTHERPLPROPS:
						{
							Player Player = (Player)Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());

							if (Player != null)
							{
								Player.SetProps(CurPacket);
							}
					
							break;
						}

					case PacketIn.FLAGSET:
						{
							Player Player = (Player)Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());
							String FlagName = CurPacket.ReadString('=').Text;
							String FlagValue = CurPacket.ReadString().Text;
							if (Player != null)
								Player.Flags[FlagName] = FlagValue;
							break;
						}
					
					case PacketIn.PLAYERPROPS:
						{
							if (this.form.NCPlayer == null)
								this.form.NCPlayer = Server.PlayerManager.AddPlayer(0);
							//this.form.WriteText(" -!- ME!\r\n");
							this.form.NCPlayer.SetProps(CurPacket);
							break;
						}
					
				// Add weapon to list
					case PacketIn.NPCWEAPONADD:
						{
							String WeaponName = CurPacket.ReadChars(CurPacket.ReadGUByte1());
							String WeaponImage = CurPacket.ReadChars(CurPacket.ReadGUByte1());
							String WeaponScript = CurPacket.ReadString().Text;
							//Server.SetWeapon(WeaponName, WeaponImage, WeaponScript, false);
							break;
						}

				// Add/Remove weapon from Player
					case PacketIn.NPCWEAPONDEL:
						{
							Player Player = Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());
							if (Player != null)
							{
								bool addWeapon = (CurPacket.ReadGByte1() > 0);
								String name = CurPacket.ReadString().Text;
								if (addWeapon)
									Player.AddWeapon(name);
								else
									Player.DeleteWeapon(name, false);
							}

							break;
						}

					case PacketIn.UNKNOWN60:
						{
							Player Player = Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());
							if (Player != null)
							{
								while (CurPacket.BytesLeft > 0)
									Player.AddWeapon(CurPacket.ReadChars(CurPacket.ReadGUByte1()));
							}
							break;
						}

					case PacketIn.NEWWORLDTIME: // Remove Class from List
						Server.NWTime = CurPacket.ReadGByte4();
						break;
									
				// Private Message
					case PacketIn.PRIVATEMESSAGE:
						{
							short PlayerId = CurPacket.ReadGByte2();
							CString Message = CurPacket.ReadString();

							this.ReceivePM(PlayerId, Message);

							break;
						}
					case PacketIn.TOALL:
						{
							
							short PlayerId = CurPacket.ReadGByte2();
							CString Message = CurPacket.ReadString();
							//this.form.WriteText(Message.Text);
							this.ReceiveToall(PlayerId, Message);

							break;
						}
					case PacketIn.RC_SERVEROPTIONSGET:
						string serverOptions = CurPacket.ReadString().Untokenize().ToString();
						this.form.WriteText(serverOptions);
						break;
					case PacketIn.RC_SERVERFLAGSGET:
						int serverFlagsTotal = CurPacket.ReadGByte2();
						List<string> serverFlags = new List<string>();
						string serverFlagsString = "";
						while (CurPacket.BytesLeft > 0)
						{
							serverFlags.Add(CurPacket.ReadChars(CurPacket.ReadGUByte1()));
						}
						serverFlags.Sort();
						foreach (string sf in serverFlags)
							serverFlagsString += sf + "\n";

						this.ReceivedServerFlags(serverFlagsTotal, serverFlagsString);
						break;
					case PacketIn.RC_FOLDERCONFIGGET:
						//CString s2 = CurPacket.Untokenize();
						string folderConfig = CurPacket.ReadString().Untokenize().ToString();
						this.ReceivedFolderConfig(folderConfig);
						break;
					case PacketIn.RC_FILEBROWSER_DIR:
						//CString s2 = CurPacket.Untokenize();
						this.form.WriteText(CurPacket.ReadString().ToString());
						break;
					case PacketIn.RC_FILEBROWSER_DIRLIST:
						List<string> fileBrowserDirList = new List<string>(CurPacket.ReadString().Untokenize().ToString().Split('\n'));
						fileBrowserDirList.Sort();
						foreach (string dir in fileBrowserDirList)
							this.form.WriteText(dir);
						break;
					case PacketIn.RC_FILEBROWSER_MESSAGE:
						this.form.WriteText(CurPacket.ReadString().ToString());
						break;
				// Servertext
					case PacketIn.SERVERTEXT:
						{
							CString Message = CurPacket.ReadString();

							this.form = Abstraction.GetInstance();

							string TheMessage2 = "";
							TheMessage2 = CString.untokenize(Message.Text);
							string[] test = TheMessage2.Split('\n');
							List<string> test2 = new List<string>(test);

							if (test2[0].Trim() == "GraalEngine" || test2[0].Trim() == "-Playerlist")
							{
								if (test2[1].Trim() == "lister")
								{
								}
							}
							else if (test2[0].Trim() == "-Serverlist_Chat")
							{
								if (test2[1].Trim() == "irc")
								{
									switch (test2[2].Trim())
									{
										case "join":
											{
												this.form.OpenIRC(test2[3].Trim());
												break;
											}

										case "addchanneluser"://[SERVERTEXT]: -Serverlist_Chat irc addchanneluser #graal CrypticMyst login1 Gos_Pira 
											{
												this.form.IRC_AddChannelUser(test2[3].Trim(), test2[4].Trim());
												break;
											}
										case "deletechanneluser"://[SERVERTEXT]: -Serverlist_Chat irc delchanneluser #graal CrypticMyst
											{
												this.form.IRC_AddChannelUser(test2[3].Trim(), test2[4].Trim(), true);
												break;
											}
										case "privmsg":
											{
												string message = test2[5].Trim();
												if (test2[4].Trim().StartsWith("#"))
													this.form.IRC_Privmsg(test2[3].Trim(), test2[4].Trim(), message);
												else
												{

													this.ReceivePM(test2[3].Trim(), new CString(message));
												}
												break;
											}

										default:
											{
										
												break;
											}
									}

								}
							}
							else if (test2[0].Trim() == "-Serverlist")
							{
								if (test2[1].Trim() == "lister")
								{
									#region SimpleServerList
									if (test2[2].Trim() == "subscriptions2")
									{
										string subscription = "Trial";
										//this.form.WriteText(test2.Count.ToString());

										if (test2.Count > 3)
										{
											//this.form.WriteText(test2.Count.ToString());
											string server2 = CString.untokenize(test2[3].Trim());
											string[] test3 = server2.Split('\n');
											subscription = test3[1].Trim();
										}

										this.form.SetSubscriptionText("Subscription: " + subscription);
									}
									else if (test2[2].Trim() == "lockcomputer")
									{
										int locked = 0;
										int.TryParse(test2[3].Trim(), out locked);

										this.form.SetLockedByComputerText("Locked by PC-ID: " + ((locked == 1) ? "Yes" : "No"));
									}
									else if (test2[2].Trim() == "simpleserverlist" || test2[2].Trim() == "serverlist")
									{
										List<string> servers = test2;
										servers.RemoveRange(0, 3);
										this.form.WriteText("SERVERS(" + servers.Count + "):");
										this.form.WriteText("------------");
										//this.form.WriteText("[SERVERTEXT]: " + TheMessage2 + "");
										foreach (string server in servers)
										{
											//CString server2 = new CString();
											//server2.Write(server);
											if (server != "")
											{
												string server2 = CString.untokenize(server);
												string[] test3 = server2.Split('\n');
												//this.form.Write_Text(server2 + "\r\n");
												var servername = "";
												var servertype = "";
												if (test3[1].Trim().Substring(0, 2) == "P " || test3[1].Trim().Substring(0, 2) == "H " || test3[1].Trim().Substring(0, 2) == "3 " || test3[1].Trim().Substring(0, 2) == "U ")
												{
													servertype = test3[1].Trim().Substring(0, 2);
													servername = test3[1].Trim().Substring(2, test3[1].Trim().Length - 2);
													if (servertype == "P ")
														servertype = "Gold";
													if (servertype == "H ")
														servertype = "Hosted";
													if (servertype == "3 ")
														servertype = "3D";
													if (servertype == "U ")
														servertype = "Hidden";
												}
												else
												{
													servertype = "Classic";
													servername = test3[1].Trim();
												}

												var serverid = test3[0].Trim();
												var serverpc = test3[2].Trim();


												this.form.WriteText(" * Id: " + serverid + " Type: " + servertype + " Name: " + servername + " Players: " + serverpc + "");
											}

										}
										this.form.WriteText("");
									}
									else
										this.form.WriteText("[SERVERTEXT]: " + TheMessage2.Replace("\r\n", ",") + "");
									#endregion
								}
							}
							else
							{ }// this.form.WriteText("[SERVERTEXT]: " + TheMessage2.Replace("\r\n", ",") + "");

							break;
						}
						
					case PacketIn.SHOWIMG:
						{
							break;
						}
					case PacketIn.DISCMESSAGE:
						{
							//Console.WriteLine("PacketId: " + PacketId.ToString());
							string packetName = "";//Enum.GetName(typeof(PacketIn), PacketId).ToString();
							this.form.WriteText("Disconnect: " + CurPacket.ReadString().Text + "\r\n");

							break;
						}
					case PacketIn.RC_CHAT:
						this.form.WriteText(CurPacket.ReadString().Text + "");
						break;
					default:
						{
							Console.WriteLine("PacketId: " + PacketId.ToString());
							try
							{
								string packetName = Enum.GetName(typeof(PacketIn), PacketId).ToString();
								this.form.WriteText("[" + packetName + "]: " + CurPacket.ReadString().Text + "");
							}
							catch (IndexOutOfRangeException e)
							{

							}
							catch (NullReferenceException e)
							{

							}
							break;
						}
				}

				prevPacket = PacketId;
                   
			}
		}

		public void ReceivedServerFlags(int serverFlagsTotal, string serverFlagsString)
		{
			this.form.WriteText(serverFlagsTotal.ToString() + serverFlagsString);
		}

		public void ReceivedFolderConfig(string folderConfig)
		{
			this.form.WriteText(folderConfig);
		}

		private void ReceivePM(string PlayerId, CString Message)
		{
			Player PMPlayer = (Player)Server.PlayerManager.FindPlayer(PlayerId);
			this.ReceivePM(PMPlayer, Message);
		}

		private void ReceivePM(short PlayerId, CString Message)
		{
			Player PMPlayer = (Player)Server.PlayerManager.FindPlayer(PlayerId);
			if (PMPlayer != null)
				this.ReceivePM(PMPlayer, Message);
			else
				this.form.WriteText("Something went wrong. PMPlayer is null.");
		}

		private void ReceivePM(Player Player, CString Message)
		{
			
			this.form = Abstraction.GetInstance();
			this.form.WriteText(" -!- Received PM from " + Player.Account.ToString() + "!\n");

			PMWindowManager = PMWindowList.GetInstance();
			PMWindow PM = PMWindowManager.AddPMWindow(Player.Id);

			if (PM != null)
			{
				PM.SetMessage(Message);
			}
		}

		private void ReceiveToall(string PlayerId, CString Message)
		{
			Player PMPlayer = (Player)Server.PlayerManager.FindPlayer(PlayerId);
			this.ReceivePM(PMPlayer, Message);
		}

		private void ReceiveToall(short PlayerId, CString Message)
		{
			Player PMPlayer = (Player)Server.PlayerManager.FindPlayer(PlayerId);
			if (PMPlayer != null)
				this.ReceiveToall(PMPlayer, Message);
			else
				this.form.WriteText("Something went wrong. PMPlayer is null.");
		}

		private void ReceiveToall(Player Player, CString Message)
		{

			this.form = Abstraction.GetInstance();
			//this.form.WriteText(" -!- Received toall from " + Player.Account.ToString() + "!\n");

		
			ToallsWindow PM = ToallsWindow.GetInstance();

			if (PM != null)
			{
				PM.SetMessage(Player, Message);
			}
		}

	}
}
