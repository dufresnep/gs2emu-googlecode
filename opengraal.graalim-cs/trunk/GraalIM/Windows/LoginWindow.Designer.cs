using Gtk;

namespace OpenGraal.GraalIM
{
	partial class LoginWindow
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void Build()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LoginWindow));

			this.panel5 = new Fixed();
			//this.groupBox1 = new Gtk.Box();
			this.checkBox2 = new Gtk.CheckButton();
			this.checkBox1 = new Gtk.CheckButton();
			this.nickname_label = new Gtk.Label();
			this.nickname_ent = new Gtk.Entry();
			this.username_label = new Gtk.Label();
			this.username_ent = new Gtk.Entry();
			this.password_label = new Gtk.Label();
			this.password_ent = new Gtk.Entry();
			this.button1 = new Gtk.Button();
			this.LoginButton = new Gtk.Button();
			this.port = new Gtk.Entry();
			this.port_label = new Gtk.Label();
			this.server = new Gtk.Entry();
			this.server_label = new Gtk.Label();

			// Standard strings
			string nickName = "Guest";
			string accountName = "guest";
			string password = "7Y3bdzjf";
			bool rcmode = true;

		

			if (this.settings != null)
			{
				accountName = this.settings.Account;
				password = this.settings.Password;
				if (!this.settings2.Loaded)
					nickName = this.settings.Nickname;
				else
					nickName = this.settings2.Nickname;
				rcmode = this.settings.RCMode;
			}
			else
			{
				//return null;
			}

			// 
			// checkBox2
			// 
			this.checkBox2.Active = true;
			this.checkBox2.Name = "checkBox2";
			this.checkBox2.Label = "RC Mode?";
			this.checkBox2.Xalign = 0;
			this.checkBox2.Active = rcmode;

			// 
			// checkBox1
			//
			this.checkBox1.Active = false;
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Label = "Don\'t save password";
			this.checkBox1.Xalign = 0;
			
			// 
			// nickname_label
			// 
			this.nickname_label.Name = "nickname_label";
			this.nickname_label.Xalign = 0;
			this.nickname_label.SetSizeRequest(73, 23);
			this.nickname_label.Text = "Nickname:";

			// 
			// nickname
			// 
			this.nickname_ent.Name = "nickname";
			this.nickname_ent.Text = nickName;
			
			// 
			// username_label
			// 
			this.username_label.Name = "username_label";
			this.username_label.Xalign = 0;
			this.username_label.SetSizeRequest(73, 23);
			this.username_label.Text = "Account:";
			 
			// 
			// username
			// 
			this.username_ent.Name = "username";
			this.username_ent.Text = accountName;
			//this.username.Changed += new System.EventHandler(this.username_TextChanged);
			
			// 
			// password_label
			// 
			this.password_label.Name = "password_label";
			this.password_label.Xalign = 0;
			this.password_label.SetSizeRequest(73, 23);
			this.password_label.Text = "Password:";

			// 
			// password
			// 
			this.password_ent.Name = "password";
			//this.password_ent.SetSizeRequest(82, 23);
			this.password_ent.Text = password;
			this.password_ent.Visibility = false;
			
			// 
			// button1
			// 
			this.button1.SetSizeRequest(80, 25);
			this.button1.Label = "Cancel";
			this.button1.Clicked += new System.EventHandler(this.Close);
			
			// 
			// LoginButton
			// 
			this.LoginButton.Name = "LoginButton";
			this.LoginButton.SetSizeRequest(80, 25);
			this.LoginButton.Label = "OK";
			this.LoginButton.Clicked += new System.EventHandler(this.LoginButton_Click);

			// 
			// LoginWindow
			// 
			Table table = new Table(2, 1, false);
			Table table2 = new Table(7, 2, false);
			table2.RowSpacing = 1;
			Label spacer = new Label();
			spacer.SetSizeRequest(10, 1);
			table2.Attach(spacer, 0, 1, 0, 1, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			table2.Attach(this.nickname_label, 0, 1, 1, 2, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			table2.Attach(this.nickname_ent, 1, 2, 1, 2, AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill, 5, 1);
			table2.Attach(this.username_label, 0, 1, 2, 3, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			table2.Attach(this.username_ent, 1, 2, 2, 3, AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill, 5, 1);
			table2.Attach(this.password_label, 0, 1, 3, 4, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			table2.Attach(this.password_ent, 1, 2, 3, 4, AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill, 5, 1);
			table2.Attach(this.checkBox1, 0, 2, 4, 5, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			table2.Attach(this.checkBox2, 0, 2, 5, 6, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			HBox hbox = new HBox(true, 5);
			table.BorderWidth = 0;
			table.SetSizeRequest(100, 100);
			Frame frame = new Frame();
			frame.Label = " Options ";

			frame.Add(table2);
			table.Attach(frame, 0, 1, 0, 1, AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill | AttachOptions.Expand, 5, 5);

			hbox.Add(this.LoginButton);
			hbox.Add(this.button1);

			Alignment halign = new Alignment(1, 0, 0, 0);
			halign.Add(hbox);
			table.Attach(halign, 0, 1, 1, 2, AttachOptions.Fill, AttachOptions.Fill, 5, 5);
			this.Add(table);

			this.SetSizeRequest(280, 220);
			this.DoubleBuffered = true;
			this.Name = "LoginWindow";
			this.SetPosition(Gtk.WindowPosition.Center);
			this.Title = "Graal Server Remote Control";
			this.Icon = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");
			this.DeleteEvent += LoginWindow_Closed;
		}

		private Gtk.Button LoginButton;
		private Gtk.Fixed panel5;
		private Gtk.Label nickname_label;
		public Gtk.Entry nickname_ent;
		public Gtk.Entry password_ent;
		private Gtk.Label password_label;
		public Gtk.Entry username_ent;
		private Gtk.Label username_label;
		public Gtk.Entry port;
		private Gtk.Label port_label;
		public Gtk.Entry server;
		private Gtk.Label server_label;
		private Gtk.Button button1;
		private Gtk.Box groupBox1;
		public Gtk.CheckButton checkBox2;
		private Gtk.CheckButton checkBox1;
	}
}