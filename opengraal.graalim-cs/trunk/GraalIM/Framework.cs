using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;
using OpenGraal.Common.Players;
using OpenGraal.Common.Levels;

namespace OpenGraal.GraalIM
{
	public class Framework
	{
		/// <summary>
		/// Member Variables
		/// </summary>
		protected AsyncCallback cNCAccept;
		protected IGameServer GSConn;
		protected TcpListener NCListen;
		private static Framework _instance = new Framework();
		// NC Variables
		public int NWTime = 0;
		//public List<NCConnection> NCList = new List<NCConnection>();
		public String NCMsg;
		// Dictionarys
		public GraalPlayerList PlayerManager = new GraalPlayerList();
		public string nickname = "Guest";
		public string username = "guest";
		public string password = "7Y3bdzjf";
		public string server = "loginserver.graalonline.com";

		public int port = 14900;
		private Dictionary<String, GraalLevel> LevelList = new Dictionary<String, GraalLevel>();
		protected Abstraction form;
		protected ServerWindow serverWindow;

		public static Framework GetInstance()
		{
			return _instance;
		}

		/// <summary>
		/// Creates a new Framework
		/// </summary>
		private Framework()
		{
			//this.RunServer();

			// Setup Timer
			/*
			timeBeginPeriod(50);
			TimerHandle = new TimerEventHandler(RunServer);
			TimerId = timeSetEvent(50, 0, TimerHandle, IntPtr.Zero, EVENT_TYPE);
			 * */
		}

		/// <summary>
		/// Detructor -> Cleanup
		/// </summary>
		~Framework()
		{
			/*
			timeKillEvent(this.TimerId);
			timeEndPeriod(50);
			*/
		}
		/*
		/// <summary>
		/// Accept incoming connection for NPC-Control
		/// </summary>
		/// <param name="ar"></param>
		protected void NCControl_Accept(IAsyncResult ar)
		{
			// Accept Socket
			NCConnection Client = new NCConnection(this, (Socket)NCListen.EndAcceptSocket(ar));
			Client.ReceiveData();
			NCList.Add(Client);

			// Listen for incoming connections
			NCListen.BeginAcceptSocket(cNCAccept, NCListen);
		}
		*/
		/// <summary>
		/// Run NPC Server
		/// </summary>
		public void RunServer()
		{
			// Run Timeouts
			/*
			foreach (KeyValuePair<String, ServerWeapon> wep in WeaponList)
			{
				ScriptObj obj = (ScriptObj)wep.Value.ScriptObject;
				if (obj != null)
					obj.RunEvents();
			}
			
			// Level-Npcs
			lock (TimerLock)
			{
				foreach (KeyValuePair<String, GraalLevel> lvl in LevelList)
				{
					foreach (KeyValuePair<int, GraalLevelNPC> npc in lvl.Value.NpcList)
					{
						ScriptObj obj = (ScriptObj)npc.Value.ScriptObject;
						if (obj != null)
							obj.RunEvents();
					}
				}
			}
			
			while (Compiler.RunList.Count > 0)
			{
				IRefObject obj;
				lock (Compiler.RunList)
					obj = Compiler.RunList.Dequeue();
				obj.Call("onCreated", null);
			}
			*/
			this.form = Abstraction.GetInstance();

			this.form.Activate();
			this.form.WriteText("--- OpenGraal Instant Messenger / Remote Control ---");
			this.serverWindow = ServerWindow.GetInstance();
			// Default PM
			this.NCMsg = CString.tokenize("I am the Framework for\nthis game server. Almost\nall npc actions are controled\nby me.");

			// Create Compiler
			//Compiler = new GameCompiler(this);

			// Create Player Manager
			PlayerManager = new GraalPlayerList();
			
			// Connect to GServer
			GSConn = (IGameServer)new Connections.GraalServer();
			GSConn.RunServer();
			
			if (this.serverWindow.nickname != null && this.serverWindow.nickname != "")
				this.nickname = this.serverWindow.nickname;
			
			if (this.serverWindow.username != null && this.serverWindow.username != "")
				this.username = this.serverWindow.username;

			if (this.serverWindow.password != null && this.serverWindow.password != "")
				this.password = this.serverWindow.password;

			if (this.serverWindow.gserver != null && this.serverWindow.gserver != "")
				server = this.serverWindow.gserver;

			if (this.serverWindow.gport != null && this.serverWindow.gport != 0)
				this.port = this.serverWindow.gport;

			//GSConn.Disconnect();
			if (!GSConn.Connected)
			{
				this.form.WriteText("Not connected, connecting...");
				this.form.WriteText("Address:\t" + this.server);
				this.form.WriteText("Port:\t\t" + this.port.ToString());
				GSConn.Connect(this.server, this.port);

			}
			else
				this.form.WriteText("Already connected!");

			if (GSConn.Connected)
			{
				GSConn.SendLogin(this.username, this.password, this.nickname);
			}
		}

		/// <summary>
		/// Run NPC Server
		/// </summary>
		public void CloseServers()
		{
			this.GSConn.Disconnect();
			this.GSConn = null;
			//this.NCListen.Stop();
		}

		/// <summary>
		/// FindLevel
		/// </summary>
		/// <param name="Name"></param>
		/// <returns></returns>
		public GraalLevel FindLevel(String Name)
		{
			GraalLevel Level = null;
			
			if (!LevelList.TryGetValue(Name, out Level))
			{
				Level = new GraalLevel(Name, new object());
				
				//lock (TimerLock)
				//{
				LevelList[Name] = Level;
				//}
				

			}
			

			return Level;
		}
		/*
		/// <summary>
		/// FindClass
		/// </summary>
		public ServerClass FindClass(String Name)
		{
			ServerClass Class = null;
			if (ClassList.TryGetValue(Name, out Class))
				return Class;
			return null;
		}
	
		/// <summary>
		/// FindWeapon
		/// </summary>
		public ServerWeapon FindWeapon(String Name)
		{
			ServerWeapon Weapon = null;
			if (WeaponList.TryGetValue(Name, out Weapon))
				return Weapon;
			return null;
		}

		/// <summary>
		/// Delete Class
		/// </summary>
		public bool DeleteClass(String ClassName)
		{
			if (ClassList.Remove(ClassName))
			{
				SendPacket(new CString() + (byte)NCConnection.PacketOut.NC_CLASSDELETE + ClassName, null);
				// TODO: Send to GServer
				return true;
			}
			return false;
		}

		/// <summary>
		/// Delete Weapon
		/// </summary>
		public bool DeleteWeapon(String WeaponName)
		{
			if (WeaponList.Remove(WeaponName))
			{
				SendGSPacket(new CString() + (byte)GServerConnection.PacketOut.NC_NPCGET + (byte)GServerConnection.NCREQ.WEAPONDEL + WeaponName);
				return true;
			}
			return false;
		}

		/// <summary>
		/// Set Class Data (and compile+exec)
		/// </summary>
		public int SetClass(String ClassName, String ClassScript, bool SendToGS)
		{
			// Set Weapon Data
			int Status = 0;
			ServerClass Class = null;
			if (ClassList.TryGetValue(ClassName, out Class))
			{
				Class.UpdateClass(ClassName, ClassScript);
				Status = 1;
			}
			else
			{
				Class = new ServerClass(ClassName, ClassScript);
				ClassList[ClassName] = Class;
				SendPacket(new CString() + (byte)NCConnection.PacketOut.NC_CLASSADD + ClassName, null);
			}

			// Valid Weapon?
			if (Class != null)
			{
				// Send to GServer
				//if (SendToGS)
				//	this.SendGSPacket(new CString() + (byte)GServerConnection.PacketOut.NC_NPCGET + (byte)GServerConnection.NCREQ.WEAPONADD + (byte)ClassName.Length + ClassName + ClassScript);
			}

			// Added or Updated?
			return Status;
		}

		/// <summary>
		/// Set Weapon Data (and compile+exec)
		/// </summary>
		public int SetWeapon(String WeaponName, String WeaponImage, String WeaponCode, bool SendToGS)
		{
			// Set Weapon Data
			int Status = 0;
			ServerWeapon Weapon = null;
			if (WeaponList.TryGetValue(WeaponName, out Weapon))
			{
				Weapon.UpdateWeapon(WeaponName, WeaponImage, WeaponCode);
				Status = 1;
			}
				else
			{
				Weapon = new ServerWeapon(this, WeaponName, WeaponImage, WeaponCode);
				WeaponList[WeaponName] = Weapon;
			}

			// Valid Weapon?
			if (Weapon != null)
			{
				// Send to GServer
				if (SendToGS)
				{
					this.SendGSPacket(new CString() + (byte)GServerConnection.PacketOut.NC_NPCGET + (byte)GServerConnection.NCREQ.WEAPONADD + (byte)WeaponName.Length + WeaponName + (byte)WeaponImage.Length + WeaponImage + WeaponCode);
					Compiler.CompileAdd(Weapon);
				}
			}

			// Added or Updated?
			return Status;
		}
		*/
		/*
		/// <summary>
		/// Send Text to NPC-Controls
		/// </summary>
		public void SendNCChat(String Message) { this.SendNCChat(Message, null); }
		public void SendNCChat(String Message, NCConnection From)
		{
			SendPacket(new CString() + (byte)NCConnection.PacketOut.NC_CHAT + Message, From);
		}
		*/
		/// <summary>
		/// Send Private Message to Player
		/// </summary>
		public void SendPM(short PlayerId, String Message, bool IsTokenized)
		{
			GSConn.SendPacket(new CString() + (byte)28 + (short)1 + (short)PlayerId + "\"\"," + (IsTokenized ? Message : CString.tokenize(Message)));
		}

		/// <summary>
		/// Send Text to Remote-Controls
		/// </summary>
		public void SendRC_CHAT(String Message)
		{
			GSConn.SendPacket(new CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.RC_CHAT + Message);
		}

		/// <summary>
		/// Send Packet to GServer
		/// </summary>
		public void SendGSPacket(CString Packet)
		{
			GSConn.SendPacket(Packet, true);
		}
		/*
		/// <summary>
		/// Send Packet to everyone but sender
		/// </summary>
		public void SendPacket(CString Packet, NCConnection Sender)
		{
			foreach (NCConnection nc in NCList)
			{
				if (nc != Sender)
					nc.SendPacket(Packet, true);
			}
		}
		*/
		// P/Invoke declarations
		/*
		protected delegate void TimerEventHandler(int id, int msg, IntPtr user, int dw1, int dw2);
		protected const int TIME_PERIODIC = 1;
		protected const int EVENT_TYPE = TIME_PERIODIC;// + 0x100;  // TIME_KILL_SYNCHRONOUS causes a hang ?!
		[DllImport("winmm.dll")]
		protected static extern int timeSetEvent(int delay, int resolution, TimerEventHandler handler, IntPtr user, int eventType);
		[DllImport("winmm.dll")]
		protected static extern int timeKillEvent(int id);
		[DllImport("winmm.dll")]
		protected static extern int timeBeginPeriod(int msec);
		[DllImport("winmm.dll")]
		protected static extern int timeEndPeriod(int msec);
		*/
	}
}
