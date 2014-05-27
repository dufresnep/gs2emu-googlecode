using System;

namespace OpenGraal.LevelEditor
{
	public interface ILevel
	{
		bool Load(String filename);
		void Save(String filename);
		OpenGraal.Common.Levels.GraalLevelTile getTile(int layer, int x, int y);
	}
}

