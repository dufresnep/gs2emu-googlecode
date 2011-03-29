using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using CS_NPCServer;

namespace CS_NPCServer
{
	public class NPCServer
	{
		/// <summary>
		/// Member Variables
		/// </summary>
		protected AsyncCallback cNCAccept;
		internal GameCompiler Compiler;
		private IniFile ConfigFile;
		protected GServerConnection GSConn;
		protected TcpListener NCListen;
		
		protected int TimerId;
		protected TimerEventHandler TimerHandle;
		internal object TimerLock = new object();
 
		// NC Variables
		public int NWTime = 0;
		public List<NCConnection> NCList = new List<NCConnection>();
		public String NCMsg = "I am the npcserver for\",\"this game server. Almost\",\"all npc actions are controled\",\"by me.";

		// Dictionarys
		internal Dictionary<int, ScriptObj> ScriptList = new Dictionary<int, ScriptObj>();
		internal Dictionary<String, GraalLevel> LevelList = new Dictionary<String, GraalLevel>();
		internal Dictionary<String, GraalMap> MapList = new Dictionary<String, GraalMap>();
		internal Dictionary<String, ServerClass> ClassList = new Dictionary<String, ServerClass>();
		internal Dictionary<String, ServerWeapon> WeaponList = new Dictionary<String, ServerWeapon>();
		internal GraalPlayerList PlayerManager;

		/// <summary>
		/// Variable -> NPC Port
		/// </summary>
		public int NPCPort
		{
			get { return ConfigFile.GetValueInt("server", "nc_port", 14852); }
		}

		/// <summary>
		/// Creates a new NPCServer
		/// </summary>
		internal NPCServer(string OptionsFile)
		{
			// Load Options
			ConfigFile = new IniFile();
			ConfigFile.Load(OptionsFile);

			// Create Compiler / GServer Connection / PlayerManager
			Compiler = new GameCompiler(this);
			GSConn = new GServerConnection(this);
			PlayerManager = new GraalPlayerList(this);

			// Setup Timer
			timeBeginPeriod(50);
			TimerHandle = new TimerEventHandler(RunScripts);
			TimerId = timeSetEvent(50, 0, TimerHandle, IntPtr.Zero, EVENT_TYPE);
		}

		/// <summary>
		/// Function -> Connect to Server
		/// </summary>
		/// <returns></returns>
		internal bool Connect()
		{
			// already connected?
			if (GSConn.Connected)
				return false;

			// read settings
			String addr = ConfigFile.GetValue("server", "server_ip", "localhost");
			String key = ConfigFile.GetValue("server", "server_key", "testkey");
			int port = ConfigFile.GetValueInt("server", "server_port", 14800);
			int ncport = this.NPCPort;
			System.Console.WriteLine("Connecting to " + addr + ":" + port);
			
			// connect to server
			GSConn.Connect(addr, port);
			if (GSConn.Connected)
			{
				GSConn.SendLogin("(npcserver)", key, "NPC-Server (Server)");
				GSConn.ReceiveData();
				System.Console.WriteLine("Connected to server");
			}

			// Setup NPC-Control Listener
			cNCAccept = new AsyncCallback(NCControl_Accept);
			NCListen = new TcpListener(IPAddress.Parse("0.0.0.0"), ncport);
			NCListen.Start();
			NCListen.BeginAcceptSocket(cNCAccept, NCListen);
			return true;
		}

		/// <summary>
		/// Detructor -> Cleanup
		/// </summary>
		~NPCServer()
		{
			timeKillEvent(this.TimerId);
			timeEndPeriod(50);
		}

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

		public bool RunServer()
		{
			// Manage Active Compilers
			this.Compiler.ManageCompilers();

			// Send Prop Changes
			this.RunPropChanges();
			return true;
		}

		/// <summary>
		/// Run NPC Scripts
		/// </summary>
		public void RunScripts(int id, int msg, IntPtr user, int dw1, int dw2)
		{
			// Run Timeouts
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
		}

		/// <summary>
		/// Run Prop Changes
		/// </summary>
		public void RunPropChanges()
		{
			// send player prop changes
			foreach (GraalPlayer pl in PlayerManager)
				pl.SendPropBuffer();
		}

		/// <summary>
		/// Check if level is already loaded
		/// </summary>
		public bool LevelExists(String Name)
		{
			return LevelList.ContainsKey(Name);
		}

		/// <summary>
		/// FindLevel
		/// </summary>
		public GraalLevel FindLevel(String Name)
		{
			GraalLevel Level = null;
			if (Name != String.Empty && !LevelList.TryGetValue(Name, out Level))
			{
				Level = new GraalLevel(this, Name);
				lock (TimerLock)
				{
					LevelList[Name] = Level;
				}
			}

			return Level;
		}

		/// <summary>
		/// FindMap
		/// </summary>
		public GraalMap FindMap(String Name)
		{
			GraalMap Map = null;
			if (!MapList.TryGetValue(Name, out Map))
			{
				Map = new GraalMap(Name);
				MapList[Name] = Map;
			}

			return Map;
		}
		
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
				SendPacket(new DataBuffer() + (byte)NCConnection.PacketOut.NC_CLASSDELETE + ClassName, null);
				SendGSPacket(new DataBuffer() + (byte)GServerConnection.PacketOut.NCQUERY + (byte)GServerConnection.NCREQ.CLASSSET + (byte)ClassName.Length + ClassName);
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
				SendGSPacket(new DataBuffer() + (byte)GServerConnection.PacketOut.NCQUERY + (byte)GServerConnection.NCREQ.WEAPONDEL + WeaponName);
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
				SendPacket(new DataBuffer() + (byte)NCConnection.PacketOut.NC_CLASSADD + ClassName, null);
			}

			// Valid Weapon?
			if (Class != null)
			{
				// Send to GServer
				if (SendToGS)
					this.SendGSPacket(new DataBuffer() + (byte)GServerConnection.PacketOut.NCQUERY + (byte)GServerConnection.NCREQ.CLASSSET + (byte)ClassName.Length + ClassName + ClassScript.Replace("\r", "").Replace("\n", "\xa7"));
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
					this.SendGSPacket(new DataBuffer() + (byte)GServerConnection.PacketOut.NCQUERY + (byte)GServerConnection.NCREQ.WEAPONADD + (byte)WeaponName.Length + WeaponName + (byte)WeaponImage.Length + WeaponImage + WeaponCode);
					Compiler.CompileAdd(Weapon);
				}
			}

			// Added or Updated?
			return Status;
		}

		/// <summary>
		/// Send Text to NPC-Controls
		/// </summary>
		public void SendNCChat(String Message) { this.SendNCChat(Message, null); }
		public void SendNCChat(String Message, NCConnection From)
		{
			SendPacket(new DataBuffer() + (byte)NCConnection.PacketOut.NC_CHAT + Message, From);
		}

		/// <summary>
		/// Send Private Message to Player
		/// </summary>
		public void SendPM(short PlayerId, String Message, bool IsTokenized)
		{
			GSConn.SendPacket(new DataBuffer() + (byte)GServerConnection.PacketOut.NCQUERY + (byte)GServerConnection.NCREQ.SENDPM + (short)PlayerId + "\"\"," + (IsTokenized ? Message : DataBuffer.tokenize(Message)));
		}

		/// <summary>
		/// Send Text to Remote-Controls
		/// </summary>
		public void SendRCChat(String Message)
		{
			GSConn.SendPacket(new DataBuffer() + (byte)GServerConnection.PacketOut.RCCHAT + Message);
		}

		/// <summary>
		/// Send Packet to GServer
		/// </summary>
		public void SendGSPacket(DataBuffer Packet)
		{
			GSConn.SendPacket(Packet);
		}

		/// <summary>
		/// Send Packet to everyone but sender
		/// </summary>
		public void SendPacket(DataBuffer Packet, NCConnection Sender)
		{
			lock (NCList)
			{
				foreach (NCConnection nc in NCList)
				{
					if (nc != Sender)
						nc.SendPacket(Packet, true);
				}
			}
		}

		// P/Invoke declarations
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
	}
}
