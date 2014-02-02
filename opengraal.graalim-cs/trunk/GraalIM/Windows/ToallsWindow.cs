﻿
// This file has been generated by the GUI designer. Do not modify.
namespace OpenGraal.GraalIM
{
	public class ToallsWindow : Gtk.Window
	{
		private global::Gtk.Table table1;
		private global::Gtk.Button SendToallMessageButton;
		private global::Gtk.Entry entry1;
		private global::Gtk.ScrolledWindow GtkScrolledWindow;
		private global::Gtk.TextView textview1;
		private Framework Server;
		private static ToallsWindow _instance = new ToallsWindow();
		 
		public static ToallsWindow GetInstance()
		{
			return _instance;
		}
		public void WindowInit()
		{
			this.Build();
			this.Icon = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");
			this.Server = Framework.GetInstance();

			//this.Show();
		}

		private ToallsWindow()
			: base(Gtk.WindowType.Toplevel)
		{
			Gtk.Application.Invoke(delegate
			{
				
				this.WindowInit();
			}
			);
		}

		protected void Build()
		{
			// Widget OpenGraal.GraalIM.PMWindow
			this.CanFocus = true;
			this.Name = "OpenGraal.GraalIM.ToallsWindow";
			this.Title = global::Mono.Unix.Catalog.GetString("Toalls");
			this.Icon = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");
			this.WindowPosition = ((global::Gtk.WindowPosition)(1));
			// Container child OpenGraal.GraalIM.PMWindow.Gtk.Container+ContainerChild
			this.table1 = new global::Gtk.Table(((uint)(2)), ((uint)(2)), false);
			this.table1.Name = "table1";
			// Container child table1.Gtk.Table+TableChild
			this.SendToallMessageButton = new global::Gtk.Button();
			this.SendToallMessageButton.HeightRequest = 23;
			this.SendToallMessageButton.CanFocus = true;
			this.SendToallMessageButton.Name = "button1";
			this.SendToallMessageButton.UseUnderline = true;
			this.SendToallMessageButton.Label = global::Mono.Unix.Catalog.GetString("_Send");
			this.table1.Add(this.SendToallMessageButton);
			global::Gtk.Table.TableChild w1 = ((global::Gtk.Table.TableChild)(this.table1[this.SendToallMessageButton]));
			w1.TopAttach = ((uint)(1));
			w1.BottomAttach = ((uint)(2));
			w1.LeftAttach = ((uint)(1));
			w1.RightAttach = ((uint)(2));
			w1.XOptions = ((global::Gtk.AttachOptions)(4));
			w1.YOptions = ((global::Gtk.AttachOptions)(4));
			// Container child table1.Gtk.Table+TableChild
			this.entry1 = new global::Gtk.Entry();
			this.entry1.HeightRequest = 23;
			this.entry1.CanFocus = true;
			this.entry1.Name = "entry1";
			this.entry1.IsEditable = true;
			this.entry1.InvisibleChar = '•';
			this.table1.Add(this.entry1);
			global::Gtk.Table.TableChild w2 = ((global::Gtk.Table.TableChild)(this.table1[this.entry1]));
			w2.TopAttach = ((uint)(1));
			w2.BottomAttach = ((uint)(2));
			w2.YOptions = ((global::Gtk.AttachOptions)(4));
			// Container child table1.Gtk.Table+TableChild
			this.GtkScrolledWindow = new global::Gtk.ScrolledWindow();
			this.GtkScrolledWindow.Name = "GtkScrolledWindow";
			this.GtkScrolledWindow.ShadowType = ((global::Gtk.ShadowType)(1));
			// Container child GtkScrolledWindow.Gtk.Container+ContainerChild
			this.textview1 = new global::Gtk.TextView();
			this.textview1.Name = "textview1";
			this.textview1.Editable = false;
			this.textview1.AcceptsTab = false;
			this.textview1.WrapMode = ((global::Gtk.WrapMode)(3));
			this.GtkScrolledWindow.Add(this.textview1);
			this.table1.Add(this.GtkScrolledWindow);
			global::Gtk.Table.TableChild w4 = ((global::Gtk.Table.TableChild)(this.table1[this.GtkScrolledWindow]));
			w4.RightAttach = ((uint)(2));
			this.Add(this.table1);
			if ((this.Child != null))
			{
				this.Child.ShowAll();
			}
			this.DefaultWidth = 409;
			this.DefaultHeight = 308;

			this.DeleteEvent += new global::Gtk.DeleteEventHandler(this.RemoveToallWindow);
			this.HideOnDelete();
			
			this.SendToallMessageButton.Clicked += new global::System.EventHandler(this.SendToallMessageButtonClicked);
		}

		private void RemoveToallWindow(object sender, Gtk.DeleteEventArgs e)
		{
			e.RetVal = true; 
			Abstraction main = Abstraction.GetInstance();

			this.Hide();
		}

		public void AppendTextWithoutScroll(string text)
		{
			Gtk.TextIter iter;
			this.textview1.Buffer.MoveMark(this.textview1.Buffer.InsertMark, this.textview1.Buffer.EndIter);
			if (text != null)
			{
				if (text.Equals("") == false)
				{
					iter = this.textview1.Buffer.EndIter;
					this.textview1.Buffer.Insert(iter, text);
				}
			}
			iter = this.textview1.Buffer.EndIter;
			this.textview1.Buffer.Insert(iter, "\n");
		}

		public void AppendText(string text)
		{
			System.DateTime time = System.DateTime.Now;              // Use current time
			string format = "HH:mm:ss";

			AppendTextWithoutScroll("[" + time.ToString(format) + "] " + text);

			this.textview1.ScrollToMark(this.textview1.Buffer.InsertMark, 0.4, true, 0.0, 1.0);
		}

		public void WriteText(string text)
		{
			Gtk.Application.Invoke(delegate
			{
				this.AppendText(text);
			}
			);
		}

		/// <summary>
		/// Set Properties
		/// </summary>
		public void SetMessage(OpenGraal.Common.Players.GraalPlayer player, OpenGraal.Core.CString Packet)
		{
			Gtk.Application.Invoke(delegate
			{
				if (player != null)
					this.WriteText("(" + player.Nickname + "): " + Packet.ReadChars(Packet.ReadGUByte1()));
			}
			);
		}

		private void SendToallMessageButtonClicked(object sender, System.EventArgs e)
		{
			this.SendMessage();
		}

		public void SendMessage()
		{
			this.SendMessage(new Core.CString(this.entry1.Text));

			this.entry1.Text = "";
		}

		public void SendMessage(OpenGraal.Core.CString message)
		{
			this.Server = Framework.GetInstance();
			this.Server.SendGSPacket(new OpenGraal.Core.CString() + (byte)Common.Connections.Client.GraalServer.PacketOut.TOALL + (byte)message.Text.Length + message.Text);
			Gtk.Application.Invoke(delegate
			{
				Abstraction mw = Abstraction.GetInstance();
				this.WriteText("(" + mw.NCPlayer.Nickname + "(You)): " + message.Text);
				message = null;
			}
			);
		}
	}
}