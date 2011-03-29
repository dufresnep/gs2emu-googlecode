using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace CS_NPCServer
{
    class Program
    {
        static void Main(string[] args)
        {
			NPCServer Server = new NPCServer("ncoptions.txt");
			if (!Server.Connect())
			{
				System.Console.WriteLine("Failed to connect to server!");
				System.Console.ReadLine();
				Application.Exit();
			}

			while (Server.RunServer())
				System.Threading.Thread.Sleep(50);
        }
    }
}
