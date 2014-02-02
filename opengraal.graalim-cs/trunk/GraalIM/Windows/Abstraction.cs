using System;

namespace OpenGraal.GraalIM
{
	public class Abstraction
	{
		protected LoginWindow lw;
		protected RCWindow rc;
		protected RCPlayerList rcpl;
		protected MainWindow mw;
		protected Framework Framework;
		protected PMWindowList PMWindowManager;
		private bool _isRC = false;
		private static Abstraction _instance = new Abstraction();
		public GraalIM.Player NCPlayer;

		public static Abstraction GetInstance()
		{
			return _instance;
		}

		public bool isRC
		{
			get
			{
				return this._isRC;
			}

			set
			{
				this._isRC = value;
			}
		}

		private Abstraction()
		{
		}

		public void Activate()
		{
			this.lw = LoginWindow.GetInstance();
			this.isRC = this.lw.checkBox2.Active;
			Console.WriteLine("Is this rc? " + this.isRC.ToString() + "::" + this.lw.checkBox2.Active.ToString());
			if (this.isRC)
			{
				this.rc = RCWindow.GetInstance();
				this.rc.RunServer();
				this.rcpl = RCPlayerList.GetInstance();
				this.rcpl.Init();
			}
			else
			{
				this.mw = MainWindow.GetInstance();
				this.mw.RunServer();
			}
		}

		public void WriteText(string text)
		{
			if (this.isRC)
				this.rc.WriteText(text);
			else
				this.mw.WriteText(text);
		}

		public void WriteText(Gtk.TextView txtView, string text)
		{
			if (this.isRC)
				this.rc.WriteText(txtView,text);
			else
				this.mw.WriteText(txtView,text);
		}

		[GLib.ConnectBefore()] 
		public void IRC_ChatBar(object sender, Gtk.KeyPressEventArgs e)
		{
			Gtk.Entry sender2 = (Gtk.Entry)sender;

			if (e.Event.Key.ToString().ToLower() == "return")
			{
				this.SendChat(sender2);
			}
		}

		public void OpenIRC(string channel)
		{
			Gtk.Application.Invoke(delegate
			{
				Gtk.Table Chat = this.FindPanel("irctab_" + channel);
				if (Chat == null)
				{
					Chat = new global::Gtk.Table(((uint)(2)), ((uint)(1)), false);
					Chat.Name = "irctab_" + channel;

					// Container child table2.Gtk.Table+TableChild
					global::Gtk.Entry ChatBar = new global::Gtk.Entry();
					ChatBar.CanFocus = true;
					ChatBar.Name = "irctab_" + channel + "_entry";
					ChatBar.IsEditable = true;
					ChatBar.InvisibleChar = '•';
					ChatBar.KeyPressEvent += IRC_ChatBar;
					Chat.Add(ChatBar);
					global::Gtk.Table.TableChild w1 = ((global::Gtk.Table.TableChild)(Chat[ChatBar]));
					w1.TopAttach = ((uint)(1));
					w1.BottomAttach = ((uint)(2));
					w1.XOptions = ((global::Gtk.AttachOptions)(4));
					w1.YOptions = ((global::Gtk.AttachOptions)(4));
					// Container child table2.Gtk.Table+TableChild
					global::Gtk.ScrolledWindow GtkScrolledWindow = new global::Gtk.ScrolledWindow();
					GtkScrolledWindow.Name = "GtkScrolledWindow";
					GtkScrolledWindow.ShadowType = ((global::Gtk.ShadowType)(1));
					// Container child GtkScrolledWindow.Gtk.Container+ContainerChild
					global::Gtk.TextView textview1 = new global::Gtk.TextView();
					textview1.CanFocus = true;
					textview1.Name = "irctab_" + channel + "_textview";
					textview1.Editable = false;
					textview1.WrapMode = ((global::Gtk.WrapMode)(3));
					textview1.PixelsInsideWrap = 7;
					GtkScrolledWindow.Add(textview1);
					Chat.Add(GtkScrolledWindow);
					global::Gtk.Table.TableChild w3sc = ((global::Gtk.Table.TableChild)(Chat[GtkScrolledWindow]));
					w3sc.TopAttach = ((uint)(0));
					w3sc.BottomAttach = ((uint)(1));
					// Notebook tab
					Gtk.Label testLabel = new global::Gtk.Label();
					testLabel.CanDefault = true;
					testLabel.Name = "irctab_" + channel + "_label";
					testLabel.LabelProp = global::Mono.Unix.Catalog.GetString(channel);
					if (this.isRC)
					{

						this.rc.ChatTabs.AppendPage(Chat, testLabel);
						this.rc.ChatTabs.CurrentPage = this.rc.ChatTabs.PageNum(Chat);

					}
					else
					{
						this.mw.ChatTabs.AppendPage(Chat, testLabel);
						this.mw.ChatTabs.CurrentPage = this.mw.ChatTabs.PageNum(Chat);
					}

					testLabel.ShowAll();
					Chat.ShowAll();
					this.IRC_ChatWindowText(channel, "-!- You have joined " + channel);
				}
				else
				{
					if (this.isRC)
						this.rc.ChatTabs.CurrentPage = this.rc.ChatTabs.PageNum(Chat);
					else
						this.mw.ChatTabs.CurrentPage = this.mw.ChatTabs.PageNum(Chat);
				}
			}
			);
		}

		public void IRC_Privmsg(string account, string channel, string message)
		{
			Gtk.Application.Invoke(delegate
			{
				if (channel.StartsWith("#"))
					this.IRC_ChatWindowText(channel, "(" + account + ") " + message);
			}
			);
		}

		public void IRC_AddChannelUser(string channel, string account, bool del = false)
		{
			Gtk.Application.Invoke(delegate
			{
				if (del)
					this.IRC_ChatWindowText(channel, "-!- " + account + " has left " + channel);
				else
					this.IRC_ChatWindowText(channel, "-!- " + account + " has joined " + channel);
			}
			);
		}

		public Gtk.Table FindPanel(string name)
		{
			Gtk.Notebook notebook2 = new Gtk.Notebook();
			if (this.isRC)
				notebook2 = this.rc.ChatTabs;
			else
				notebook2 = this.mw.ChatTabs;
			foreach (object l in notebook2.AllChildren)
			{
				//Console.WriteLine("test: " + l.GetType());
				if (l.GetType() == typeof(Gtk.Table))
				{
					Gtk.Table test = (Gtk.Table)l;
					//this.WriteText("VPanedName: " + test.Name);
					if (test.Name == name)
						return test;
				}
			}
			return null;
		}

		public Gtk.Table RemovePanel(string name)
		{
			Gtk.Notebook notebook2 = new Gtk.Notebook();
			if (this.isRC)
				notebook2 = this.rc.ChatTabs;
			else
				notebook2 = this.mw.ChatTabs;
			
			foreach (object l in notebook2.AllChildren)
			{
				//Console.WriteLine("test: " + l.GetType());
				if (l.GetType() == typeof(Gtk.Table))
				{
					Gtk.Table test = (Gtk.Table)l;
					//this.WriteText("VPanedName: " + test.Name);
					if (test.Name == name)
					{
						notebook2.Remove(test);
						test.Dispose();
					}
				}
			}
			return null;
		}

		public void IRC_ChatWindowText(string channel, string message)
		{
			Gtk.Application.Invoke(delegate
			{
				Gtk.Table test = this.FindPanel("irctab_" + channel);
				if (test == null)
					this.OpenIRC(channel);

				test = this.FindPanel("irctab_" + channel);

				foreach (object d in test)
				{
					if (d.GetType() == typeof(Gtk.ScrolledWindow))
					{
						Gtk.ScrolledWindow sw = (Gtk.ScrolledWindow)d;
						foreach (object t in sw)
						{
							if (t.GetType() == typeof(Gtk.TextView))
							{
								this.WriteText((Gtk.TextView)t, message);
							}
						}
					}
				}
			}
			);
		}
	
		public void SetSubscriptionText(string str)
		{
			if (this.isRC)
			{
			}
			else
				this.mw.SetSubscriptionText(str);
		}

		public void SetLockedByComputerText(string str)
		{
			if (this.isRC)
			{
			}
			else
				this.mw.SetLockedByComputerText(str);
		}

		public void AddPlayer(string account, string account2)
		{
			if (this.isRC)
			{
				this.rcpl = RCPlayerList.GetInstance();
				this.rcpl.AddPlayer(account, account2);
			}
			else
				this.mw.AddPlayer(account, account2);
		}

		public void RMPlayer(string account)
		{
			if (this.isRC)
			{
				this.rcpl = RCPlayerList.GetInstance();
				this.rcpl.RMPlayer(account);
			}
			else
				this.mw.RMPlayer(account);
		}

		public void SendChat(Gtk.Entry sender2)
		{
			this.Framework = Framework.GetInstance();
			if (sender2.Name.StartsWith("irctab_"))
			{

				if (sender2.Text.StartsWith("/"))
				{
					string[] command = sender2.Text.Split(' ');
					if (command[0].ToLower() == "/part" && command.Length != 1)
					{
						this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)154 + "-Serverlist_Chat,irc,part," + command[1].Trim() + "\n");
						this.RemovePanel("irctab_" + command[1].Trim());
					}
					else if (command[0].ToLower() == "/part" && command.Length == 1)
					{
						this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)154 + "-Serverlist_Chat,irc,part," + sender2.Name.Replace("irctab_", "").Replace("_entry", "").ToString() + "\n");
						this.RemovePanel(sender2.Name.Replace("_entry", ""));
					}
					else if (command[0].ToLower() == "/join" && command.Length != 1)
						this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)154 + "-Serverlist_Chat,irc,join," + command[1].Trim() + "\n");
				}
				else
					this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)154 + "-Serverlist_Chat,irc,privmsg," + sender2.Name.Replace("irctab_", "").Replace("_entry", "").ToString() + "," + OpenGraal.Core.CString.tokenize(sender2.Text) + "\n");
			}
			else
			{
				if (sender2.Text.StartsWith("/"))
				{
					string[] command = sender2.Text.Split(' ');
					if (command[0].ToLower() == "/join" && command.Length != 1)
						this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)154 + "-Serverlist_Chat,irc,join," + command[1].Trim() + "\n");
					else if (command[0].ToLower() == "/msg" && command.Length != 1)
					{
						String msg = "";
						for (int i = 2; i < command.Length; i++)
							msg += command[i].Trim() + " ";

						// Turn the string into a CString.
						Core.CString Message = new Core.CString(msg);

						this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)154 + "-Serverlist_Chat,irc,privmsg," + command[1].Trim() + "," + Message.Tokenize() + "\n");
					}
					else if (command[0].ToLower() == "/pm" && command.Length != 1)
					{
						this.SendPM(command);
					}
					else if (command[0].ToLower() == "/mapgen" && command.Length == 1)
					{
						/*
						MapGenWindow MapGen;
						Thread thread = new Thread(() =>
						{
							MapGen = new MapGenWindow();
							MapGen.Show();
							//MapGen.Closed += (s, e) => MapGen.Dispatcher.InvokeShutdown(); 
							//Dispatcher.Run();
						}
						);
						thread.SetApartmentState(ApartmentState.STA);
						//thread.IsBackground = true;
						thread.Start();
						//MapGenWindow MapGen = new MapGenWindow();
						
						TestWindow test = new TestWindow();
						test.ShowAll();
						*/

					}
					else
					{
						if (this.isRC)
							this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.RC_CHAT + sender2.Text + "\n");
						else
						{
							this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.PLAYERPROPS + (byte)Common.Connections.Client.GraalServer.PLPROPS.CURCHAT + (byte)sender2.Text.Length + sender2.Text + "\n");
							this.WriteText("(" + this.NCPlayer.Account + "(" + this.NCPlayer.Id + ")) " + sender2.Text);
						}
					}
				}
				else
				{
					if (this.isRC)
						this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.RC_CHAT + sender2.Text + "\n");
					else
					{
						this.NCPlayer.Chat = sender2.Text;
						this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.PLAYERPROPS + (byte)Common.Connections.Client.GraalServer.PLPROPS.CURCHAT + (byte)sender2.Text.Length + sender2.Text + "\n");
						this.WriteText("(" + this.NCPlayer.Account + "(" + this.NCPlayer.Id + ")) " + sender2.Text);
					}
				}
			}
			sender2.Text = "";
		}

		private void SendPM(string[] command)
		{
			Player PMPlayer = (Player)Framework.PlayerManager.FindPlayer(command[1].Trim());

			PMWindowManager = PMWindowList.GetInstance();
			PMWindow PM = PMWindowManager.AddPMWindow(PMPlayer.Id);

			String msg = "";
			for (int i = 2; i < command.Length; i++)
				msg += command[i].Trim() + " ";

			// Turn the string into a CString.
			Core.CString Message = new Core.CString(msg);

			if (PM != null)
			{
				PM.SendMessage(Message);
			}
		}
	}
}