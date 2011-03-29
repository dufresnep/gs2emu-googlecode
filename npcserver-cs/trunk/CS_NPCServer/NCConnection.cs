using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using CS_NPCServer;

namespace CS_NPCServer
{
	public class NCConnection : GraalSocket
	{
		#region Enumerators		/// <summary>
		/// Enumerator -> Packet In
		/// </summary>
		public enum PacketIn
		{
			NC_NPCGET = 103,	// {103}{INT id}
			NC_NPCDELETE = 104,	// {104}{INT id}
			NC_NPCRESET = 105,	// {105}{INT id}
			NC_NPCSCRIPTGET = 106,	// {106}{INT id}
			NC_NPCWARP = 107,	// {107}{INT id}{CHAR x*2}{CHAR y*2}{level}
			NC_NPCFLAGSGET = 108,	// {108}{INT id}
			NC_NPCSCRIPTSET = 109,	// {109}{INT id}{GSTRING script}
			NC_NPCFLAGSSET = 110,	// {110}{INT id}{GSTRING flags}
			NC_NPCADD = 111,	// {111}{GSTRING info}  - (info) name,id,type,scripter,starting level,x,y
			NC_CLASSGET = 112,	// {112}{class}
			NC_CLASSADD = 113,	// {113}{CHAR name length}{name}{GSTRING script}
			NC_LOCALNPCSGET = 114,	// {114}{level}
			NC_WEAPONLISTGET = 115,	// {115}
			NC_WEAPONGET = 116,	// {116}{weapon}
			NC_WEAPONADD = 117,	// {117}{CHAR weapon length}{weapon}{CHAR image length}{image}{code}
			NC_WEAPONDELETE = 118,	// {118}{weapon}
			NC_CLASSDELETE = 119,	// {119}{class}
			NC_LEVELLISTGET = 150,	// {150}
			NC_LEVELLISTSET = 151,	// {151}{GSTRING levels}
		};

		/// <summary>
		/// Enumerator -> Packet Out
		/// </summary>
		public enum PacketOut
		{
			NC_CHAT = 74,	// {74}{GSTRING text}
			NC_LEVELLIST = 80,	// {80}{GSTRING levels}
			NC_NPCATTRIBUTES = 157,	// {157}{GSTRING attributes}
			NC_NPCADD = 158,	// {158}{INT id}{CHAR 50}{CHAR name length}{name}{CHAR 51}{CHAR type length}{type}{CHAR 52}{CHAR level length}{level}
			NC_NPCDELETE = 159,	// {159}{INT id}
			NC_NPCSCRIPT = 160,	// {160}{INT id}{GSTRING script}
			NC_NPCFLAGS = 161,	// {161}{INT id}{GSTRING flags}
			NC_CLASSGET = 162,	// {162}{CHAR name length}{name}{GSTRING script}
			NC_CLASSADD = 163,	// {163}{class}
			NC_LEVELDUMP = 164,
			NC_WEAPONLISTGET = 167,	// {167}{CHAR name1 length}{name1}{CHAR name2 length}{name2}...
			NC_CLASSDELETE = 188,	// {188}{class}
			NC_WEAPONGET = 192,	// {192}{CHAR name length}{name}{CHAR image length}{image}{script}
		};
		#endregion


		/// <summary>
		/// Member Variables
		/// </summary>
		protected NPCServer Server;
		public bool LoggedIn = false;
		public string Account, Password;

		/// <summary>
		/// Base Constructor
		/// </summary>
		public NCConnection(NPCServer Server, Socket Sock)
			: base(Sock)
		{
			this.Server = Server;
		}

		/// <summary>
		/// Handle Login Packet
		/// </summary>
		protected void HandleLogin(DataBuffer LoginPacket)
		{
			// Check Type & Version
			Int32 type = LoginPacket.ReadGUByte1();
			String version = LoginPacket.ReadChars(8);
			if (type != 3 || version != "NCL21075")
			{
				SendPacket(new DataBuffer() + (byte)16 + "Your nc version is not allowed on this server.", true);
				this.Disconnect();
				return;
			}

			// Send Login data for verification
			Account = LoginPacket.ReadChars(LoginPacket.ReadGUByte1());
			Password = LoginPacket.ReadChars(LoginPacket.ReadGUByte1());
			//server->sendToGserver(CString() >> (char)GO_REQUEST_RIGHTS << account);

			// Send NC Login
			SendPacket(new DataBuffer() + (byte)PacketOut.NC_CHAT + "Welcome to the NPC-Server for NPC-Server Test");
			Server.SendNCChat("New NC: " + Account, this);

			// Send Current NC's
			foreach (NCConnection nc in Server.NCList)
			{
				if (nc != this)
					SendPacket(new DataBuffer() + (byte)PacketOut.NC_CHAT + "New NC: " + Account);
			}

			// Send Classes
			foreach (KeyValuePair<string, ServerClass> obj in Server.ClassList)
				SendPacket(new DataBuffer() + (byte)PacketOut.NC_CLASSADD + obj.Value.Name);

			/*
			// Send Classes
			std::map<CString, TScriptClass *> *classList = mServer->getClassList();
			for (std::map<CString, TScriptClass *>::const_iterator i = classList->begin(); i != classList->end(); ++i)
				sendPacket(CString() >> (char)PLO_NC_CLASSADD << i->first);

			// Send Npcs
			std::map<int, TScriptNPC *> *npcList = mServer->getNpcList();
			for (std::map<int, TScriptNPC *>::const_iterator i = npcList->begin(); i != npcList->end(); ++i)
				sendPacket(CString() << i->second->GetNpcPacket());
			*/

			// Set Login
			LoggedIn = true;
		}

		/// <summary>
		/// Handle Received Data
		/// </summary>
		protected override void HandleData(DataBuffer Packet)
		{
			// Player not logged in
			if (!LoggedIn)
				HandleLogin(Packet.ReadString('\n'));

			// Parse Packets
			while (Packet.BytesLeft > 0)
			{
				// Grab Single Packet
				DataBuffer CurPacket = Packet.ReadString('\n');

				// Read Packet Type
				int PacketId = CurPacket.ReadGUByte1();

				// Call Packet Callback
				//RemoteControl.CallCallBack(PacketId, (DataBuffer)CurPacket.DeepClone());

				// Run Internal Packet Function
				switch ((PacketIn)PacketId)
				{
					// Retrieve Class
					case PacketIn.NC_CLASSGET:
					{
						String Name = CurPacket.ReadString().Text;
						ServerClass Class = Server.FindClass(Name);
						if (Class != null)
							SendPacket(new DataBuffer() + (byte)PacketOut.NC_CLASSGET + (byte)Class.Name.Length + Class.Name + DataBuffer.tokenize(Class.Script));
						else
							Server.SendNCChat(Account + " prob: script " + Name + " doesn't exist", null);
						break;
					}

					// Set Class
					case PacketIn.NC_CLASSADD:
					{
						String ClassName = CurPacket.ReadChars(CurPacket.ReadGUByte1());
						String ClassScript = CurPacket.ReadString().Text;
						int res = Server.SetClass(ClassName, DataBuffer.untokenize(ClassScript), true);
						if (res >= 0)
							Server.SendNCChat("Script " + ClassName + " " + (res == 1 ? "added" : "updated") + " by " + this.Account);
						break;
					}

					// Delete Class
					case PacketIn.NC_CLASSDELETE:
					{
						String ClassName = CurPacket.ReadString().Text;
						if (Server.DeleteClass(ClassName))
							Server.SendNCChat("Script " + ClassName + " deleted by " + this.Account);
						else
							Server.SendNCChat(Account + " prob: script " + ClassName + " doesn't exist");
						break;
					}

					// Retrieve Weapon List
					case PacketIn.NC_WEAPONLISTGET:
					{
						DataBuffer Out = new DataBuffer() + (byte)PacketOut.NC_WEAPONLISTGET;
						foreach (KeyValuePair<string, ServerWeapon> weapon in Server.WeaponList)
						{
							Out.WriteGByte1((sbyte)weapon.Key.Length);
							Out.Write(weapon.Key);
						}
						SendPacket(Out);
						break;
					}

					// Retrieve Weapon
					case PacketIn.NC_WEAPONGET:
					{
						String Name = CurPacket.ReadString().Text;
						ServerWeapon Weapon = Server.FindWeapon(Name);
						if (Weapon != null)
							SendPacket(new DataBuffer() + (byte)PacketOut.NC_WEAPONGET + (byte)Weapon.Name.Length + Weapon.Name + (byte)Weapon.Image.Length + Weapon.Image + Weapon.Script.Replace("\n", "\xa7"));
						else
							Server.SendNCChat(Account + " prob: weapon " + Name + " doesn't exist", null);
						break;
					}

					// Set Weapon
					case PacketIn.NC_WEAPONADD:
					{
						String WeaponName = CurPacket.ReadChars(CurPacket.ReadGUByte1());
						String WeaponImage = CurPacket.ReadChars(CurPacket.ReadGUByte1());
						String WeaponScript = CurPacket.ReadString().Text;
						int res = Server.SetWeapon(WeaponName, WeaponImage, WeaponScript, true);
						if (res >= 0)
							Server.SendNCChat("Weapon/GUI-script " + WeaponName + " " + (res == 1 ? "added" : "updated") + " by " + this.Account);
						break;
					}

					// Delete Weapon
					case PacketIn.NC_WEAPONDELETE:
					{
						String WeaponName = CurPacket.ReadString().Text;
						if (Server.DeleteWeapon(WeaponName))
							Server.SendNCChat("Weapon " + WeaponName + " deleted by " + this.Account);
						else
							Server.SendNCChat(Account + " prob: weapon " + WeaponName + " doesn't exist");
						break;
					}

					default:
						System.Console.WriteLine("CLIENTNC -> Packet [" + PacketId + "]: " + CurPacket.Text);
						break;
				}
			}
		}
	}
}
