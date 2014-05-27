using System;
using Gtk;

namespace OpenGraal.LevelEditor 
{
	class Program 
	{
		public Program()
		{
			Application.Init();
			MainWindow mainWin = new MainWindow ();
			Application.Run();
		}

		public static void Main()
		{
			new Program ();
		}
	}
}
