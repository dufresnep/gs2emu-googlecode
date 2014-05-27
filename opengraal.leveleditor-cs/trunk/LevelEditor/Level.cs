using System;
using Gtk;
using OpenGraal.Core;
using System.Collections.Generic;
using OpenGraal.Common.Levels;

namespace OpenGraal.LevelEditor
{
	public class Level
	{
		public int _mapWidth, _mapHeight, _levelWidth, _levelHeight, _tileWidth, _tileHeight, _tilesetHeight;
		public String tilesetPath;
		private Gtk.Image _tilesetImg;//change this to a native object?(future proofing), only need height
		
		private ILevel[,] _levels;


		/*
		 * Note:
		 * tileID = tilePosY + tilePosX * tilesetHeight
		 * thus...
		 * tilePosY = tileID % tilesetHeight
		 * tilePosXddddd = floor(tileId / tilesetHeight)
		 * 
		 * tilesetHeight = floor(tilesetPixelHeight / tileHeight)
		 */


		public Level()
		{
			init("Content/pics1.png", 64, 64, 16, 16);
		}

		//maps of varying sizes yet to be supported via opengraal.common.levels.graalLevel
		/*public Level(String tileset, int mapW, int mapH, int tileW, int tileH)
		{
			init (tileset, mapW, mapH, tileW, tileH);
		}*/

		public Level(String fileName)
		{
			if (fileName.EndsWith (".nw")) {//graal NW files
				_levels = new GraalLevel[1, 1];
				_levelWidth = _levelHeight = _mapWidth = _mapHeight = 64;
				_levels [0, 0] = new GraalLevel(fileName);

			} else {
				throw new Exception ("Unsupported level format");
			}
		}

		public void init (String tileset, int mapW, int mapH, int tileW, int tileH)
		{

			_levelWidth = _mapWidth = mapW;	_levelHeight = _mapHeight = mapH;
			_tileWidth = tileW;	_tileHeight = tileH;

			tilesetPath = tileset;	_tilesetImg = new Image (tileset);

			//used to satisfy parameter data
			int temp;
			Gdk.Pixbuf.GetFileInfo (tilesetPath, out temp, out _tilesetHeight);

			_levels = new GraalLevel[1, 1];
			_levels [0, 0] = new GraalLevel ("new.nw");
		}

		public OpenGraal.Common.Levels.GraalLevelTile getTile(int layer, int tx, int ty)
		{
			//Console.WriteLine (tx + "," + ty + ">>" + _levelWidth + "," + _levelHeight + "," + _mapWidth + "," + _mapHeight);
			if (tx < 0 || tx >= _mapWidth || ty < 0 || ty >= _mapHeight)
				return null;

			ILevel level = _levels [(int)(tx / _levelWidth), (int)(ty / _levelHeight)];
			return level.getTile(layer, tx, ty);
		}

		public void setTiles(int layer, int placeX, int placeY, GraalLevelTile[,] tileGroup)
		{
			for (int x = placeX; x < Math.Min (_levelWidth, placeX + tileGroup.GetLength (0)); x++) {
				if (x < 0)
					continue;

				for (int y = placeY; y < Math.Min (_levelHeight, placeY + tileGroup.GetLength (1)); y++) {
					if (y < 0)
						continue;

					OpenGraal.Common.Levels.GraalLevelTile tile = getTile (0, x, y);
					tile.SetTile (tileGroup [x - placeX, y - placeY].GetIndex());
				}
			}
		}

		//Convenience method
		public void renderLevel(Renderable renderTo)
		{
			renderTiles (renderTo, 0, 0, _levelWidth, _levelHeight);
		}

		public void renderTiles(Renderable renderTo, int pX, int pY, int pW, int pH)
		{
			for (int x = pX; x < pX + pW; x++) {
				if (x < 0)
					continue;
				for (int y = pY; y < pY + pH; y++) {
					if (y < 0)
						continue;

					renderTile (renderTo, x, y);
				}
			}
			renderTo.flush ();
		}

		//dont' forget to call flush if using this function alone
		public void renderTile(Renderable renderTo, int x, int y)
		{
			GraalLevelTile tileFind = getTile (0, x, y);
			int tileID = 0;
			if (tileFind != null)
				tileID = tileFind.GetIndex ();

			int	tilesetHeight = (int)(_tilesetHeight / _tileHeight),
				srcY = (tileID % tilesetHeight) * _tileWidth,
				srcX = (int) (tileID / tilesetHeight) * _tileHeight;

			renderTo.render (tilesetPath, x * _tileWidth, y * _tileHeight, srcX, srcY, _tileWidth, _tileHeight);
		}

		public void getPixelSize(out int width, out int height)
		{
			width = _levels.GetLength(0) * _levelWidth * _tileWidth;
			height = _levels.GetLength(1) * _levelHeight * _tileHeight;
		}

		public void pixToTile(int px, int py, out int tx, out int ty)
		{
			tx = (int)((px + 0.0) / _tileWidth);
			ty = (int)((py + 0.0) / _tileHeight);
		}

		public int getTilesetVal(int tx, int ty)
		{
			return ty + tx * (_tilesetHeight / _tileHeight);
		}

		public void flood(int sx, int sy, OpenGraal.Common.Levels.GraalLevelTile[,] fTileCluster)
		{
			Queue<coordTile> tileQueue = new Queue<coordTile> ();//tiles waiting to have their neighbours checked
			List<coordTile> usedList = new List<coordTile> ();//tiles that have been found

			coordTile tileAdd = new coordTile (sx, sy);
			tileAdd.SetTile (getTile (0, sx, sy).GetIndex ());

			tileQueue.Enqueue (tileAdd);
			usedList.Add (tileAdd);

			while (tileQueue.Count > 0)
			{
				coordTile curTile = tileQueue.Dequeue ();

				//find adjacent tiles of the same type
				for (int a = 0; a < 4; a++) {
					int offX = (int)Math.Cos (a * Math.PI / 2),
						offY = (int)Math.Sin (a * Math.PI / 2);

					coordTile findTile = new coordTile (curTile.posX + offX, curTile.posY + offY);

					GraalLevelTile levelFind = getTile (0, curTile.posX + offX, curTile.posY + offY);
					if (levelFind == null)
						continue;

					findTile.SetTile(levelFind.GetIndex());

					if (findTile.GetIndex () == curTile.GetIndex () && !usedList.Contains (findTile)) {
						tileQueue.Enqueue (findTile);
						usedList.Add (findTile);
					}
				}

				//finally replace the current tile with the flood tile
				GraalLevelTile floodTile = fTileCluster [
					curTile.posX % fTileCluster.GetLength (0), 
					curTile.posY % fTileCluster.GetLength (1)
				];
				getTile (0, curTile.posX, curTile.posY).SetTile (floodTile.GetIndex ());
			}
		}

		//for use with the fill function to get around the lack of position storage in the graalTile obj.
		private class coordTile : GraalLevelTile,IEquatable<coordTile>
		{
			public int posX, posY;
			public coordTile(int x, int y):base()
			{
				posX = x;
				posY = y;
			}

			public bool Equals(coordTile o)
			{
				return (o.posX == posX && o.posY == posY);
			}
		}
	}
}

