using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;
//using OpenGraal.Common.Players;

/*
public void onCreated()
{
  var npc = FindWeapon("-gr_movement");
  SendToRC("TEST: " + npc.name + " - " + IMCOOL());
}

public string IMCOOL()
{
  return "RUCOOL?";
}

public void onCreated()
{
  foreach (GraalPlayer pl in players)
    SendToRC("Player " + pl.Id + ": " + pl.Account);
}

public string IMCOOL()
{
  return "RUCOOL?";
}

*/

namespace OpenGraal.GraalIM
{
	public class Player : OpenGraal.Common.Players.GraalPlayer
	{
		/// <summary>
		/// Member Variables
		/// </summary>
		internal Framework Server;
		protected Abstraction form;

		/// <summary>
		/// Constructor by Account / Id
		/// </summary>
		internal Player(Int16 Id) : base(Id)
		{
			this.Server = Framework.GetInstance();
			this.form = Abstraction.GetInstance();
			this.Id = Id;
			this.ColorManager = new OpenGraal.Common.Players.ColorManager(new OpenGraal.Common.Players.ColorManager.dSendColors(SendColors));
			this.FlagManager = new OpenGraal.Common.Players.FlagManager(new OpenGraal.Common.Players.FlagManager.dSendFlag(SendFlag));
			//this.form.AddPlayer(this.id, this.account);
		}
		/*
		/// <summary>
		/// Send Colors
		/// </summary>
		internal void SendColors()
		{
			this.SendProp(Properties.PLCOLORS);
		}

		/// <summary>
		/// Send Flag to Framework
		/// </summary>
		internal void SendFlag(String FlagName, String FlagValue)
		{
			// fix packet for flag
			//Server.SendGSPacket(new CString() + (byte)GServerConnection.PacketOut.NC_NPCGET + (byte)GServerConnection.NCREQ.PLSETFLAG + (short)Id + FlagName + "=" + FlagValue);
		}

		/// <summary>
		/// Send Prop to GServer
		/// </summary>
		internal void SendProp(Properties PropId)
		{
			//Server.SendGSPacket(new CString() + (byte)GServerConnection.PacketOut.NC_NPCGET + (byte)GServerConnection.NCREQ.PLSETPROPS + (short)this.Id + (byte)PropId + GetProp(PropId));
		}

		/// <summary>
		/// Set Player Guild
		/// </summary>
		internal void SetGuild(string GuildName)
		{
			int guildStart, guildEnd;
			if ((guildStart = Nickname.IndexOf('(')) >= 0)
			{
				if ((guildEnd = Nickname.IndexOf(')', guildStart)) >= 0)
					Nickname = Nickname.Substring(0, guildStart).TrimEnd();
			}

			if (GuildName != String.Empty)
				Nickname += " (" + GuildName + ")";
		}

		/// <summary>
		/// Set Player Nickname
		/// </summary>
		/// <param name="Nickname"></param>
		internal void SetNick(string Nickname)
		{
			// Set Nickname
			this.Nickname = Nickname;
			this.Guild = "";

			// Parse Guild
			int len = Nickname.LastIndexOf('(');
			if (len != -1)
			{
				int len2 = Nickname.LastIndexOf(')');
				if (len2 > len)
					this.Guild = Nickname.Substring(len + 1, len2 - len - 1);
			}

			this.form.AddPlayer(this.account, this.Nickname);

		}
		*/

		/// <summary>
		/// Set Properties
		/// </summary>
		public void SetProps(CString Packet)
		{
			this.form = Abstraction.GetInstance();
			bool moved = false;

			while (Packet.BytesLeft > 0)
			{
				Int32 PropId = Packet.ReadGUByte1();
				//this.form.AddPlayer(this.Account, this.Account);
				switch ((Properties)PropId)
				{
					case Properties.NICKNAME: // 0
						{
							this.SetNick(Packet.ReadChars(Packet.ReadGUByte1()));
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Nickname + "");
							break;
						}

					case Properties.MAXPOWER: // 1
						this.FullHearts = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.FullHearts + "\r\n");
						break;

					case Properties.CURPOWER: // 2
						this.Hearts = (double)Packet.ReadGUByte1() / 2;
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Hearts + "\r\n");
						break;

					case Properties.RUPEESCOUNT: // 3
						this.Gralats = (int)Packet.ReadGUByte3();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Gralats + "");
						break;

					case Properties.ARROWSCOUNT: // 4
						this.Arrows = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Arrows + "\r\n");
						break;

					case Properties.BOMBSCOUNT: // 5
						this.Bombs = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Bombs + "\r\n");
						break;

					case Properties.GLOVEPOWER: // 6
						this.GlovePower = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.glovepower + "\r\n");
						break;

					case Properties.BOMBPOWER: // 7
						this.BombPower = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.BombPower + "\r\n");
						break;

					case Properties.SWORDPOWER: // 8
						{
							Int32 sp = Packet.ReadGUByte1();
							if (sp > 4)
							{
								sp -= 30;
								Int32 len = Packet.ReadGUByte1();
								if (len > 0)
									this.SwordImage = Packet.ReadChars(len);
							}
							else
								this.SwordImage = "sword" + sp + ".png";
						
							this.SwordPower = sp;
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.swordpower + "\r\n");
							break;
						}

					case Properties.SHIELDPOWER: // 9
						{
							Int32 sp = Packet.ReadGUByte1();
							if (sp > 3)
							{
								sp -= 10;
								if (sp < 0)
									break;
								Int32 len = Packet.ReadGUByte1();
								if (len > 0)
									this.ShieldImage = Packet.ReadChars(len);
							}
							else
								this.ShieldImage = "shield" + sp + ".png";

							this.ShieldPower = sp;
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.shieldpower + "\r\n");
							break;
						}

					case Properties.ANIMATION: // 10
						this.Ani = Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.ani + "\r\n");
						break;

					case Properties.HEADIMAGE: // 11
						{
							Int32 len = Packet.ReadGUByte1();
							this.HeadImage = (len < 100 ? "head" + len + ".png" : Packet.ReadChars(len - 100));
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.HeadImage + "");
							break;
						}

					case Properties.CURCHAT:
						{// 12
							this.Chat = Packet.ReadChars(Packet.ReadGUByte1());
							//if (PlayerLevel != null)
							//	PlayerLevel.CallNPCs("onPlayerChats", new object[] { this });

							if (this.Chat != null && this.Chat != "" && this.Account != "" && this.Account != null)
								this.form.WriteText("(" + this.Account + ") " + this.Chat + "");

							break;
						}
					case Properties.PLCOLORS: // 13
						for (int i = 0; i < 5; i++)
						{
							this.ColorManager[i] = Packet.ReadGUByte1();
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.ColorManager[i] + "\r\n");
						}
						break;

					case Properties.PLAYERID: // 14
						{
							this.Id = (short)Packet.ReadGUByte2();
							this.form.AddPlayer(this.Account, this.Account);
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Id + "");
							break;

						}
					case Properties.PLAYERX: // 15 - obsolete
						Packet.ReadGUByte1();
						//this.X = ((double)Packet.ReadGUByte1() / 2);
						//moved = true;
					//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte1().ToString() + "\r\n");
						break;

					case Properties.PLAYERY: // 16 - obsolete
						Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte1().ToString() + "\r\n");
						//this.Y = ((double)Packet.ReadGUByte1() / 2);
						//moved = true;
						break;

					case Properties.PLSPRITE: // 17
						this.Dir = Packet.ReadGUByte1() % 4;
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.dir + "\r\n");
						break;

					case Properties.PLSTATUS: // 18
						this.PlayerStatus = Packet.ReadGUByte1();
						if ((this.PlayerStatus & (int)Status.DEAD) != 0)
						{
							//if (PlayerLevel != null)
							//	PlayerLevel.CallNPCs("onPlayerDies", new object[] { this });
						}
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.PlayerStatus + "");
						break;

					case Properties.CARRYSPRITE: // 19
						Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte1().ToString() + "\r\n");
						break;

					case Properties.CURLEVEL: // 20
						string levelName = Packet.ReadChars(Packet.ReadGUByte1()).Trim();
						if (this.Level != null)
							this.Level.DeletePlayer(this);
						this.Level = Server.FindLevel(levelName);
						this.Level.AddPlayer(this);
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Level.Name + "");
						break;

					case Properties.HORSEIMG: // 21
						Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadChars(Packet.ReadGUByte1()).ToString() + "\r\n");
						break;

					case Properties.HORSEBUSHES: // 22
						Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte1().ToString() + "\r\n");
						break;

					case Properties.EFFECTCOLOR: // 23
						{
							int len = Packet.ReadGUByte1();
							if (len > 0)
								Packet.ReadGUByte4();
							break;
						}

					case Properties.CARRYNPC: // 24
						Packet.ReadGUByte3();
					//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte3().ToString() + "\r\n");
						break;

					case Properties.APCOUNTER: // 25
						Packet.ReadGUByte2();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte2().ToString() + "\r\n");
						break;

					case Properties.MAGICPOINTS: // 26
						this.Magic = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Magic.ToString() + "\r\n");
						break;

					case Properties.KILLSCOUNT: // 27
						this.Kills = Packet.ReadGByte3();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.kills.ToString() + "\r\n");
						break;

					case Properties.DEATHSCOUNT: // 28
						this.Deaths = Packet.ReadGByte3();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.deaths.ToString() + "\r\n");
						break;

					case Properties.ONLINESECS: // 29
						this.OnlineTime = Packet.ReadGByte3();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.onlinetime.ToString() + "\r\n");
						break;

					case Properties.IPADDRESS: // 30
						this.IPAddress = new System.Net.IPAddress(Packet.ReadGUByte5()).ToString();
						this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.IPAddress.ToString() + "");
						break;

					case Properties.UDPPORT: // 31
						int port = Packet.ReadGByte3();
						this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + port.ToString() + "");
						break;

					case Properties.ALIGNMENT: // 32
						Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte1().ToString() + "\r\n");
						break;

					case Properties.ADDITFLAGS: // 33
						Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte1().ToString() + "\r\n");
						break;

					case Properties.ACCOUNTNAME: // 34
						this.Account = Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.Account + "");
						break;

					case Properties.BODYIMAGE: // 35
						this.BodyImage = Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.bodyimg + "\r\n");
						break;

					case Properties.PLRATING: // 36
						Packet.ReadGUByte3();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte3().ToString() + "\r\n");
						break;

					case Properties.GATTRIB1: // 37
					case Properties.GATTRIB2: // 38
					case Properties.GATTRIB3: // 39
					case Properties.GATTRIB4: // 40
					case Properties.GATTRIB5: // 41
						Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadChars(Packet.ReadGUByte1()).ToString() + "\r\n");
						break;

					case Properties.PLATTACHNPC: // 42
						Packet.ReadGUByte4();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte4().ToString() + "\r\n");
						break;

					case Properties.GMAPLEVELX: // 43
						GmapX = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + GmapX.ToString() + "\r\n");
						break;

					case Properties.GMAPLEVELY: // 44
						GmapY = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + GmapY.ToString() + "\r\n");
						break;

					case Properties.PLAYERZ: // 45
						Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte1().ToString() + "\r\n");
						break;

					case Properties.GATTRIB6: // 46
					case Properties.GATTRIB7: // 47
					case Properties.GATTRIB8: // 48
					case Properties.GATTRIB9: // 49
						Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadChars(Packet.ReadGUByte1()).ToString() + "\r\n");
						break;

					case Properties.JOINLEAVELV: // 50
						byte joinLeaveLv = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + joinLeaveLv.ToString() + "");
						break;

					case Properties.CONNECTED: // 51
						{
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "] Disconnected\r\n");
							this.form.RMPlayer(this.Account);
							Server.PlayerManager.DeletePlayer(Id);
						
							// disconnect player.
							break;
						}

					case Properties.LANGUAGE: // 52
						Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadChars(Packet.ReadGUByte1()).ToString() + "\r\n");
						break;

					case Properties.STATUSMSG: // 53
						byte statusMsg = Packet.ReadGUByte1();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + statusMsg.ToString() + "");
						break;

					case Properties.GATTRIB10: // 54
					case Properties.GATTRIB11: // 55
					case Properties.GATTRIB12: // 56
					case Properties.GATTRIB13: // 57
					case Properties.GATTRIB14: // 58
					case Properties.GATTRIB15: // 59
					case Properties.GATTRIB16: // 60
					case Properties.GATTRIB17: // 61
					case Properties.GATTRIB18: // 62
					case Properties.GATTRIB19: // 63
					case Properties.GATTRIB20: // 64
					case Properties.GATTRIB21: // 65
					case Properties.GATTRIB22: // 66
					case Properties.GATTRIB23: // 67
					case Properties.GATTRIB24: // 68
					case Properties.GATTRIB25: // 69
					case Properties.GATTRIB26: // 70
					case Properties.GATTRIB27: // 71
					case Properties.GATTRIB28: // 72
					case Properties.GATTRIB29: // 73
					case Properties.GATTRIB30: // 74
						Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadChars(Packet.ReadGUByte1()).ToString() + "\r\n");
						break;

					case Properties.OSTYPE: // 75
						Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadChars(Packet.ReadGUByte1()).ToString() + "");
						break;

					case Properties.TEXTCODEPG: // 76
						Packet.ReadGUByte3();
						this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte3().ToString() + "");
						break;

					case Properties.UNKNOWN77: // 77
						this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.Text.ToString() + "");
						break;

					case Properties.PIXELX: // 78
						{
							int tmp = this.PixelX = Packet.ReadGUByte2();

							// If the first bit is 1, our position is negative.
							this.PixelX >>= 1;
							if ((tmp & 0x0001) != 0)
								this.PixelX = -this.PixelX;
							moved = true;
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.PixelX.ToString() + "\r\n");
							break;
						}

					case Properties.PIXELY: // 79
						{
							int tmp = this.PixelY = Packet.ReadGUByte2();

							// If the first bit is 1, our position is negative.
							this.PixelY >>= 1;
							if ((tmp & 0x0001) != 0)
								this.PixelY = -this.PixelY;
							moved = true;
							//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + this.PixelY.ToString() + "\r\n");
							break;
						}

					case Properties.PIXELZ: // 80
						Packet.ReadGUByte2();
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]" + Packet.ReadGUByte2().ToString() + "\r\n");
						break;

					case Properties.UNKNOWN81: // 81
						this.isLocalPlayer = false;
						this.form.WriteText("(" + this.Account + "(" + this.Id.ToString() + ")) I am on other server.");
						break;

					case Properties.COMMUNITY: // 82
						string communityname = Packet.ReadChars(Packet.ReadGUByte1());
						//this.form.WriteText("[" + this.Account + "(" + this.Id.ToString() + ")" + "][" + Enum.GetName(typeof(Properties), PropId).ToString() + "]: " + communityname + "");
						break;

					default:
						{
							//this.form.WriteText("[" + PropId + "] " + Packet.Text + "\r\n");
							this.form.WriteText("(" + this.Account + "(" + this.Id.ToString() + ")" + ") [" + PropId.ToString() + "]" + Packet.Text.ToString() + "");
							break;
						}
				}
			}

			//if (moved && PlayerLevel != null)
			//{
			//bool onGmap = PlayerLevel.Name.EndsWith(".gmap");
			//GraalLevelNPC npc = PlayerLevel.isOnNPC(PixelX + touchtestd[dir*2], PixelY + touchtestd[dir*2+1]);
			//if (npc != null)
			//	npc.Call("onPlayerTouchsMe", new object[] { this });
			//}
		}
		/*
		/// <summary>
		/// Function -> Delete Weapon from Player
		/// </summary>
		public void DeleteWeapon(String WeaponName)
		{
			DeleteWeapon(WeaponName, true);
		}
		
		/// <summary>
		/// Function -> Delete Weapon from Player
		/// </summary>
		public void DeleteWeapon(String WeaponName, bool SendToServer)
		{
			ServerWeapon weapon = FindWeapon(WeaponName);
			if (weapon != null)
			{
				//if (SendToServer)
				//	Server.SendGSPacket(new CString() + (byte)GServerConnection.PacketOut.NC_NPCGET + (byte)GServerConnection.NCREQ.PLDELWEP + (short)Id + WeaponName);
				WeaponList.Remove(weapon);
			}
		}
		
		/// <summary>
		/// Function -> Find Weapon
		/// </summary>
		public ServerWeapon FindWeapon(String WeaponName)
		{
			foreach (ServerWeapon weapon in WeaponList)
			{
				if (weapon.Name == WeaponName)
					return weapon;
			}
			return null;
		}
		*/

	}
}
