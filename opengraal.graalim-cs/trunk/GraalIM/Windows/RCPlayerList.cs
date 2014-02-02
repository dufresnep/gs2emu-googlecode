using System;

namespace OpenGraal.GraalIM
{
	public class RCPlayerList  : Gtk.Window
	{

		#region Private variables

		private static RCPlayerList _instance = new RCPlayerList();
		private Framework Framework;
		private Gtk.TreeView _playerList = null;
		private Gtk.ListStore _playerListModel = null;
		private Gtk.ScrolledWindow _playerListScroll = null;
		private Gtk.Notebook _playerlistTabs = null;
		private Gtk.Label _thisServerLabel = null;

		#endregion

		#region Cunstructor/Destructor functions

		public static RCPlayerList GetInstance()
		{
			return _instance;
		}

		private RCPlayerList() : base(Gtk.WindowType.Toplevel)
		{
		}

		public void Init()
		{
			this.Name = "OpenGraal.GraalIM.RCPlayerList";
			this.Title = global::Mono.Unix.Catalog.GetString("Players");
			this.Icon = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");
			this.WidthRequest = 580;
			this.HeightRequest = 420;
			this._playerlistTabs = new global::Gtk.Notebook();
			this._playerlistTabs.CanFocus = true;
			this._playerlistTabs.Name = "_playerlistTabs";
			this._playerlistTabs.CurrentPage = 0;
			this._playerlistTabs.EnablePopup = true;
			this._playerlistTabs.Scrollable = true;

			this._thisServerLabel = new global::Gtk.Label();
			this._thisServerLabel.CanDefault = true;
			this._thisServerLabel.Name = "StatusTabLabel";
			this._thisServerLabel.LabelProp = global::Mono.Unix.Catalog.GetString("This server");

			this._playerList = new Gtk.TreeView();

			this._playerList.SetSizeRequest(230, 230);

			this._playerListModel = new Gtk.ListStore(typeof(Gdk.Pixbuf), typeof(string), typeof(int));
			//this.tree.Selection.Changed += new System.EventHandler(OnSelectionChanged);
			//this._playerList.CursorChanged += new System.EventHandler(OnSelectionChanged);
			/*
			this._playerList.ButtonPressEvent += new ButtonPressEventHandler(delegate(object o, ButtonPressEventArgs args)
			                                                          {
				System.Console.WriteLine(args.ToString());
			});
			*/
			Gtk.CellRendererPixbuf pixbufrender = new Gtk.CellRendererPixbuf();
			Gtk.TreeViewColumn TypeCol = this._playerList.AppendColumn("", new Gtk.CellRendererPixbuf(), "pixbuf", 0);
			TypeCol.SortIndicator = true;
			TypeCol.SortColumnId = 0;
			Gtk.CellRendererPixbuf cellpb = new Gtk.CellRendererPixbuf();
			Gtk.CellRendererText cell = new Gtk.CellRendererText();
			TypeCol.PackStart(cellpb, false);
			TypeCol.PackStart(cell, false);

			Gtk.CellRendererText NickColText = new Gtk.CellRendererText();
			
			Gtk.TreeViewColumn NickCol = new Gtk.TreeViewColumn ();
			NickCol.PackStart(NickColText,true);
			NickCol.Title = "Artist";
			NickCol.SortIndicator = true;
			NickCol.SortColumnId = 1;
		
			NickCol.AddAttribute(NickColText, "text", 1);
			this._playerList.AppendColumn(NickCol);

			Gtk.TreeViewColumn AccCol = this._playerList.AppendColumn("Account", new Gtk.CellRendererText(), "text", 2);
			AccCol.SortIndicator = true;
			AccCol.SortColumnId = 2;
			Gtk.TreeViewColumn LvlCol = this._playerList.AppendColumn("Level", new Gtk.CellRendererText(), "text", 3);
			LvlCol.SortIndicator = true;
			LvlCol.SortColumnId = 3;
			Gtk.TreeViewColumn IdCol = this._playerList.AppendColumn("ID", new Gtk.CellRendererText(), "text", 4);
			IdCol.SortIndicator = true;
			IdCol.SortColumnId = 4;

			Gtk.TreeIter iter = _playerListModel.AppendValues("Admins");
			_playerListModel.AppendValues(iter, "Fannypack", "Nu Nu (Yeah Yeah) (double j and haze radio edit)");

			iter = _playerListModel.AppendValues("Players");
			_playerListModel.AppendValues(iter, "Nelly", "Country Grammer");

			this._playerList.FixedHeightMode = false;
			this._playerList.HeadersClickable = true;
			this._playerList.SearchColumn = 1;
			
			this._playerList.EnableSearch = true;
			this._playerList.EnableTreeLines = true;
			this._playerList.ShowExpanders = true;
			//this.tree.Vadjustment.
			//_playerListModel.AppendValues("", "Loading...", "");

			Gtk.Frame frame2 = new Gtk.Frame();
			this._playerList.Model = _playerListModel;

			this._playerListScroll = new global::Gtk.ScrolledWindow();
			this._playerListScroll.Name = "GtkScrolledWindow";
			this._playerListScroll.ShadowType = ((global::Gtk.ShadowType)(1));

			// Container child GtkScrolledWindow.Gtk.Container+ContainerChild

			this._playerListScroll.Add(this._playerList);

			this._playerlistTabs.Add(this._playerListScroll);
			this._playerlistTabs.SetTabLabel(this._playerListScroll, this._thisServerLabel);
			this._thisServerLabel.ShowAll();
			this.Add(this._playerlistTabs);
		}

		#endregion

		public void RMPlayer(string account)
		{
			Gtk.Application.Invoke(delegate
			{
				Player player = (Player)this.Framework.PlayerManager.FindPlayer(account);
				/*
				TreeNode[] player2 = this.Players.Nodes.Find(account,true);
				TreeNode[] rc = this.RCs.Nodes.Find(account,true);

				if (player2.Length == 0)
				{
					// Do nothing
				}
				else
				{
					player2[0].Remove();
				}

				if (rc.Length == 0)
				{
					// Do nothing
				}
				else
				{
					rc[0].Remove();
				}
				*/
			}
			);
		}

		public void AddPlayer(string account, string nickname)
		{
			this.Framework = Framework.GetInstance();

			nickname = (nickname != null) ? nickname : "unknown";
			if (account != null)
			{
				Gtk.Application.Invoke(delegate
				{
					Player player = (Player)this.Framework.PlayerManager.FindPlayer(account);
					/*
					if (player != null && player.Account != null)
					{

						TreeNode[] player2 = this.Players.Nodes.Find(account, true);
						TreeNode[] rc = this.RCs.Nodes.Find(account, true);
						TreeNode[] irc = this.IRC.Nodes.Find(account, true);
						if (player.Account.StartsWith("irc:"))
						{
							if (irc.Length == 0)
							{
								if (player.Account != null && player.Level != null && player.Nickname != null)
									this.IRC.Nodes.Add(player.Account.ToString(), player.Nickname.ToString());
								else if (player.Account != null && player.Level != null && player.Nickname == "")
									this.RCs.Nodes.Add(player.Account.ToString(), player.Account.Split(':')[1].ToString());
							}
							else
							{
								irc[0].Text = player.Nickname;
							}
						}
						else
						{

							if (player2.Length == 0)
							{
								if (player.Account != null && player.Level != null && player.Nickname != null)
									this.Players.Nodes.Add(player.Account.ToString(), player.Nickname);
								else if (player.Account != null)
									this.Players.Nodes.Add(player.Account.ToString(), "*" + player.Account.ToString());
							}
							else
							{
								player2[0].Text = player.Nickname;
							}

							if (rc.Length == 0)
							{
								if (player.Account != null && player.Level != null && player.Nickname != null)
									this.RCs.Nodes.Add(player.Account.ToString(), player.Nickname.ToString());
								else if (player.Account != null && player.Level != null && player.Nickname == "")
									this.RCs.Nodes.Add(player.Account.ToString(), "*" + player.Account.ToString());
							}
							else
							{
								rc[0].Text = player.Nickname;
							}
						}
					}
					*/
				}
				);
			}
		}
	}
}

