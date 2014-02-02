using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using Gtk;

namespace OpenGraal.GraalIM
{
	public partial class ErrorWindow : Gtk.Window
	{ 
		public Gtk.Label error_label;
		private static ErrorWindow _instance = new ErrorWindow();

		public static ErrorWindow GetInstance()
		{
			return _instance;
		}

		private ErrorWindow (): base(Gtk.WindowType.Toplevel)
		{
			this.Build();
			this.DeleteEvent += ErrorWindow_Closed;
		}

		private void ErrorWindow_Closed(object sender, Gtk.DeleteEventArgs e)
		{
			e.RetVal = true;
			this.Hide();
		}

		private void Close(object sender, Gtk.DeleteEventArgs e)
		{
			this.ErrorWindow_Closed(sender, e);
		}
	}
}