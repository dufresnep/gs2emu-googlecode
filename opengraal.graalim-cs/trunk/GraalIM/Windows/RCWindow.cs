using System;

namespace OpenGraal.GraalIM
{
	public class RCWindow : Gtk.Window
	{

		#region Private Variables

		private static RCWindow _instance = new RCWindow();
		private Framework Framework;
		private Abstraction form = Abstraction.GetInstance();
		private Control2Config rcSettings = Control2Config.GetInstance();
		private Gtk.Image bg = null, playerlistIcon = null, accountsIcon = null, serverOptionsIcon = null, toallsIcon = null, optionsIcon = null, folderOptionsIcon = null, serverFlagsIcon = null, fileBrowserIcon = null;
		private Gtk.EventBox playerlistButton, accountsButton, serverOptionsButton, toallsButton = null, optionsButton = null, folderOptionsButton = null, serverFlagsButton = null, fileBrowserButton = null;
		private global::Gtk.Table table2, table3, ButtonTable;
		private global::Gtk.Entry RCChatBar;
		private global::Gtk.ScrolledWindow GtkScrolledWindow;
		private global::Gtk.TextView textview1;
		private global::Gtk.Fixed ServerNameAndSuch;
		private global::Gtk.Notebook _chatTabs;
		protected static RCPlayerList rcpl = RCPlayerList.GetInstance();

		#endregion

		#region Public Variables

		public PMWindowList PMWindowManager = null;

		public global::Gtk.Notebook ChatTabs
		{
			get { return this._chatTabs; }
			set { this._chatTabs = value; }
		}

		#endregion

		#region Construct/Destruct functions

		public static RCWindow GetInstance()
		{
			return _instance;
		}

		private RCWindow() : base(Gtk.WindowType.Toplevel)
		{
		}

		#endregion

		#region Public functions

		public void RunServer()
		{
			this.Build();

			this.bg = new global::Gtk.Image();
			this.bg.Name = "bg";

			this.bg.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_opengraal.jpg");

			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.Background))
					this.bg.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.Background);
			}
							
			this.bg.SetAlignment(0, 0);
			this.bg.HeightRequest = 160;
			
			this.SizeRequested += OnResize;
			this.table3.Add(this.bg);

			global::Gtk.Table.TableChild w4 = ((global::Gtk.Table.TableChild)(this.table3[this.bg]));
			w4.BottomAttach = ((uint)(0));


			//this.ResizeChecked += OnResize;
			//this.SizeRequested += OnResize;
			this.CreateButtons();

			this.Framework = Framework.GetInstance();
			this.PMWindowManager = PMWindowList.GetInstance();
			this.DeleteEvent += delegate
			{
				this.Framework.CloseServers();

				ServerWindow serverWindow = ServerWindow.GetInstance();
				serverWindow.Show();
			};
			this.DestroyEvent += delegate
			{
				this.Framework.CloseServers();

				ServerWindow serverWindow = ServerWindow.GetInstance();
				serverWindow.Show();
			};
			this.ShowAll();
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
					Gtk.TextTag tag = new Gtk.TextTag("bold");
					tag.Weight = Pango.Weight.Bold;
					tag.Foreground = this.rcSettings.ColorChatBold;
					tag.SizePoints = this.rcSettings.ChatFontSize;
					t.Buffer.TagTable.Add(tag);
					tag = new Gtk.TextTag("font");
					tag.SizePoints = this.rcSettings.ChatFontSize;
					tag.Foreground = this.rcSettings.ColorChat;
					t.Buffer.TagTable.Add(tag);

					string time = text.Substring(0, text.IndexOf("]") + 1);
					text = text.Substring(text.IndexOf("]") + 1);
					t.Buffer.InsertWithTagsByName(ref iter, time, "bold");
					t.Buffer.InsertWithTagsByName(ref iter, text.Substring(0, text.IndexOf(":") + 1), "bold");
					t.Buffer.InsertWithTagsByName(ref iter, text.Substring(text.IndexOf(":") + 1),"font");
				
				}
			}
			iter = t.Buffer.EndIter;
			t.Buffer.Insert(iter, "\n");
		}

		public void AppendText(Gtk.TextView t, string text)
		{
			DateTime time = DateTime.Now; // Use current time
			string format = "HH:mm:ss";

			this.AppendTextWithoutScroll(t, "[" + time.ToString(format) + "] " + text);
			t.ScrollToIter(t.Buffer.EndIter, 0.4, true, 0.0, 1.0);
			//t.ScrollToMark(t.Buffer.InsertMark, 0.4, true, 0.0, 1.0);
		}

		public void WriteText(string text)
		{
			this.WriteText(this.textview1, text);
			Console.WriteLine(text);
		}

		public void WriteText(Gtk.TextView t, string text)
		{
			Gtk.Application.Invoke(delegate
			{
				this.AppendText(t, text);
			}
			);
		}

		#endregion

		#region Private functions

		private void CreateButtons()
		{
			#region Playerlist Button
			this.playerlistIcon = new global::Gtk.Image();
			this.playerlistIcon.Name = "playerlist";
			this.playerlistIcon.Xalign = 0F;
			this.playerlistIcon.Yalign = 0F;
			this.playerlistIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_playerlist_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconPlayerList))
					this.playerlistIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconPlayerList);
			}
			this.playerlistButton = new Gtk.EventBox();
			this.playerlistButton.Add(this.playerlistIcon);
			this.playerlistButton.ButtonPressEvent += ButtonClicked;
			this.playerlistButton.ButtonReleaseEvent += ButtonReleased;
			this.playerlistButton.BorderWidth = 0;
			this.playerlistButton.VisibleWindow = false;
			this.ButtonTable.Add(this.playerlistButton);
			global::Gtk.Table.TableChild w11 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.playerlistButton]));
			w11.XOptions = ((global::Gtk.AttachOptions)(0));
			w11.YOptions = ((global::Gtk.AttachOptions)(0));
			#endregion

			#region Accounts Button
			this.accountsIcon = new global::Gtk.Image();
			this.accountsIcon.CanDefault = true;
			this.accountsIcon.Events = ((global::Gdk.EventMask)(4));
			this.accountsIcon.Name = "accounts";
			this.accountsIcon.Xalign = 0F;
			this.accountsIcon.Yalign = 0F;
			this.accountsIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_accounts_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconAccounts))
					this.accountsIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconAccounts);
			}
			this.accountsButton = new Gtk.EventBox();
			this.accountsButton.Add(this.accountsIcon);
			this.accountsButton.ButtonPressEvent += ButtonClicked;
			this.accountsButton.ButtonReleaseEvent += ButtonReleased;
			this.accountsButton.BorderWidth = 0;
			this.accountsButton.VisibleWindow = false;
			this.ButtonTable.Add(this.accountsButton);
			global::Gtk.Table.TableChild w6 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.accountsButton]));
			w6.LeftAttach = ((uint)(1));
			w6.RightAttach = ((uint)(2));
			w6.XOptions = ((global::Gtk.AttachOptions)(4));
			w6.YOptions = ((global::Gtk.AttachOptions)(4));
			#endregion

			#region Serveroptions Button
			// Container child table4.Gtk.Table+TableChild
			this.serverOptionsIcon = new global::Gtk.Image();
			this.serverOptionsIcon.CanDefault = true;
			this.serverOptionsIcon.Name = "serveroptions";
			this.serverOptionsIcon.Xalign = 0F;
			this.serverOptionsIcon.Yalign = 0F;
			this.serverOptionsIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_serveroptions_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconServerOptions))
					this.serverOptionsIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconServerOptions);
			}
			this.serverOptionsButton = new Gtk.EventBox();
			this.serverOptionsButton.Add(this.serverOptionsIcon);
			this.serverOptionsButton.ButtonPressEvent += ButtonClicked;
			this.serverOptionsButton.ButtonReleaseEvent += ButtonReleased;
			this.serverOptionsButton.BorderWidth = 0;
			this.serverOptionsButton.VisibleWindow = false;
			this.ButtonTable.Add(this.serverOptionsButton);
			global::Gtk.Table.TableChild w5 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.serverOptionsButton]));
			w5.TopAttach = ((uint)(1));
			w5.BottomAttach = ((uint)(2));
			w5.LeftAttach = ((uint)(6));
			w5.RightAttach = ((uint)(7));
			w5.XOptions = ((global::Gtk.AttachOptions)(4));
			w5.YOptions = ((global::Gtk.AttachOptions)(4));
			#endregion

			#region Toalls Button
			this.toallsIcon = new global::Gtk.Image();
			this.toallsIcon.CanDefault = true;
			this.toallsIcon.Name = "toalls";
			this.toallsIcon.Xalign = 0F;
			this.toallsIcon.Yalign = 0F;
			this.toallsIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_toalls_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconToalls))
					this.toallsIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconToalls);
			}
			this.toallsButton = new Gtk.EventBox();
			this.toallsButton.Add(this.toallsIcon);
			this.toallsButton.ButtonPressEvent += ButtonClicked;
			this.toallsButton.ButtonReleaseEvent += ButtonReleased;
			this.toallsButton.BorderWidth = 0;
			this.toallsButton.VisibleWindow = false;
			this.ButtonTable.Add(this.toallsButton);
			global::Gtk.Table.TableChild w9 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.toallsButton]));
			w9.LeftAttach = ((uint)(2));
			w9.RightAttach = ((uint)(3));
			w9.XOptions = ((global::Gtk.AttachOptions)(4));
			w9.YOptions = ((global::Gtk.AttachOptions)(4));
			#endregion

			#region Serverflags Button
			this.serverFlagsIcon = new global::Gtk.Image();
			this.serverFlagsIcon.CanDefault = true;
			this.serverFlagsIcon.Name = "serverflags";
			this.serverFlagsIcon.Xalign = 0F;
			this.serverFlagsIcon.Yalign = 0F;
			this.serverFlagsIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_serverflags_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconServerFlags))
					this.serverFlagsIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconServerFlags);
			}
			this.serverFlagsButton = new Gtk.EventBox();
			this.serverFlagsButton.Add(this.serverFlagsIcon);
			this.serverFlagsButton.ButtonPressEvent += ButtonClicked;
			this.serverFlagsButton.ButtonReleaseEvent += ButtonReleased;
			this.serverFlagsButton.BorderWidth = 0;
			this.serverFlagsButton.VisibleWindow = false;
			this.ButtonTable.Add(this.serverFlagsButton);
			global::Gtk.Table.TableChild w12 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.serverFlagsButton]));
			w12.LeftAttach = ((uint)(5));
			w12.RightAttach = ((uint)(6));
			w12.XOptions = ((global::Gtk.AttachOptions)(4));
			w12.YOptions = ((global::Gtk.AttachOptions)(4));
			#endregion

			#region Options Button
			this.optionsIcon = new global::Gtk.Image();
			this.optionsIcon.CanDefault = true;
			this.optionsIcon.Name = "options";
			this.optionsIcon.Xalign = 0F;
			this.optionsIcon.Yalign = 0F;
			this.optionsIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_options_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconOptions))
					this.optionsIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconOptions);
			}
			this.optionsButton = new Gtk.EventBox();
			this.optionsButton.Add(this.optionsIcon);
			this.optionsButton.ButtonPressEvent += ButtonClicked;
			this.optionsButton.ButtonReleaseEvent += ButtonReleased;
			this.optionsButton.BorderWidth = 0;
			this.optionsButton.VisibleWindow = false;
			this.ButtonTable.Add(this.optionsButton);
			global::Gtk.Table.TableChild w7 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.optionsButton]));
			w7.LeftAttach = ((uint)(4));
			w7.RightAttach = ((uint)(5));
			w7.XOptions = ((global::Gtk.AttachOptions)(4));
			w7.YOptions = ((global::Gtk.AttachOptions)(4));
			#endregion

			#region Folderoptions Button
			this.folderOptionsIcon = new global::Gtk.Image();
			this.folderOptionsIcon.CanDefault = true;
			this.folderOptionsIcon.Name = "folderoptions";
			this.folderOptionsIcon.Xalign = 1F;
			this.folderOptionsIcon.Yalign = 1F;
			this.folderOptionsIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_folderoptions_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconFolderOptions))
					this.folderOptionsIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconFolderOptions);
			}
			this.folderOptionsButton = new Gtk.EventBox();
			this.folderOptionsButton.Add(this.folderOptionsIcon);
			this.folderOptionsButton.ButtonPressEvent += ButtonClicked;
			this.folderOptionsButton.ButtonReleaseEvent += ButtonReleased;
			this.folderOptionsButton.BorderWidth = 0;
			this.folderOptionsButton.VisibleWindow = false;
			this.ButtonTable.Add(this.folderOptionsButton);
			global::Gtk.Table.TableChild w8 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.folderOptionsButton]));
			w8.LeftAttach = ((uint)(6));
			w8.RightAttach = ((uint)(7));
			w8.XOptions = ((global::Gtk.AttachOptions)(4));
			w8.YOptions = ((global::Gtk.AttachOptions)(4));
			#endregion

			#region Filebrowser Button
			this.fileBrowserIcon = new global::Gtk.Image();
			this.fileBrowserIcon.CanDefault = true;
			this.fileBrowserIcon.Name = "filebrowser";
			this.fileBrowserIcon.Xalign = 0F;
			this.fileBrowserIcon.Yalign = 0F;
			this.fileBrowserIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_filebrowser_normal.png");
			if (this.rcSettings.Loaded)
			{
				if (System.IO.File.Exists("skins/" + this.rcSettings.IconFileBrowser))
					this.fileBrowserIcon.Pixbuf = new Gdk.Pixbuf("skins/" + this.rcSettings.IconFileBrowser);
			}
			this.fileBrowserButton = new Gtk.EventBox();
			this.fileBrowserButton.Add(this.fileBrowserIcon);
			this.fileBrowserButton.ButtonPressEvent += ButtonClicked;
			this.fileBrowserButton.ButtonReleaseEvent += ButtonReleased;
			this.fileBrowserButton.BorderWidth = 0;
			this.fileBrowserButton.VisibleWindow = false;
			this.ButtonTable.Add(this.fileBrowserButton);
			global::Gtk.Table.TableChild w13 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.fileBrowserButton]));
			w13.TopAttach = ((uint)(1));
			w13.BottomAttach = ((uint)(2));
			w13.XOptions = ((global::Gtk.AttachOptions)(0));
			w13.YOptions = ((global::Gtk.AttachOptions)(0));
			#endregion
		}

		#region Button Actions

		private void ButtonClicked(object o, EventArgs args)
		{
			Gtk.EventBox button = (Gtk.EventBox)o;
			Gtk.Image img = (Gtk.Image)button.Child;

			img.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_" + img.Name + "_pressed.png");
			if (this.rcSettings.Loaded)
			{
				string iconString = "";

				if (img.Name == "playerlist")
					iconString = this.rcSettings.IconPlayerListPressed;
				else if (img.Name == "toalls")
					iconString = this.rcSettings.IconToallsPressed;
				else if (img.Name == "serveroptions")
					iconString = this.rcSettings.IconServerOptionsPressed;
				else if (img.Name == "accounts")
					iconString = this.rcSettings.IconAccountsPressed;
				else if (img.Name == "classlist")
					iconString = this.rcSettings.IconClassListPressed;
				else if (img.Name == "filebrowser")
					iconString = this.rcSettings.IconFileBrowserPressed;
				else if (img.Name == "folderoptions")
					iconString = this.rcSettings.IconFolderOptionsPressed;
				else if (img.Name == "levellist")
					iconString = this.rcSettings.IconLevelListPressed;
				else if (img.Name == "localnpcs")
					iconString = this.rcSettings.IconLocalNpcsPressed;
				else if (img.Name == "npclist")
					iconString = this.rcSettings.IconNpclistPressed;
				else if (img.Name == "options")
					iconString = this.rcSettings.IconOptionsPressed;
				else if (img.Name == "serverflags")
					iconString = this.rcSettings.IconServerFlagsPressed;
				else if (img.Name == "weaponlist")
					iconString = this.rcSettings.IconWeaponListPressed;

				if (System.IO.File.Exists("skins/" + iconString))
					img.Pixbuf = new Gdk.Pixbuf("skins/" + iconString);
			}

			if (img.Name == "playerlist")
				Gtk.Application.Invoke(delegate
				{
					rcpl.Init();
					rcpl.ShowAll();
				});
			else if (img.Name == "toalls")
				Gtk.Application.Invoke(delegate
				{
					ToallsWindow toalls = ToallsWindow.GetInstance();
					
					//toalls.WindowInit();
					toalls.ShowAll();
				});
			else if (img.Name == "serveroptions")
				Gtk.Application.Invoke(delegate
				{
					this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.RC_SERVEROPTIONSGET);
				});
			else if (img.Name == "serverflags")
				Gtk.Application.Invoke(delegate
				{
					this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.RC_SERVERFLAGSGET);
				});
			else if (img.Name == "folderoptions")
				Gtk.Application.Invoke(delegate
				{
					this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.RC_FOLDERCONFIGGET);
				});
			else if (img.Name == "filebrowser")
				Gtk.Application.Invoke(delegate
				{
					this.Framework.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.RC_FILEBROWSER_START);
				});
			else if (img.Name == "options")
			{
				

			}
		}

		private void ButtonReleased(object o, EventArgs args)
		{
			Gtk.EventBox button = (Gtk.EventBox)o;
			Gtk.Image img = (Gtk.Image)button.Child;

			img.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rc_images.rc_" + img.Name + "_normal.png");
			if (this.rcSettings.Loaded)
			{
				string iconString = "";

				if (img.Name == "playerlist")
					iconString = this.rcSettings.IconPlayerList;
				else if (img.Name == "toalls")
					iconString = this.rcSettings.IconToalls;
				else if (img.Name == "serveroptions")
					iconString = this.rcSettings.IconServerOptions;
				else if (img.Name == "accounts")
					iconString = this.rcSettings.IconAccounts;
				else if (img.Name == "classlist")
					iconString = this.rcSettings.IconClassList;
				else if (img.Name == "filebrowser")
					iconString = this.rcSettings.IconFileBrowser;
				else if (img.Name == "folderoptions")
					iconString = this.rcSettings.IconFolderOptions;
				else if (img.Name == "levellist")
					iconString = this.rcSettings.IconLevelList;
				else if (img.Name == "localnpcs")
					iconString = this.rcSettings.IconLocalNpcs;
				else if (img.Name == "npclist")
					iconString = this.rcSettings.IconNpclist;
				else if (img.Name == "options")
					iconString = this.rcSettings.IconOptions;
				else if (img.Name == "serverflags")
					iconString = this.rcSettings.IconServerFlags;
				else if (img.Name == "weaponlist")
					iconString = this.rcSettings.IconWeaponList;

				if (System.IO.File.Exists("skins/" + iconString))
					img.Pixbuf = new Gdk.Pixbuf("skins/" + iconString);
			}
		}

		#endregion

        [GLib.ConnectBefore()] 
		public void IRC_ChatBar(object s, Gtk.KeyPressEventArgs e)
		{
			Gtk.Entry sender = (Gtk.Entry)s;
			if (e.Event.Key.ToString().ToLower() == "return")
			{
				this.form.SendChat(sender);
			}
			else if (e.Event.Key == Gdk.Key.Up)
			{
			}
		}
		#endregion

		#region Protected functions

		protected void Build()
		{
			//global::Stetic.Gui.Initialize(this);
			this._chatTabs = new global::Gtk.Notebook();
			this._chatTabs.CanFocus = true;
			this._chatTabs.Name = "_chatTabs";
			this._chatTabs.CurrentPage = 0;
			this._chatTabs.EnablePopup = true;
			this._chatTabs.Scrollable = true;

			// Widget OpenGraal.GraalIM.RCWindow
			this.Name = "OpenGraal.GraalIM.RCWindow";
			this.Title = global::Mono.Unix.Catalog.GetString("OpenGraal Remote Control 2013/12/02");
			this.Icon = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");
			this.WindowPosition = ((global::Gtk.WindowPosition)(4));

			// Container child OpenGraal.GraalIM.RCWindow.Gtk.Container+ContainerChild
			this.table2 = new global::Gtk.Table(((uint)(2)), ((uint)(1)), false);
			this.table2.Name = "table2";

			global::Gtk.Table RCChat = new global::Gtk.Table(((uint)(2)), ((uint)(1)), false);
			RCChat.Name = "RCChat";


			// Container child table2.Gtk.Table+TableChild
			this.RCChatBar = new global::Gtk.Entry();
			this.RCChatBar.CanFocus = true;
			this.RCChatBar.Name = "entry1";
			this.RCChatBar.IsEditable = true;
			this.RCChatBar.InvisibleChar = '•';
			this.RCChatBar.KeyPressEvent += IRC_ChatBar;
			Gdk.Color chatBarBgColor = new Gdk.Color();
			Gdk.Color.Parse(this.rcSettings.ColorEditBack, ref chatBarBgColor);
			Gdk.Color chatBarTextColor = new Gdk.Color();
			Gdk.Color.Parse(this.rcSettings.ColorChatBack, ref chatBarTextColor);
			this.RCChatBar.ModifyText(Gtk.StateType.Normal, chatBarTextColor);
			this.RCChatBar.ModifyBase(Gtk.StateType.Normal, chatBarBgColor);
			RCChat.Add(this.RCChatBar);
			global::Gtk.Table.TableChild w1 = ((global::Gtk.Table.TableChild)(RCChat[this.RCChatBar]));
			w1.TopAttach = ((uint)(1));
			w1.BottomAttach = ((uint)(2));
			w1.XOptions = ((global::Gtk.AttachOptions)(4));
			w1.YOptions = ((global::Gtk.AttachOptions)(4));
			// Container child table2.Gtk.Table+TableChild
			this.GtkScrolledWindow = new global::Gtk.ScrolledWindow();
			this.GtkScrolledWindow.Name = "GtkScrolledWindow";
			this.GtkScrolledWindow.ShadowType = ((global::Gtk.ShadowType)(1));
			// Container child GtkScrolledWindow.Gtk.Container+ContainerChild
			this.textview1 = new global::Gtk.TextView();
			this.textview1.CanFocus = true;
			this.textview1.Name = "textview1";
			this.textview1.Editable = false;
			this.textview1.WrapMode = ((global::Gtk.WrapMode)(3));
			this.textview1.PixelsInsideWrap = 7;
			this.textview1.AppPaintable = true;
			Gdk.Color bgColor = new Gdk.Color();
			Gdk.Color.Parse(this.rcSettings.ColorChatBack, ref bgColor);
			this.textview1.ModifyBase(Gtk.StateType.Normal, bgColor);
						
			this.GtkScrolledWindow.Add(this.textview1);
			RCChat.Add(this.GtkScrolledWindow);
			global::Gtk.Table.TableChild w3sc = ((global::Gtk.Table.TableChild)(RCChat[this.GtkScrolledWindow]));
			w3sc.TopAttach = ((uint)(0));
			w3sc.BottomAttach = ((uint)(1));
			global::Gtk.Label RCChatLabel = new global::Gtk.Label();
			RCChatLabel.Text = "RC Chat";

			this._chatTabs.Add(RCChat);
			this._chatTabs.SetTabLabel(RCChat, RCChatLabel);
			RCChatLabel.ShowAll();
			//this._chatTabs.

			this.table2.Add(this._chatTabs);
			global::Gtk.Table.TableChild w3 = ((global::Gtk.Table.TableChild)(this.table2[this._chatTabs]));
			w3.TopAttach = ((uint)(1));
			w3.BottomAttach = ((uint)(2));
			//w1.XOptions = ((global::Gtk.AttachOptions)(4));
			//w3.YOptions = global::Gtk.AttachOptions.Fill;
			//this._chatTabs.
			

			// Container child table2.Gtk.Table+TableChild
			this.table3 = new global::Gtk.Table(((uint)(2)), ((uint)(1)), false);
			this.table3.WidthRequest = 476;
			this.table3.HeightRequest = 136;
			this.table3.Name = "table3";
			// Container child table3.Gtk.Table+TableChild
			this.ButtonTable = new global::Gtk.Table(((uint)(3)), ((uint)(7)), false);
			this.ButtonTable.Name = "table4";
			this.ButtonTable.RowSpacing = ((uint)(1));
			this.ButtonTable.ColumnSpacing = ((uint)(1));
			this.ButtonTable.BorderWidth = ((uint)(5));
			// Container child table4.Gtk.Table+TableChild
			this.ServerNameAndSuch = new global::Gtk.Fixed();
			this.ServerNameAndSuch.Name = "fixed2";
			this.ServerNameAndSuch.HasWindow = false;
			this.ButtonTable.Add(this.ServerNameAndSuch);
			global::Gtk.Table.TableChild w4 = ((global::Gtk.Table.TableChild)(this.ButtonTable[this.ServerNameAndSuch]));
			w4.BottomAttach = ((uint)(3));
			w4.LeftAttach = ((uint)(3));
			w4.RightAttach = ((uint)(4));
			w4.YOptions = ((global::Gtk.AttachOptions)(0));
			this.table3.Add(this.ButtonTable);
			global::Gtk.Table.TableChild w5 = ((global::Gtk.Table.TableChild)(this.table3[this.ButtonTable]));
			w5.YPadding = ((uint)(11));
			w5.YOptions = global::Gtk.AttachOptions.Fill;//((global::Gtk.AttachOptions)(4));
			this.table2.Add(this.table3);
			global::Gtk.Table.TableChild w6 = ((global::Gtk.Table.TableChild)(this.table2[this.table3]));
			w6.YOptions = ((global::Gtk.AttachOptions)(4));
			this.Add(this.table2);
			if ((this.Child != null))
			{
				this.Child.ShowAll();
			}
			this.DefaultWidth = 476;
			this.DefaultHeight = 310;
			this.Show();
			//this.ResizeChecked += new global::System.EventHandler(this.OnResize);
		}

		protected void OnResize(object sender, EventArgs e)
		{
			//this.OnSizeRequested()
			Gtk.Window sender2 = (Gtk.Window)sender;
			//sender2.resi
			//this.bg.SetSizeRequest();
			this.bg.Pixbuf.ScaleSimple(sender2.WidthRequest, 160, Gdk.InterpType.Nearest);
			//Console.WriteLine("RESIZED");
		}

		protected void OnHide(object sender, EventArgs e)
		{
			this.Visible = !this.Visible;
		}

		#endregion

	}
}

