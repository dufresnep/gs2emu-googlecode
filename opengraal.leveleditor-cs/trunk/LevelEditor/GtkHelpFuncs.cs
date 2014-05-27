using System;
using Gdk;
using Gtk;

namespace OpenGraal.LevelEditor
{
	public class GtkHelpFuncs
	{
		public static void drawWidgetRect(Widget w, Pixbuf widgetBuffer, int offsetX, int offsetY, int drawX, int drawY, int drawW, int drawH)
		{
			Gdk.GC gc = new Gdk.GC(w.GdkWindow);
			w.GdkWindow.DrawPixbuf (gc, widgetBuffer, 0, 0, offsetX, offsetY, -1, -1, Gdk.RgbDither.None, 0, 0);
			w.GdkWindow.DrawRectangle(gc, false, new Rectangle (drawX + offsetX, drawY + offsetY, drawW, drawH));

			//set colour to white
			gc.RgbFgColor = new Color (255, 255, 255);

			w.GdkWindow.DrawRectangle(gc, false, new Rectangle (drawX + offsetX + 1, drawY + offsetY + 1, drawW, drawH));
		}

		//for this method to work, the widget(w)'s preffered size must be set
		public static void parentOffset(out int offX, out int offY, Widget w)
		{
			try {
				int w1, h1, w2, h2;
				w.GetSizeRequest (out w1, out h1);
				w.Parent.GdkWindow.GetSize (out w2, out h2);

				//halve the results as the container is centered
				offX = (w2 - w1) /2;	offY = (h2 - h1) /2;
			}
			catch (NullReferenceException e) {
				Console.WriteLine ("Parent offset:" + e.Message);
				offX = offY = 0;
			}
		}
	}
}

