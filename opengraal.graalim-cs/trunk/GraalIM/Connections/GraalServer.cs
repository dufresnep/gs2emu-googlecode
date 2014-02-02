using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;
using OpenGraal.Common.Players;
using OpenGraal.Common.Connections.Client;

namespace OpenGraal.GraalIM.Connections
{
	public class GraalServer : OpenGraal.Common.Connections.Client.GraalServer, IGameServer
	{

		/// <summary>
		/// Member Variables
		/// </summary>
		private static Framework Server;
		private static PMWindowList PMWindowManager = PMWindowList.GetInstance();
		protected Abstraction form;
		public string Nickname;
	
		public void RunServer()
		{
			Server = Framework.GetInstance();
			this.form = Abstraction.GetInstance();
			this.Init();
			this.Setup();
		}

		public void SendLogin(String Account, String Password, String Nickname)
		{
			this.Codec.Reset(Encrypt.Generation.GEN5);
			//Set the nickname
			this.Nickname = Nickname;
			this.form = Abstraction.GetInstance();
			string versionStr = "GNW03014";

			// Key Packet // GNW03014 // G3D14097 // G3D0208A // GSERV025
			if (this.form.isRC)
			{
				versionStr = "GSERV025";
			}

			this.SendLogin(Account, Password, Nickname, versionStr, this.form.isRC);

			this.ReceiveData();
		}

		public override void ReceivedServerFlags(int serverFlagsTotal, string serverFlagsString)
		{
			this.form.WriteText(serverFlagsTotal.ToString() + serverFlagsString);
		}

		public override void ReceivedFolderConfig(string folderConfig)
		{
			this.form.WriteText(folderConfig);
		}

		public override void ReceivedPM(string PlayerId, CString Message)
		{
			Player PMPlayer = (Player)Server.PlayerManager.FindPlayer(PlayerId);
			this.ReceivedPM(PMPlayer, Message);
		}

		public override void ReceivedPM(short PlayerId, CString Message)
		{
			Player PMPlayer = (Player)Server.PlayerManager.FindPlayer(PlayerId);
			if (PMPlayer != null)
				this.ReceivedPM(PMPlayer, Message);
			else
				this.form.WriteText("Something went wrong. PMPlayer is null.");
		}

		public void ReceivedPM(Player Player, CString Message)
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

		public override void ReceivedToall(short PlayerId, CString Message)
		{
			Player PMPlayer = (Player)Server.PlayerManager.FindPlayer(PlayerId);
			if (PMPlayer != null)
				this.ReceivedToall(PMPlayer, Message);
			else
				this.form.WriteText("Something went wrong. PMPlayer is null.");
		}

		public void ReceivedToall(Player Player, CString Message)
		{
			this.form = Abstraction.GetInstance();

			ToallsWindow PM = ToallsWindow.GetInstance();

			if (PM != null)
			{
				PM.SetMessage(Player, Message);
			}
		}

		public override void WriteText(string text)
		{
			this.form.WriteText(text);
		}

		public override void ReceivedRCChat(string text)
		{
			this.form.WriteText(text);
		}

		public override void ReceivedSignature()
		{
			this.form.WriteText("Connected!");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.PLAYERPROPS + (byte)GraalServer.PLPROPS.NICKNAME + (byte)this.Nickname.Length + this.Nickname);
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "GraalEngine,pmguilds,\"\"");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "GraalEngine,pmservers,\"all\"");
			//Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "GraalEngine,pmserverplayers,\"U Classic iPhone\"\n");

			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.SENDTEXT + "GraalEngine,lister,options,globalpms=true,buddytracking=true,showbuddies=true");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.SENDTEXT + "GraalEngine,lister,verifybuddies,1,1964252486");

			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"unixmad\"");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"stefan\"");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"tig\"");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"skyld\"");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "GraalEngine,lister,addbuddy,\"xor\"");
			//Server.SendGSPacket(new CString() + (byte)PacketOut.REQUESTTEXT + "-ShopGlobal,lister,getglobalitems\n");

			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "-Serverlist,lister,list,all");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "-Serverlist,lister,upgradeinfo\n");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "-Serverlist,lister,subscriptions\n");
			Server.SendGSPacket(new CString() + (byte)GraalServer.PacketOut.REQUESTTEXT + "-Serverlist,lister,getlockcomputer");

			// Logging into IRC. 
			Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,login,-");
			Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,privmsg,IRCBot,!geteventbots");
			Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,join,#graalim");
		}

		public override void ReceivedServertext(List<string> serverText)
		{
			if (serverText[0].Trim() == "GraalEngine" || serverText[0].Trim() == "-Playerlist")
			{
				if (serverText[1].Trim() == "lister")
				{
				}
			}
			else if (serverText[0].Trim() == "-Serverlist_Chat")
			{
				if (serverText[1].Trim() == "irc")
				{
					switch (serverText[2].Trim())
					{
						case "join":
							{
								this.form.OpenIRC(serverText[3].Trim());
								break;
							}

						case "addchanneluser"://[SERVERTEXT]: -Serverlist_Chat irc addchanneluser #graal CrypticMyst login1 Gos_Pira 
							{
								this.form.IRC_AddChannelUser(serverText[3].Trim(), serverText[4].Trim());
								break;
							}
						case "deletechanneluser"://[SERVERTEXT]: -Serverlist_Chat irc delchanneluser #graal CrypticMyst
							{
								this.form.IRC_AddChannelUser(serverText[3].Trim(), serverText[4].Trim(), true);
								break;
							}
						case "privmsg":
							{
								string message = serverText[5].Trim();
								if (serverText[4].Trim().StartsWith("#"))
									this.form.IRC_Privmsg(serverText[3].Trim(), serverText[4].Trim(), message);
								else
								{

									this.ReceivedPM(serverText[3].Trim(), new CString(message));
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
			else if (serverText[0].Trim() == "-Serverlist")
			{
				if (serverText[1].Trim() == "lister")
				{
					#region SimpleServerList
					if (serverText[2].Trim() == "subscriptions2")
					{
						string subscription = "Trial";
						//this.form.WriteText(test2.Count.ToString());

						if (serverText.Count > 3)
						{
							//this.form.WriteText(test2.Count.ToString());
							string server2 = CString.untokenize(serverText[3].Trim());
							string[] test3 = server2.Split('\n');
							subscription = test3[1].Trim();
						}

						this.form.SetSubscriptionText("Subscription: " + subscription);
					}
					else if (serverText[2].Trim() == "lockcomputer")
					{
						int locked = 0;
						int.TryParse(serverText[3].Trim(), out locked);

						this.form.SetLockedByComputerText("Locked by PC-ID: " + ((locked == 1) ? "Yes" : "No"));
					}
					else if (serverText[2].Trim() == "simpleserverlist" || serverText[2].Trim() == "serverlist")
					{
						List<string> servers = serverText;
						servers.RemoveRange(0, 3);
						//this.form.WriteText("SERVERS(" + servers.Count + "):");
						//this.form.WriteText("------------");
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


								//this.form.WriteText(" * Id: " + serverid + " Type: " + servertype + " Name: " + servername + " Players: " + serverpc + "");
							}

						}
						this.form.WriteText("");
					}
					else
					{ }
					
					#endregion
				}
			}
			else
			{ }// this.form.WriteText("[SERVERTEXT]: " + TheMessage2.Replace("\r\n", ",") + "");
		}
	}
}
