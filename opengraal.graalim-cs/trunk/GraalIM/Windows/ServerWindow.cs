using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using Gtk;
using System.Runtime.InteropServices;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;

namespace OpenGraal.GraalIM
{
	public partial class ServerWindow : Gtk.Window
	{
		private static ServerWindow _instance = new ServerWindow();
		protected MainWindow main = null;
		protected Framework framework = null;
		protected ListServerConnection LSConn;
		public LoginWindow loginWindow;
		public ErrorWindow errorWindow;
		public string nickname = "guest";
		public string username = "guest";
		public string password = "7Y3bdzjf";
		public string server = AppSettings.GetInstance().Addresses[0], gserver = "-";
		public int port = AppSettings.GetInstance().Port, gport = 14900;
		public string errortext = "error";

		delegate void ErrorCallback(string msg);

		public TServerList serverList;

		private ServerWindow() : base ("")
		{
			this.Build();
		}

		public void Build()
		{
			this.loginWindow = LoginWindow.GetInstance();
			this.InitializeComponent();
			this.serverList = new TServerList();
			//this.errorWindow = new ErrorWindow();
			this.ShowAll();

			//this.Error("hello");
			this.LSConn = new ListServerConnection(this.musicListStore);
			if (this.loginWindow.nickname_ent != null && this.loginWindow.nickname_ent.Text != "")
				this.nickname = this.loginWindow.nickname_ent.Text;

			if (this.loginWindow.username_ent != null && this.loginWindow.username_ent.Text != "")
				this.username = this.loginWindow.username_ent.Text;

			if (this.loginWindow.password_ent != null && this.loginWindow.password_ent.Text != "")
				this.password = this.loginWindow.password_ent.Text;

			if (this.loginWindow.server != null && this.loginWindow.server.Text != "")
				this.server = this.loginWindow.server.Text;

			if (this.loginWindow.port != null && this.loginWindow.port.Text != "")
				int.TryParse(this.loginWindow.port.Text, out this.port);

			this.refreshButton_Click(this, new EventArgs());
			//this.loginButton_Click(this, new EventArgs());
			
		}

		public static ServerWindow GetInstance()
		{
			return _instance;
		}

		/// <summary>
		/// Send Packet to GServer
		/// </summary>
		public void SendLSPacket(CString Packet)
		{
			LSConn.SendPacket(Packet);
		}

		private void loginButton_Click(object sender, EventArgs e)
		{
			this.framework = Framework.GetInstance();
			this.framework.RunServer();

			this.Hide();
		}

		private void OnSelectionChanged(object sender, EventArgs e)
		{
			Gtk.Application.Invoke(delegate
			{
				TreeSelection selection = (sender as TreeView).Selection;
				TreeModel model;
				TreeIter iter;
				 
				// THE ITER WILL POINT TO THE SELECTED ROW
				if (selection.GetSelected(out model, out iter))
				{
					try
					{
						Console.WriteLine("Selected Value:" + model.GetValue(iter, 1).ToString());
						foreach (TServer server in this.LSConn.serverList)
						{
							if (server.name == model.GetValue(iter, 1).ToString().Trim())
							{
								this.gserver = server.ip;
								this.gport = server.port;
								Console.WriteLine("Servername: " + server.name + " IP: " + server.ip + " Port: " + server.port);
							}
						}
					}
					catch (Exception ez)
					{
						Console.WriteLine("Error: " + ez.Message);
					}
				}
			}
			);
		}

		private void refreshButton_Click(object sender, EventArgs e)
		{
			//if (this.LSConn.Connected)
			//this.LSConn.Disconnect();

			this.LSConn.Connect(this.server, this.port);

			if (this.LSConn.Connected)
			{
				this.LSConn.Codec.Reset(Encrypt.Generation.GEN2);
				this.LSConn.SendLogin(this.username, this.password, this.nickname);
				this.LSConn.ReceiveData();
			}
		}

		private void ServerWindow_Closed(object sender, EventArgs e)
		{
			this.loginWindow.Show();
			this.Destroy();
		}

		private void ServerWindow_hide(object sender, EventArgs e)
		{
			//this.loginWindow.ShowAll();
			/*
			if (this.ErrorMsg != "error" && this.ErrorMsg != null)
			{
				this.errorWindow = new ErrorWindow();
				this.errorWindow.error_label.Text = this.ErrorMsg;
				this.errorWindow.ShowAll();
			}
			 */
		}

		public void Close(object sender, EventArgs e)
		{
			this.ServerWindow_Closed(sender, e);
		}

		public string ErrorMsg
		{
			get
			{
				return this.errortext;
			}
			set
			{
				//this.loginWindow.ShowAll();
				this.errortext = value;
				//this.errorWindow.ShowAll();
			}
		}

		internal void Write_Text(string p)
		{
			Gtk.Application.Invoke(delegate
			{
				this.logText.Text = p;
			}
			);
		}
	}
}
