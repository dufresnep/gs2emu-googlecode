using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using Microsoft.CSharp;
using CS_NPCServer;

namespace CS_NPCServer
{
    public partial class Form1 : Form
    {
		/// <summary>
		/// Member Variables
		/// </summary>
		NPCServer Server;

		/// <summary>
		/// Form Constructor
		/// </summary>
		public Form1()
        {
			InitializeComponent();
			Server = new NPCServer("");
        }

		/// <summary>
		/// Form Closed
		/// </summary>
		private void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			Server = null;
			Application.Exit();
		}

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
