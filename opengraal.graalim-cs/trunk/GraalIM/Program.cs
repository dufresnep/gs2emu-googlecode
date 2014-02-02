using System;
using System.Collections.Generic;
using System.Linq;
using OpenGraal.Core;
using Gtk;
using Gdk;

namespace OpenGraal.GraalIM
{
	static class Program
	{
		// The tray Icon
		private static StatusIcon trayIcon;

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(String[] args)
		{
			//Application.EnableVisualStyles();
			//Application.SetCompatibleTextRenderingDefault(false);
			//Application.Run(new LoginWindow());
			Application.Init();

			LoginWindow loginWindow = LoginWindow.GetInstance();
			//RCWindow rc = new RCWindow();
			//rc.ShowAll();
			loginWindow.ShowAll();
			// Creation of the Icon
			trayIcon = new StatusIcon(global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico"));
			//trayIcon.Pixbuf = global::Gdk.Pixbuf.LoadFromResource("OpenGraal.GraalIM.Resources.rcicon.ico");
			trayIcon.Visible = true;

			// Show/Hide the window (even from the Panel/Taskbar) when the TrayIcon has been clicked.
			trayIcon.Activate += delegate
			{
				loginWindow.Visible = !loginWindow.Visible;
				//rc.Visible = !rc.Visible;
			};

			// Show a pop up menu when the icon has been right clicked.
			trayIcon.PopupMenu += OnTrayIconPopup;

			// A Tooltip for the Icon
			trayIcon.Tooltip = "GraalIM";
			
			Application.Run();
		}
		// Create the popup menu, on right click.
		static void OnTrayIconPopup(object o, EventArgs args)
		{
			Menu popupMenu = new Menu();
		
			ImageMenuItem menuItemQuit = new Gtk.ImageMenuItem("Quit");
			Gtk.Image appimg = new Gtk.Image(Stock.Quit, IconSize.Menu);
			menuItemQuit.Image = appimg;
			popupMenu.Add(menuItemQuit);

			// Quit the application when quit has been clicked.
			menuItemQuit.Activated += delegate
			{
				Application.Quit();
			};

			popupMenu.ShowAll();
			popupMenu.Popup();
			popupMenu.TakeFocus = true;
			popupMenu.Title = "Graal RC";
		}
	}
}
