using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;
using OpenGraal.Common.Players;

namespace OpenGraal.GraalIM
{
	public class GraalPlayerList : OpenGraal.Common.Players.GraalPlayerList
	{
		/// <summary>
		/// Player-List
		/// </summary>
		internal Framework Server;
		public Dictionary<Int16, Player> PlayerList = new Dictionary<Int16, Player>();
		public List<Player> PlayerList2 = new List<Player>();
		protected Int32 Position = -1;
		protected Abstraction form;

		/// <summary>
		/// Constructor
		/// </summary>
		public GraalPlayerList() :base()
		{
			this.Server = Framework.GetInstance();
			this.form = Abstraction.GetInstance();
		}

		/// <summary>
		/// Add Player to Playerlist (or return player)
		/// </summary>
		public Player AddPlayer(Int16 Id)
		{
			Player pl = this.FindPlayer(Id);
			if (pl == null)
			{
				Player Player = new Player(Id);
				PlayerList[Id] = Player;
				//PlayerList2.Add(Player);
				return Player;
			}

			return pl;
		}
		/// <summary>
		/// Delete Player from Playerlist
		/// </summary>
		public override bool DeletePlayer(Int16 pId)
		{
			return PlayerList.Remove(pId);
		}

		/// <summary>
		/// Find Player by Id
		/// </summary>
		public Player FindPlayer(Int16 Id)
		{
			Player player;
			if (PlayerList.TryGetValue(Id, out player))
				return player;
			return null;
		}

		/// <summary>
		/// Find Player by Account
		/// </summary>
		public Player FindPlayer(String Account)
		{
			Player rc = new Player(0);
			try
			{
				foreach (KeyValuePair<Int16, Player> Player in PlayerList)
				{
					if (Account != null && Player.Value.Account != null)
					{
						if (Player.Value.Account.ToLower() == Account.ToLower())
						{
							if (Player.Value.Level != null && Player.Value.isLocalPlayer == true)
								return Player.Value;
							else
								rc = Player.Value;
						}
					}
				}
			}
			catch (Exception e)
			{
				return null;
			}
			finally
			{
			}
			return rc;
		}

		/// <summary>
		/// Find Player by Id/Account
		/// </summary>
		public Player FindPlayer(String pAccount, Int16 pId)
		{
			foreach (KeyValuePair<Int16, Player> Player in PlayerList)
			{
				if (Player.Value.Account == pAccount && Player.Value.Id == pId)
					return Player.Value;
			}

			return null;
		}

		public int count()
		{
			return PlayerList.Count;
		}
	}
}
