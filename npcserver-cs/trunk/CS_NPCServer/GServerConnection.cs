using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using CS_NPCServer;

namespace CS_NPCServer
{
	public class GServerConnection : GraalSocket
	{
		#region Enumerators
		/// <summary>
		/// Enumerator -> Packet In
		/// </summary>
		public enum PacketIn
		{
			// Level Packets
			LEVELLINKS = 1,
			LEVELNPCPROPS = 3,
			LEVELSIGNS = 5,
			LEVELNAME = 6,
			LEVELMODTIME = 39,
			LEVELBOARD = 101,
			LEVELNPCDELETE = 150,
			LEVELSETACTIVE = 156,

			// File Packets
			FILEFAILED = 30,
			FILEBIGSTART = 68,
			FILEBIGEND = 69,	
			RAWDATA = 100,
			FILEDATA = 102,

			// NPC-Server Management
			NPCWEAPONADD = 33,
			NPCCLASSADD = 113,

			// Player-Management Packets
			OTHERPLPROPS = 8,
			PLAYERPROPS = 9,
			PLFLAGSET = 28,
			PLWEAPONMNG = 34,
			PRIVATEMESSAGE = 37,
			NEWWORLDTIME = 42,
			TRIGGERACTION = 48,
			PLWEAPONSSET = 60,
		};

		/// <summary>
		/// Enumerator -> Packet Out
		/// </summary>
		public enum PacketOut
		{
			PLAYERPROPS = 2,
			FILEGET = 23,
			RCCHAT = 79,
			NCQUERY = 103,
		};

		/// <summary>
		/// Enumerator -> NCQuery Packets
		/// </summary>
		public enum NCREQ
		{
			NPCLOG		= 0,
			GETWEAPONS	= 1,
			GETLEVELS	= 2,
			SENDPM		= 3,
			SENDTORC	= 4,
			WEAPONADD   = 5,
			WEAPONDEL	= 6,
			PLSETPROPS	= 7,
			PLGETWEPS	= 8,
			PLSNDPACKET	= 9,
			PLADDWEP	= 10,
			PLDELWEP	= 11,
			LEVELGET	= 12,
			NPCPROPSET	= 13,
			NPCWARP		= 14,
			PLRPGMSG	= 15,
			PLSETFLAG	= 16,
			PLMSGSIGN	= 17,
			PLSETSTATUS = 18,
			NPCMOVE		= 19,
			CLASSSET	= 20,
		};
		#endregion

		/// <summary>
		/// Member Variables
		/// </summary>
		protected NPCServer Server;
		protected GraalPlayer ActivePlayer = null;
		protected GraalLevel ActiveLevel = null;

		// Raw Data
		protected byte LastPacketId = 0;
		protected int ReadCount = 0;
		protected MemoryStream MemStream = null;
		protected String DownloadFile = String.Empty;

		/// <summary>
		/// Constructor
		/// </summary>
		public GServerConnection(NPCServer Server)
		{
			this.Server = Server;
		}

		/// <summary>
		/// Send Login Information
		/// </summary>
		public void SendLogin(String Account, String Password, String Nickname)
		{
			// Send Login
			DataBuffer LoginPacket = new DataBuffer() + (byte)2 + "GRNS0000" + (byte)Account.Length + Account + (byte)Password.Length + Password + (short)Server.NPCPort + "\n"
				+  (byte)PacketOut.PLAYERPROPS + (byte)0 + (byte)Nickname.Length + Nickname + "\n";
			LoginPacket.ZCompress().PreLength();
			this.Send(LoginPacket.Buffer);
		}

		/// <summary>
		/// Start Download
		/// </summary>
		protected void DownloadStart(String FileName)
		{
			this.DownloadFile = FileName;
			this.MemStream = new MemoryStream();
		}

		/// <summary>
		/// End Download
		/// </summary>
		protected void DownloadEnd()
		{
			if (this.MemStream != null)
			{
				this.DownloadFinished(this.DownloadFile);
				this.MemStream.Close();
				this.MemStream = null;
			}
		}

		/// <summary>
		/// File Finished?
		/// </summary>
		protected void DownloadFinished(String FileName)
		{
			StreamReader reader = new StreamReader(MemStream);
			reader.BaseStream.Position = 0;

			if (FileName.EndsWith(".gmap"))
			{
				GraalMap map = Server.FindMap(FileName);
				map.Parse(reader);

				foreach (GraalPlayer pl in Server.PlayerManager)
				{
					if (pl.level != null && pl.level.name == map.MapName)
						pl.AdjustLevels();
				}
			}
			else System.Console.WriteLine("File downloaded: " + FileName);
		}

		/// <summary>
		/// Handle Received Data
		/// </summary>
		protected override void HandleData(DataBuffer Packet)
		{
			while (Packet.BytesLeft > 0)
			{
				// Grab Single Packet
				DataBuffer CurPacket = (LastPacketId == 100 ? Packet.Read(ReadCount - 1) : Packet.ReadString('\n'));

				// Read Packet Type
				byte PacketId = CurPacket.ReadGUByte1();

				// Run Internal Packet Function
				switch ((PacketIn)PacketId)
				{
					// Paceket 1 - Level Links
					case PacketIn.LEVELLINKS:
						break;

					// Packet 3 - Level NPC Props
					case PacketIn.LEVELNPCPROPS:
					{
						int npcId = CurPacket.ReadGByte3();
						if (ActiveLevel != null)
							ActiveLevel.GetNPC(npcId).SetProps(CurPacket);
						break;
					}

					// Packet 5 - Level Signs
					case PacketIn.LEVELSIGNS:
						break;

					// Packet 6 - Set Active Level & Clear
					case PacketIn.LEVELNAME:
						ActiveLevel = Server.FindLevel(CurPacket.ReadString().Text);
						break;

					// Packet 8 - Set Player Props
					case PacketIn.OTHERPLPROPS:
						ActivePlayer = Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());
						if (ActivePlayer != null)
							ActivePlayer.SetProps(CurPacket);
						break;

					// Packet 28 - Set Player Flag
					case PacketIn.PLFLAGSET:
					{
						ActivePlayer = Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());
						String FlagName = CurPacket.ReadString('=').Text;
						String FlagValue = CurPacket.ReadString().Text;
						if (ActivePlayer != null)
							ActivePlayer.flags[FlagName] = FlagValue;
						break;
					}

					// Packet 33 - Add weapon to list
					case PacketIn.NPCWEAPONADD:
					{
						String WeaponName = CurPacket.ReadChars(CurPacket.ReadGUByte1());
						String WeaponImage = CurPacket.ReadChars(CurPacket.ReadGUByte1());
						String WeaponScript = CurPacket.ReadString().Text;
						Server.SetWeapon(WeaponName, WeaponImage, WeaponScript, false);
						break;
					}

					// Packet 34 - Add/Remove weapon from Player
					case PacketIn.PLWEAPONMNG:
					{
						ActivePlayer = Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());
						if (ActivePlayer != null)
						{
							bool addWeapon = (CurPacket.ReadGByte1() > 0);
							String name = CurPacket.ReadString().Text;
							if (addWeapon)
								ActivePlayer.AddWeapon(name);
							else
								ActivePlayer.DeleteWeapon(name, false);
						}

						break;
					}

					// Packet 37 - Private Message
					case PacketIn.PRIVATEMESSAGE:
						short PlayerId = CurPacket.ReadGByte2();
						DataBuffer Message = CurPacket.ReadString();
						Server.SendPM(PlayerId, Server.NCMsg, true);
						break;

					// Packet 39 - Level Mod Time
					case PacketIn.LEVELMODTIME:
						if (ActiveLevel != null)
							ActiveLevel.SetModTime(CurPacket.ReadGUByte5());
						break;

					// Packet 42 - New World Time
					case PacketIn.NEWWORLDTIME:
						Server.NWTime = CurPacket.ReadGByte4();
						break;

					// Packet 48 - Triggeractions
					case PacketIn.TRIGGERACTION:
					{
						// read data
						short pid = CurPacket.ReadGByte2();
						uint npcId = CurPacket.ReadGUByte3();
						float trigx = (float)CurPacket.ReadGUByte1() / 2.0f;
						float trigy = (float)CurPacket.ReadGUByte1() / 2.0f;
						List<String> actions = CurPacket.ReadString().Text.Split(',').ToList();
						ActivePlayer = Server.PlayerManager.AddPlayer(pid);

						// parse triggeraction
						if (ActivePlayer != null)
						{
							String action = actions[0];
							actions.RemoveAt(0);
							if (actions.Count == 0)
								actions.Add(String.Empty);

							// trigger to weapon
							if (action == "serverside")
							{
								ServerWeapon weapon = Server.FindWeapon(actions[0]); 
								actions.RemoveAt(0);
								if (actions.Count == 0)
									actions.Add(String.Empty);

								if (weapon != null && weapon.ScriptObject != null)
									weapon.ScriptObject.trigger(ActivePlayer, "onActionServerSide", actions.ToArray());

							}
							else // trigger to level
							{
								GraalLevel tmp = ActivePlayer.level;
								if (tmp != null)
									tmp.trigger(trigx, trigy, "onAction" + action, actions.ToArray());
							}
						}
						break;
					}

					// Packet 60 - Set Weapon List
					case PacketIn.PLWEAPONSSET:
					{
						ActivePlayer = Server.PlayerManager.AddPlayer(CurPacket.ReadGByte2());
						if (ActivePlayer != null)
						{
							while (CurPacket.BytesLeft > 0)
								ActivePlayer.AddWeapon(CurPacket.ReadChars(CurPacket.ReadGUByte1()));
						}
						break;
					}

					// Packet 68 - Start Big File Transfer
					case PacketIn.FILEBIGSTART:
						CurPacket.ReadString(); // filename
						break;

					// Packet 69 - End Big File Transfer
					case PacketIn.FILEBIGEND:
						CurPacket.ReadString(); // filename
						MemStream.Close();
						break;

					// Packet 100 - Raw Packet Data
					case PacketIn.RAWDATA:
						ReadCount = CurPacket.ReadGByte3();
						break;

					// Packet 101 - Board Data
					case PacketIn.LEVELBOARD:
						if (ActiveLevel != null)
						{
							for (int i = 0; i < 4096; i++)
								ActiveLevel.Tiles[i] = BitConverter.ToInt16(CurPacket.Buffer, 1 + (i * 2));
						}
						break;

					// Packet 102 - File Data
					case PacketIn.FILEDATA:
					{
						// Read Packet Data
						uint modtime = CurPacket.ReadGUByte5(); // modtime
						string file = CurPacket.ReadChars(CurPacket.ReadGUByte1()); // filename
						byte[] buf = CurPacket.ReadString().Buffer; // filedata

						// Small file? Start download
						bool SmallFile = (MemStream == null);
						if (SmallFile)
							this.DownloadStart(file);

						// Write Buffer
						MemStream.Write(buf, 0, buf.Length);

						// Small file? End download
						if (SmallFile)
							this.DownloadEnd();
						break;
					}

					// Packet 113 - Add class to list
					case PacketIn.NPCCLASSADD:
					{
						String ClassName = CurPacket.ReadChars(CurPacket.ReadGUByte1());
						String ClassScript = CurPacket.ReadString().Text;
						Server.SetClass(ClassName, ClassScript.Replace("\xa7", "\n"), false);
						break;
					}

					// Packet 150 - Delete Level NPC
					case PacketIn.LEVELNPCDELETE:
						Server.FindLevel(CurPacket.ReadChars(CurPacket.ReadGUByte1())).DeleteNPC(CurPacket.ReadGByte3());
						break;

					// Packet 156 - Set Active Level
					case PacketIn.LEVELSETACTIVE:
						ActiveLevel = Server.FindLevel(CurPacket.ReadString().Text);
						break;

					default:
						System.Console.WriteLine("GSCONN -> Packet [" + PacketId + "]: " + CurPacket.ReadString().Text);
						break;
				}

				// Last Packet Id
				LastPacketId = PacketId;
			}
		}
	}
}
