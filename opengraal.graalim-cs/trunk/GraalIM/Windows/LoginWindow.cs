using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using Gtk;
using System.Runtime.InteropServices;

namespace OpenGraal.GraalIM
{
	public partial class LoginWindow : Gtk.Window
	{
		private static LoginWindow _instance = new LoginWindow();
		protected ServerWindow serverWindow = null;
		private AppSettings settings = AppSettings.GetInstance();
		private Control2Config settings2 = Control2Config.GetInstance();
		public ErrorWindow errorWindow;

		private LoginWindow() : base(Gtk.WindowType.Toplevel)
		{
			this.settings.Load("settings.ini");
			if (System.IO.File.Exists("control2config.txt"))
			{
				this.settings2.Load("control2config.txt");
				this.settings2.Loaded = true;
			}
			this.Build();
			//this.errorWindow = new ErrorWindow();
			//this.errorWindow.ShowAll();
		}

		public static LoginWindow GetInstance()
		{
			return _instance;
		}

		private void LoginButton_Click(object sender, EventArgs e)
		{
			if (this.settings != null)
			{
				this.settings.Account = this.username_ent.Text;
				this.settings.Password = this.password_ent.Text;
				if (!this.settings2.Loaded)
					this.settings.Nickname = this.nickname_ent.Text;
				else
					this.settings2.Nickname = this.nickname_ent.Text;
				this.settings.RCMode = this.checkBox2.Active;

				this.settings.Addresses = this.settings.Addresses;
				this.settings.Port = this.settings.Port;
			}

			this.serverWindow = ServerWindow.GetInstance();
			this.serverWindow.Build();
			this.serverWindow.ShowAll();

			this.Hide();
		}

		private void LoginWindow_Closed(object sender, EventArgs e)
		{
			Application.Quit();
		}

		private void Close(object sender, EventArgs e)
		{
			this.LoginWindow_Closed(sender, e);
		}

		internal void Error(string p)
		{
			//this.errorWindow.SetErrorMsg(p);
			//this.errorWindow.ShowAll();
			//errorWindow.Destroyed += this.Close;
		}
	}
}
