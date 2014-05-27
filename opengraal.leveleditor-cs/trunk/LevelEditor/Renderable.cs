using System;

namespace OpenGraal.LevelEditor
{
	public interface Renderable
	{
		void render (String img, int drawX, int drawY, int srcX, int srcY, int srcW, int srcH);
		void flush();
	}
}

