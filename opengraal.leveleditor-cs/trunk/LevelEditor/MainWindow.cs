using System;
using Gtk;
using Gdk;
using OpenGraal;
namespace OpenGraal.LevelEditor
{
	public partial class MainWindow : Gtk.Window
	{
		private OpenGraal.LevelEditor.Level _editLevel;
		private OpenGraal.LevelEditor.Renderable _levelRenderer;
		private Pixbuf _tilesetCache, _levelCache;
		private int _dragX, _dragY,
		//remember the last level update x&y to save drawing time
		_levelUpdateX, _levelUpdateY;
		private OpenGraal.Common.Levels.GraalLevelTile[,] _fillTile, _tileCluster;

		private bool _levelTileSelection, _tilesetGrid;

		public MainWindow()
			: base(Gtk.WindowType.Toplevel)
		{
			Build();
			initNewLevel(null);
		}

		public void initNewLevel(String filename)
		{
			_levelTileSelection = false;
			if (_levelTileSelection) {
				//temp code to satisfy warning...
			}

			_tilesetGrid = false;//at least for now...

			_fillTile = new OpenGraal.Common.Levels.GraalLevelTile[1, 1];
			_fillTile [0, 0] = new OpenGraal.Common.Levels.GraalLevelTile ();
			_fillTile [0, 0].SetTile (0);

			if (filename == null || filename == "null")
				_editLevel = new OpenGraal.LevelEditor.Level ();
			else
				_editLevel = new OpenGraal.LevelEditor.Level (filename);

			drawTileset();


			int levelWidth, levelHeight; _editLevel.getPixelSize(out levelWidth, out levelHeight);
			_levelRenderer = new OpenGraal.LevelEditor.TileRenderer(levelDrawPane, levelWidth, levelHeight);

			_editLevel.renderLevel(_levelRenderer);

			levelDrawPane.SetSizeRequest (levelWidth, levelHeight);

			_levelCache = levelDrawPane.Pixbuf;
		}

		protected void OnDeleteEvent(object sender, DeleteEventArgs a)
		{
			Application.Quit();
			a.RetVal = true;

		}

		//level must be declared prior to calling this
		public void drawTileset()
		{
			_tilesetCache = new Gdk.Pixbuf(_editLevel.tilesetPath);

			Pixmap pMap, mask;
			_tilesetCache.RenderPixmapAndMask (out pMap, out mask, 0);

			int imgWidth = _tilesetCache.Width,
			imgHeight = _tilesetCache.Height;

			_tilesetEventBox.SetSizeRequest (imgWidth, imgHeight);
			tilesetDrawPane.SetSizeRequest (imgWidth, imgHeight);

			if (_tilesetGrid) {
				//get the context we're drawing in
				Gdk.GC gc = new Gdk.GC (levelDrawPane.GdkWindow);

				//draw the grid
				for (int x = 0; x < imgWidth; x += _editLevel._tileWidth)
					pMap.DrawLine (gc, x, 0, x, imgHeight);
				for (int y = 0; y < imgHeight; y += _editLevel._tileHeight)
					pMap.DrawLine (gc, 0, y, imgWidth, y);
			}

			tilesetDrawPane.SetFromPixmap (pMap, mask);

			//----------------------
		}

		protected void onTilesetClick(object o, ButtonPressEventArgs args)
		{
			if (args.Event.Button != 1)
				return;

			_levelTileSelection = false;

			int offX, offY, cx, cy;
			GtkHelpFuncs.parentOffset (out offX, out offY, tilesetDrawPane);

			Widget w = (Widget)o;
			w.GetPointer (out cx, out cy);

			//convert our pixel coordinate to a tile coordinate
			_editLevel.pixToTile (cx - offX, cy - offY, out cx, out cy);

			_dragX = cx;
			_dragY = cy;
		}

		protected void onTilesetDrag(object o, MotionNotifyEventArgs args)
		{
			int offX, offY, cx, cy;
			GtkHelpFuncs.parentOffset (out offX, out offY, tilesetDrawPane);

			Widget w = (Widget)o;
			w.GetPointer (out cx, out cy);
			_editLevel.pixToTile (cx - offX, cy - offY, out cx, out cy);

			int tX, tY, tW, tH;
			tX = Math.Min (_dragX, cx);
			tY = Math.Min (_dragY, cy);
			tW = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragX - cx + 0.0)));
			tH = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragY - cy + 0.0)));

			GtkHelpFuncs.drawWidgetRect (
				tilesetDrawPane, _tilesetCache, offX, offY, 
				tX * _editLevel._tileWidth, tY * _editLevel._tileHeight,
				tW * _editLevel._tileWidth, tH * _editLevel._tileHeight
			);
		}

		protected void onTilesetRelease(object o, ButtonReleaseEventArgs args)
		{
			int offX, offY, cx, cy;
			GtkHelpFuncs.parentOffset (out offX, out offY, tilesetDrawPane);

			Widget w = (Widget)o;
			w.GetPointer (out cx, out cy);
			_editLevel.pixToTile (cx - offX, cy - offY, out cx, out cy);

			int tX, tY, tW, tH;
			tX = Math.Min (_dragX, cx);
			tY = Math.Min (_dragY, cy);
			tW = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragX - cx + 0.0)));
			tH = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragY - cy + 0.0)));

			GtkHelpFuncs.drawWidgetRect (
				tilesetDrawPane, _tilesetCache, offX, offY, 
				tX * _editLevel._tileWidth, tY * _editLevel._tileHeight,
				tW * _editLevel._tileWidth, tH * _editLevel._tileHeight
			);

			_tileCluster = new OpenGraal.Common.Levels.GraalLevelTile[tW, tH];
			//determine the tile value based on tile coordinate found above
			for (int x = 0; x < tW; x++) {
				for (int y = 0; y < tH; y++){
					_tileCluster [x, y] = new OpenGraal.Common.Levels.GraalLevelTile ();
					_tileCluster[x,y].SetTile(_editLevel.getTilesetVal (tX + x, tY + y));
				}
			}
		}

		protected void onLevelClick(object o, ButtonPressEventArgs args)
		{
			int offX, offY, cx, cy;
			GtkHelpFuncs.parentOffset (out offX, out offY, levelDrawPane);

			Widget w = (Widget)o;
			w.GetPointer (out cx, out cy);

			_editLevel.pixToTile (cx - offX, cy - offY, out cx, out cy);

			if (args.Event.Button == 3) {//right click
				_editLevel.flood (cx, cy, (_tileCluster != null) ? _tileCluster : _fillTile);
				_editLevel.renderLevel (_levelRenderer);
			} else if (args.Event.Button == 1) {
				if (_tileCluster == null) {
					//tile selection

					_dragX = cx;
					_dragY = cy;

				} else {
					placeTiles (w);
				}
			}
		}

		protected void onLevelRelease(object o, ButtonReleaseEventArgs args)
		{
			if (args.Event.Button != 1)//we're not interested in anything other than left button release
				return;

			if (_tileCluster != null) {
				_levelUpdateX = _levelUpdateY = _dragX = _dragY = -1;
				_tileCluster = null;
				drawTileset ();
			} else if (_dragX + _dragY > 0) {
				int offX, offY, cx, cy;
				GtkHelpFuncs.parentOffset (out offX, out offY, levelDrawPane);

				Widget w = (Widget)o;
				w.GetPointer (out cx, out cy);

				_editLevel.pixToTile (cx - offX, cy - offY, out cx, out cy);

				int tX, tY, tW, tH;
				tX = Math.Min (_dragX, cx);
				tY = Math.Min (_dragY, cy);
				tW = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragX - cx + 0.0)));
				tH = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragY - cy + 0.0)));

				GtkHelpFuncs.drawWidgetRect (
					levelDrawPane, _levelCache, offX, offY, 
					tX * _editLevel._tileWidth, tY * _editLevel._tileHeight,
					tW * _editLevel._tileWidth, tH * _editLevel._tileHeight
				);

				_tileCluster = new OpenGraal.Common.Levels.GraalLevelTile[tW, tH];
				//determine the tile value based on tile coordinate found above
				for (int x = 0; x < tW; x++) {
					//if (x < 0 || x > _editLevel._mapWidth)
						//continue;

					for (int y = 0; y < tH; y++){
						//if (y < 0 || y >= _editLevel._mapHeight)
						//continue;

						_tileCluster[x,y] = new OpenGraal.Common.Levels.GraalLevelTile();
						_tileCluster [x, y].SetTile(_editLevel.getTile (0, tX + x, tY + y).GetIndex());
					}
				}
			}
		}

		protected void onLevelMove(object o,MotionNotifyEventArgs args)
		{
			if (_tileCluster == null) {
				int offX, offY, cx, cy;
				GtkHelpFuncs.parentOffset (out offX, out offY, levelDrawPane);

				Widget w = (Widget)o;
				w.GetPointer (out cx, out cy);

				_editLevel.pixToTile (cx - offX, cy - offY, out cx, out cy);

				int tX, tY, tW, tH;
				tX = Math.Min (_dragX, cx);
				tY = Math.Min (_dragY, cy);
				tW = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragX - cx + 0.0)));
				tH = Math.Max(1,Math.Abs ((int)Math.Ceiling(_dragY - cy + 0.0)));

				reportLbl.Text = "[" + tX + "," + tY + ":" + tW + "," + tH + "]";

				GtkHelpFuncs.drawWidgetRect (
					levelDrawPane, _levelCache, offX, offY, 
					tX * _editLevel._tileWidth, tY * _editLevel._tileHeight,
					tW * _editLevel._tileWidth, tH * _editLevel._tileHeight
				);
			} else {
				placeTiles ((Widget)o);
			}
		}

		private void placeTiles(Widget o)
		{
			if (_tileCluster == null)
				return;

			int offX, offY, cx, cy;
			GtkHelpFuncs.parentOffset (out offX, out offY, levelDrawPane);

			Widget w = (Widget)o;
			w.GetPointer (out cx, out cy);

			_editLevel.pixToTile (cx - offX, cy - offY, out cx, out cy);

			if (_levelUpdateX == cx && _levelUpdateY == cy)
				return;

			_levelUpdateX = cx;
			_levelUpdateY = cy;

			_editLevel.setTiles (0, cx, cy, _tileCluster);

			_editLevel.renderTiles (_levelRenderer, cx, cy, _tileCluster.GetLength (0), _tileCluster.GetLength (1));

			_levelCache = levelDrawPane.Pixbuf;//doesn't seem to be working... :( try pixmap?
		}
	}
}