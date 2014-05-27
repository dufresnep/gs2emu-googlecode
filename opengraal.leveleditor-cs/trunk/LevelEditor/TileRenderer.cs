using System;
using Gtk;
using Gdk;

namespace OpenGraal.LevelEditor
{
	public class TileRenderer : Renderable
	{
		private Gdk.GC _graphicsContext;
		private String _tileCache;
		private Gtk.Image _renderTo;
		private Pixbuf _tileBuffer;
		private Pixmap _levelPMap;

		public TileRenderer(Gtk.Image draw, int pixelWidth, int pixelHeight)
		{
			_renderTo = draw;
			_tileCache = "null";
			_levelPMap = new Pixmap(_renderTo.GdkWindow, pixelWidth, pixelHeight);

			_graphicsContext = new Gdk.GC(_renderTo.GdkWindow);
		}

		public void render (String img, int drawX, int drawY, int srcX, int srcY, int srcW, int srcH)
		{
			if (img.CompareTo (_tileCache) != 0)
				getTileMap (img);
			_levelPMap.DrawPixbuf (_graphicsContext, _tileBuffer, srcX, srcY, drawX, drawY, srcW, srcH, Gdk.RgbDither.None, 0, 0);
		}

		public void flush()
		{
			_renderTo.SetFromPixmap (_levelPMap, null);
		}

		//basically converts our image into a pixmap
		//note: not ideal to repeatedly change the filename...(e.g. in a loop)
		public void getTileMap(String img)
		{
			_tileCache = img;
			_tileBuffer = new Pixbuf (img);
		}
	}
}

