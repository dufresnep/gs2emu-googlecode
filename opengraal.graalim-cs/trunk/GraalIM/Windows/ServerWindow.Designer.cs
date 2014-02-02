using Gtk;

namespace OpenGraal.GraalIM
{
	[Gtk.TreeNode(ListOnly = true)]
	public class MyTreeNode : Gtk.TreeNode
	{
		string song_title;

		public MyTreeNode(string artist, string song_title)
		{
			Artist = artist;
			this.song_title = song_title;
		}

		[Gtk.TreeNodeValue(Column = 1)]
		public string
			Artist;

		[Gtk.TreeNodeValue(Column = 2)]
		public string SongTitle { get { return song_title; } }
	}

	partial class ServerWindow
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		//private System.ComponentModel.IContainer components = null;
		/*
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose();
		}
		*/
		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LoginWindow));

			this.checkBox2 = new Gtk.CheckButton();
			this.checkBox1 = new Gtk.CheckButton();

			this.cancelButton = new Gtk.Button();
			this.loginButton = new Gtk.Button();


			// 
			// checkBox2
			// 
			this.checkBox2.Active = true;
			this.checkBox2.Name = "checkBox2";
			this.checkBox2.Label = "Graphical Menu";
			this.checkBox2.Xalign = 0;

			// 
			// checkBox1
			//
			this.checkBox1.Active = false;
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Label = "Don\'t save password";
			this.checkBox1.Xalign = 0;
			/*
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
			this.nickname.Name = "nickname";
			this.nickname.Text = "Guest";
			*/
			
			// 
			// button1
			// 
			this.cancelButton.SetSizeRequest(80, 25);
			this.cancelButton.Label = "Cancel";
			this.cancelButton.Clicked += new System.EventHandler(this.Close);

			// 
			// refreshButton
			// 
			this.refreshButton = new Gtk.Button();
			this.refreshButton.Name = "refreshButton";
			this.refreshButton.SetSizeRequest(80, 25);
			this.refreshButton.Label = "Refresh";
			this.refreshButton.Clicked += new System.EventHandler(this.refreshButton_Click);

			// 
			// LoginButton
			// 
			this.loginButton.Name = "LoginButton";
			this.loginButton.SetSizeRequest(80, 25);
			this.loginButton.Label = "Connect";
			this.loginButton.Clicked += new System.EventHandler(this.loginButton_Click);

			/*
			// 
			// port
			// 
			this.port.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(155)))), ((int)(((byte)(123)))), ((int)(((byte)(76)))));
			this.port.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.port.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(184)))), ((int)(((byte)(142)))));
			this.port.Location = new System.Drawing.Point(215, 101);
			this.port.Margin = new System.Windows.Forms.Padding(0);
			this.port.MaximumSize = new System.Drawing.Size(50, 25);
			this.port.Name = "port";
			this.port.Size = new System.Drawing.Size(50, 21);
			this.port.TabIndex = 9;
			this.port.Text = "14922";
			this.port.TextChanged += new System.EventHandler(this.port_TextChanged);
			// 
			// port_label
			// 
			this.port_label.AutoSize = true;
			this.port_label.BackColor = System.Drawing.Color.Transparent;
			this.port_label.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(76)))), ((int)(((byte)(52)))), ((int)(((byte)(16)))));
			this.port_label.Location = new System.Drawing.Point(183, 101);
			this.port_label.Name = "port_label";
			this.port_label.Size = new System.Drawing.Size(29, 15);
			this.port_label.TabIndex = 8;
			this.port_label.Text = "Port";
			// 
			// server
			// 
			this.server.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(155)))), ((int)(((byte)(123)))), ((int)(((byte)(76)))));
			this.server.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.server.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(184)))), ((int)(((byte)(142)))));
			this.server.Location = new System.Drawing.Point(27, 119);
			this.server.Name = "server";
			this.server.Size = new System.Drawing.Size(170, 21);
			this.server.TabIndex = 7;
			this.server.Text = "listserver.graalonline.com";
			// 
			// server_label
			// 
			this.server_label.AutoSize = true;
			this.server_label.BackColor = System.Drawing.Color.Transparent;
			this.server_label.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(76)))), ((int)(((byte)(52)))), ((int)(((byte)(16)))));
			this.server_label.Location = new System.Drawing.Point(36, 94);
			this.server_label.Name = "server_label";
			this.server_label.Size = new System.Drawing.Size(42, 15);
			this.server_label.TabIndex = 6;
			this.server_label.Text = "Server";
			this.server_label.Click += new System.EventHandler(this.server_label_Click);
			 * 
			 */

			// 
			// ServerWindow
			// 
			Table table = new Table(2, 1, false);
			Table table2 = new Table(1, 2, false);
			table2.RowSpacing = 1;
			Label spacer = new Label();
			spacer.SetSizeRequest(10, 1);
			table2.Attach(spacer, 0, 1, 0, 1, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			
			//table2.Attach(this.nickname_label, 0, 1, 1, 2, AttachOptions.Fill, AttachOptions.Fill, 5, 1);
			this.logText = new Gtk.Entry();
			this.logText.SetSizeRequest(230, 230);
			this.tree = new Gtk.NodeView();
		
			this.tree.SetSizeRequest(230, 230);
			
			this.musicListStore = new Gtk.ListStore(typeof(Gdk.Pixbuf), typeof(string), typeof(int));
			//this.tree.Selection.Changed += new System.EventHandler(OnSelectionChanged);
			
			this.tree.CursorChanged += new System.EventHandler(OnSelectionChanged);
			this.tree.ButtonPressEvent += new ButtonPressEventHandler(delegate(object o, ButtonPressEventArgs args)
			{
				System.Console.WriteLine(args.ToString());
			});
			Gtk.CellRendererPixbuf pixbufrender = new Gtk.CellRendererPixbuf();
			TreeViewColumn TypeCol = this.tree.AppendColumn("", new Gtk.CellRendererPixbuf(), "pixbuf", 0);
			TypeCol.SortIndicator = true;
			TypeCol.SortColumnId = 0;
			Gtk.CellRendererPixbuf cellpb = new Gtk.CellRendererPixbuf();
			Gtk.CellRendererText cell = new Gtk.CellRendererText();
			TypeCol.PackStart(cellpb, false);
			TypeCol.PackStart(cell, false);
			
			TreeViewColumn NameCol = this.tree.AppendColumn("Name", new Gtk.CellRendererText(), "text", 1);
			NameCol.SortIndicator = true;
			NameCol.SortColumnId = 1;
			TreeViewColumn PCCol = this.tree.AppendColumn("Players", new Gtk.CellRendererText(), "text", 2);
			PCCol.SortIndicator = true;
			PCCol.SortColumnId = 2;
			
			this.tree.FixedHeightMode = false;
			this.tree.HeadersClickable = true;
			this.tree.SearchColumn = 1;
			this.tree.EnableSearch = true;
			this.tree.EnableTreeLines = true;
			this.tree.ShowExpanders = true;
			//this.tree.Vadjustment.
			musicListStore.AppendValues("", "Loading...", "");

			Gtk.Frame frame2 = new Gtk.Frame();
			this.tree.Model = musicListStore;
			
			this.GtkScrolledWindow = new global::Gtk.ScrolledWindow();
			this.GtkScrolledWindow.Name = "GtkScrolledWindow";
			this.GtkScrolledWindow.ShadowType = ((global::Gtk.ShadowType)(1));

			// Container child GtkScrolledWindow.Gtk.Container+ContainerChild

			this.GtkScrolledWindow.Add(this.tree);
			frame2.Add(this.GtkScrolledWindow);
			table2.Attach(this.logText, 1, 2, 0, 1, AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill | AttachOptions.Expand, 5, 1);
			table2.Attach(frame2, 0, 1, 0, 1, AttachOptions.Fill, AttachOptions.Fill, 5, 1);


			HBox hbox = new HBox(true, 5);
			table.BorderWidth = 0;
			table.SetSizeRequest(100, 100);
			Frame frame = new Frame();
			frame.Label = " Servers ";
			//frame.Add(new Label());
			frame.Add(table2);
			table.Attach(frame, 0, 1, 0, 1, AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Fill | AttachOptions.Expand, 5, 5);

			hbox.Add(this.cancelButton);
			hbox.Add(this.refreshButton);
			hbox.Add(this.loginButton);

			Alignment halign = new Alignment(1, 0, 0, 0);
			halign.Add(hbox);
			table.Attach(halign, 0, 1, 1, 2, AttachOptions.Fill, AttachOptions.Fill, 5, 5);
			this.Add(table);

			this.SetSizeRequest(520, 350);

			this.DoubleBuffered = true;
			
			this.Icon = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");

			this.Name = "ServerWindow";
			this.SetPosition(Gtk.WindowPosition.Center);
			this.Title = "Graal Servers";
			this.DeleteEvent += ServerWindow_Closed;
			this.Hidden += ServerWindow_hide;

		}

		#endregion

		private Gtk.ScrolledWindow GtkScrolledWindow;
		private Gtk.Button loginButton;
		private Gtk.Button refreshButton;
		private Gtk.Entry logText;
		private Gtk.Button cancelButton;
		private Gtk.CheckButton checkBox2;
		private Gtk.CheckButton checkBox1;
		public Gtk.NodeView tree;
		public Gtk.ListStore musicListStore;
	}
}