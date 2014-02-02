using System;
using OpenGraal;
using OpenGraal.Core;

namespace OpenGraal.GraalIM
{
	public partial class PMWindow : Gtk.Window
	{
		private short _id = 0;
		protected PMWindowList PMWindowManager;
		protected CString LastMessage = null;
		protected Player PMPlayer;
		private Framework Server;
		 
		public short Id
		{
			get { return this._id; }
			set { this._id = value; }
		}

		public void WindowInit()
		{
			
			this.Icon = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");
			this.Server = Framework.GetInstance();
			this.PMWindowManager = PMWindowList.GetInstance();

			this.PMPlayer = (Player)this.Server.PlayerManager.FindPlayer(this.Id);
			string TitleText = "";
			if (this.PMPlayer != null)
			{
				TitleText = this.PMPlayer.Account;
				if (this.PMPlayer.Nickname != "" && this.PMPlayer.Nickname != null)
					TitleText += ": " + this.PMPlayer.Nickname;
			}
			this.Title = TitleText;
			this.Show();
		}

		public PMWindow(short id) : base(Gtk.WindowType.Toplevel)
		{
			Gtk.Application.Invoke(delegate
			{
				this.Id = id;
				this.Build();
				this.WindowInit();
			}
			);
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
					Gtk.TextTag tag = new Gtk.TextTag("bold");
					tag.Weight = Pango.Weight.Bold;
					tag.Foreground = "#373349";
					this.textview1.Buffer.TagTable.Add(tag);
					this.textview1.Buffer.InsertWithTagsByName(ref iter, text.Substring(0,text.IndexOf(":",10) + 1),"bold");
					this.textview1.Buffer.Insert(ref iter, text.Substring(text.IndexOf(":", 10) + 1));
				}
			}
			iter = this.textview1.Buffer.EndIter;
			this.textview1.Buffer.Insert(ref iter, "\n");
		}

		public void AppendText(string text)
		{
			DateTime time = DateTime.Now;              // Use current time
			string format = "HH:mm:ss";

			AppendTextWithoutScroll("[" + time.ToString(format) + "] " + text);
			//while (Application.EventsPending ()) 
			//	Application.RunIteration ();
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
		public void SetMessage(CString Packet)
		{
			Gtk.Application.Invoke(delegate
			{
				this.LastMessage = Packet;

				if (this.PMPlayer != null)
					this.WriteText("(" + this.PMPlayer.Nickname + "): " + Packet.Untokenize().Text.Trim());

				this.Show();
			}
			);
		}

		public void SendMessage()
		{
			this.SendMessage(new Core.CString(this.entry1.Text));

			this.entry1.Text = "";
		}

		public void SendMessage(CString message)
		{
			this.Server = Framework.GetInstance();
			this.Server.SendPM(this._id, CString.tokenize(message.Text), true);
			Gtk.Application.Invoke(delegate
			{
				Abstraction mw = Abstraction.GetInstance();
				this.WriteText("(" + mw.NCPlayer.Nickname + "(You)): " + message.Text);
				message = null;
			}
			);
		}
		private void RemovePMWindow(object sender, Gtk.DeleteEventArgs e)
		{
			//e.RetVal = true; 
			Abstraction main = Abstraction.GetInstance();
			//main.WriteText("Removed PMWindow!");
			this.PMWindowManager = PMWindowList.GetInstance();
			this.PMWindowManager.DeletePMWindow(this.Id);
			this.Dispose();
		}

		private void button2_Click(object sender, EventArgs e)
		{
			this.SendMessage();
		}
	}
}

