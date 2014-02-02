using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenGraal.GraalIM
{
	public class TServerList : IEnumerable
	{
		/// <summary>
		/// Player-List
		/// </summary>
		internal Framework Server;
		public Dictionary<Int16, TServer> serverList = new Dictionary<Int16, TServer>();
		protected Int32 Position = -1;
		protected MainWindow form;

		/// <summary>
		/// Constructor
		/// </summary>
		public TServerList ()
		{ 

		}

		/// <summary>
		/// Iterate Playerlist Manager
		/// </summary>
		public IEnumerator<TServer> GetEnumerator()
		{
			foreach (KeyValuePair<Int16, TServer> server in this.serverList)
				yield return server.Value;
		}

		IEnumerator IEnumerable.GetEnumerator()
		{
			return GetEnumerator();
		}

		/// <summary>
		/// Add Player to Playerlist (or return player)
		/// </summary>
		internal TServer AddTServer(Int16 Id)
		{
			TServer pl = FindTServer(Id);
			if (pl == null)
			{
				TServer TServer = new TServer();
				this.serverList[Id] = TServer;
				//PlayerList2.Add(Player);
				return TServer;
			}

			return pl;
		}

		/// <summary>
		/// Delete Player from Playerlist
		/// </summary>
		internal bool DeleteTServer(Int16 pId)
		{
			return this.serverList.Remove(pId);
		}

		/// <summary>
		/// Find Player by Id
		/// </summary>
		public TServer FindTServer(Int16 Id)
		{
			TServer player;
			if (this.serverList.TryGetValue(Id, out player))
				return player;
			return null;
		}

		/// <summary>
		/// Find Player by Account
		/// </summary>
		/// 

		
		public int count()
		{
			return serverList.Count;
		}

		public void Clear()
		{
			serverList.Clear();
		}
	}
}
