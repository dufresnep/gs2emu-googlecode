using System;
using System.Threading;

//using System.Windows.Forms;
namespace OpenGraal.GraalIM
{
	public partial class MainWindow : Gtk.Window
	{
		private static MainWindow _instance = new MainWindow();
		public PMWindowList PMWindowManager = null;
		private Framework Server;
		private Abstraction form;

		private MainWindow() : base(Gtk.WindowType.Toplevel)
		{
			//this.RunServer();
		}
		public global::Gtk.Notebook ChatTabs
		{
			get { return this.notebook2; }
			set { this.notebook2 = value; }
		}
		public void RunServer()
		{
			this.Build();
			this.ShowAll();

			//this.Server = Framework.GetInstance();
			this.PMWindowManager = PMWindowList.GetInstance();
			//this.Server.RunServer();
			this.form = Abstraction.GetInstance();
		}
		public void AppendTextWithoutScroll(Gtk.TextView t, string text)
		{
			Gtk.TextIter iter;
			//t.Buffer.MoveMark(t.Buffer.InsertMark, t.Buffer.EndIter);
			if (text != null)
			{
				if (text.Equals("") == false)
				{
					iter = t.Buffer.EndIter;
					t.Buffer.Insert(iter, text);
				}
			}
			iter = t.Buffer.EndIter;
			t.Buffer.Insert(iter, "\n");
		}

		public void AppendText(Gtk.TextView t, string text)
		{
			DateTime time = DateTime.Now; // Use current time
			string format = "HH:mm:ss";

			AppendTextWithoutScroll(t, "[" + time.ToString(format) + "] " + text);
			t.ScrollToIter(t.Buffer.EndIter, 0.4, true, 0.0, 1.0);
			//t.ScrollToMark(t.Buffer.InsertMark, 0.4, true, 0.0, 1.0);
		}

		public void WriteText(string text)
		{
			this.WriteText(this.StatusLog, text);
		}

		public void WriteText(Gtk.TextView t, string text)
		{
			Gtk.Application.Invoke(delegate
			{
				this.AppendText(t, text);
			}
			);
		}
		public void SetSubscriptionText(string text)
		{
			Gtk.Application.Invoke(delegate
			{
				this.label1.Text = text;
			}
			);
		}

		public void SetLockedByComputerText(string text)
		{
			Gtk.Application.Invoke(delegate
			{
				this.label2.Text = text;
			}
			);
		}

		public static MainWindow GetInstance()
		{
			return _instance;
		}

		public void RMPlayer(string account)
		{
			Gtk.Application.Invoke(delegate
			{
				Player player = (Player)this.Server.PlayerManager.FindPlayer(account);
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
			this.Server = Framework.GetInstance();

			nickname = (nickname != null) ? nickname : "unknown";
			if (account != null)
			{
				Gtk.Application.Invoke(delegate
				{
					Player player = (Player)this.Server.PlayerManager.FindPlayer(account);
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
		/*
		private void mnuClose_Click(object sender, EventArgs e)
		{
			if (this.tabControl1.SelectedTab.Name.ToLower() == "status")
			{
			}
			else if (this.tabControl1.SelectedTab.Name.StartsWith("#"))
			{
				this.Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,part," + this.tabControl1.SelectedTab.Name + "\n");
				this.tabControl1.TabPages.Remove(this.tabControl1.SelectedTab);
			}
			else
			{
				this.tabControl1.TabPages.Remove(this.tabControl1.SelectedTab);
			}
			//e.Cancel = true;
		}

		private void mnu_Opening(object sender, CancelEventArgs e)
		{
			Point p = this.tabControl1.PointToClient(Cursor.Position);
			for (int i = 0; i < this.tabControl1.TabCount; i++)
			{
				Rectangle r = this.tabControl1.GetTabRect(i);
				if (r.Contains(p))
				{
					this.tabControl1.SelectedIndex = i; // i is the index of tab under cursor
					return;
				}
			}
			e.Cancel = true;
		}

		void tabControl1_MouseDown(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Right)
			{
				// To show a context menu for only the tab button but not the content of the tab,
				// we must show it in the tab control's mouse up event.
				mnu.Show((Control)sender, e.Location);
			}
		}
		*/


		private void test(object sender, EventArgs e)
		{
			//GraalPlayer PMPlayer = (GraalPlayer)this.Server.PlayerManager.FindPlayer(this.treeView1.SelectedNode.Name);
			/*
			if (this.treeView1.SelectedNode.Parent != null)
			{
				if (this.treeView1.SelectedNode.Parent.Name == "IRC")
				{
					if (PMPlayer != null)
					{
						this.Write_Text("Trying to join IRC channel " + PMPlayer.Account.Split(':')[1] + "\r\n");
						this.Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,join," + PMPlayer.Account.Split(':')[1] + "\n");
					}
					else
					{
						this.Write_Text("Trying to join IRC channel " + this.treeView1.SelectedNode.Name.Split(':')[1] + "\r\n");
						this.Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,join," + this.treeView1.SelectedNode.Name.Split(':')[1] + "\n");
					}

					//this.OpenIRC(PMPlayer.account.Split(':')[1]);

				}
				else
				{
					if (PMPlayer != null)
						this.AddPMWindow((Int16)PMPlayer.Id);
				}
			}
			*/
		}
		/*
		public void TabControl_Click(object sender, MouseEventArgs e)
		{
			TabControl sender2 = (TabControl)sender;
			
			if (e.Button.ToString().Trim().ToLower() == "right")
			{
				//this.IRC_ChatWindowText("tabPage2", "(Me) " + sender2.SelectedIndex + " " + sender2.);
			}
			/*
			if (e.KeyCode.ToString().ToLower().Trim() == "return")
			{
				//this.IRC_ChatWindowText(sender2.Parent.Name.ToString(), "(Me) " + sender2.Text);
				this.Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,privmsg," + sender2.Parent.Name.ToString() + "," + CString.tokenize(sender2.Text) + "\n");
				sender2.Text = "";
			}
			*
		}
		*/
		public void IRC_ChatBar(object sender, Gtk.KeyPressEventArgs e)
		{
			Gtk.Entry sender2 = (Gtk.Entry)sender;
			//Gdk.EventKey k = e.Event;
			//Console.WriteLine("KEY WAS PRESSED!");
			//sender2.
			if (e.Event.Key.ToString().ToLower() == "return")
			{
				this.form.SendChat(sender2);
			}
		}
		/*
		public void tabPage_Disposed(object sender, ControlEventArgs e)
		{
			TabPage tabPage = (TabPage)sender;
			this.IRC_ChatWindowText("tabPage2", "(Me) " + "-Serverlist_Chat,irc,part," + tabPage.Name + "\n");
			this.Server.SendGSPacket(new CString() + (byte)154 + "-Serverlist_Chat,irc,part," + tabPage.Name + "\n");
		}
		*/
		protected void formclosed(object o, EventArgs args)
		{
			this.Server.CloseServers();

			ServerWindow serverWindow = ServerWindow.GetInstance();
			serverWindow.Show();
		}

		private void SendButtonPressed(object o, System.EventArgs args)
		{
			
			try
			{
				
					this.form.SendChat(this.entry2);
				
			}
			catch (NullReferenceException e)
			{
				this.WriteText("ERROR: " + e.Message.ToString());
			}

		}

		

		protected void Exit(object sender, EventArgs e)
		{
			Gtk.Application.Quit();
			this.Dispose();
		}
	}
}