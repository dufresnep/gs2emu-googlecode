using System;
using System.Collections.Generic;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;
using OpenGraal.Common.Players;
using OpenGraal.Common.Connections.Client;

namespace OpenGraal.GraalIM
{
	public interface IGameServer 
	{
		bool Connected { get; }
		
		void Connect(System.Net.IPHostEntry ip, Int32 Port);

		void Connect(System.Net.IPAddress addr, Int32 Port);
		void Connect(String Hostname, Int32 Port);

		void Disconnect();

		void RunServer();

		void SendPacket(CString packet);

		void SendPacket(CString packet, bool test);

		void SendLogin(String Account, String Password, String Nickname);

		void ReceivedServerFlags(int serverFlagsTotal, string serverFlagsString);

		void ReceivedFolderConfig(string folderConfig);

		void ReceivedPM(string PlayerId, CString Message);

		void ReceivedPM(short PlayerId, CString Message);

		void ReceivedPM(Player Player, CString Message);

		void ReceivedToall(short PlayerId, CString Message);

		void ReceivedToall(Player Player, CString Message);
		void WriteText(string text);
		void ReceivedRCChat(string text);
		void ReceivedSignature();
	}
}
