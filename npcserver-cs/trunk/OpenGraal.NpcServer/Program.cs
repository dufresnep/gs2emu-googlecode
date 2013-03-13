﻿using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.NpcServer;
using OpenGraal.NpcServer.Players;

namespace OpenGraal.NpcServer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
			Framework framework = new Framework("ncoptions.txt");
			while (framework.running)
				System.Threading.Thread.Sleep(50);
        }
    }
}
