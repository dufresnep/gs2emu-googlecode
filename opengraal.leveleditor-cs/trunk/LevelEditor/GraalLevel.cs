using System;

namespace OpenGraal.LevelEditor
{
	public class GraalLevel : OpenGraal.Common.Levels.GraalLevel, ILevel
	{
		public GraalLevel (String filename) : base (filename, null)
		{
			try {Load (filename);}
			catch (Exception){
				Console.WriteLine ("No level found.x");
			}
		}

		public void Save(String filename)
		{
			Save (new OpenGraal.Core.CString (filename));
		}

		public OpenGraal.Common.Levels.GraalLevelTile getTile(int layer, int x, int y)
		{
			OpenGraal.Common.Levels.GraalLevelTileList tileList;
			Layers.TryGetValue (layer, out tileList);

			if (tileList == null)
			{
				tileList = new OpenGraal.Common.Levels.GraalLevelTileList ();
				layers.Add (layer, tileList);
			}

			return tileList.FindTile (x, y);
		}
	}
}

