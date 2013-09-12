using System;

namespace OpenGraal.MapGenerator
{
	public partial class TestWindow : Gtk.Window
	{
		public TestWindow() : 
				base(Gtk.WindowType.Toplevel)
		{
			this.Build();
		}
	}
}

