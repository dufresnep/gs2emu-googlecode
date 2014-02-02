using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;
using OpenGraal.Common.Players;

namespace OpenGraal.GraalIM
{
	public partial class PMWindow : Form
	{
		/// <summary>
		/// Member Variables
		/// </summary>
		internal Framework Server;

		/// <summary>
		/// Set Variables
		/// </summary>
		protected short Id = 0;
		protected MainWindow2 form;
		protected CString LastMessage = null;
		protected GraalPlayer PMPlayer;
		delegate void SetTextCallback(CString text);
		delegate void PMRMCallback(object sender, FormClosedEventArgs e);

		internal PMWindow(Int16 Id)
		{
			this.Server = Framework.GetInstance();
			this.form = MainWindow2.GetInstance();
			this.Id = Id;
			this.PMPlayer = (GraalPlayer)this.Server.PlayerManager.FindPlayer(this.Id);
			this.CenterToScreen();
			
			string TitleText = "";
			TitleText = this.PMPlayer.Account;
			if (this.PMPlayer.Nickname != "" && this.PMPlayer.Nickname != null)
				TitleText +=  ": " + this.PMPlayer.Nickname;
			this.Text = TitleText;
			this.InitializeComponent(Id);
			this.richTextBox1.ReadOnly = true;
			//this.listBox1 = listBox1;

			//e.
			/*
			try
			{
				this.InitializeComponent();
				this.Show();
			}
			catch (SyntaxErrorException e)
			{
				this.form.Write_Text(e.Message + "\r\n");
			}
			*/
		}

		/// <summary>
		/// Set Properties
		/// </summary>
		internal void SetMessage(CString Packet)
		{
			if (this.richTextBox1.InvokeRequired)
			{
				SetTextCallback d = new SetTextCallback(SetMessage);
				this.Invoke(d, new object[] { Packet });
			}
			else
			{
				//this.richTextBox1.AppendText(text);
				
				this.LastMessage = Packet;
				//this.Enabled = true;
				//this.richTextBox1.Enabled = true;
				this.richTextBox1.AppendText(this.PMPlayer.Nickname + " says:\r\n"+ CString.untokenize(this.LastMessage.Text)+"\r\n");

				this.Show();
			}
		}

		internal void SendMessage()
		{
			this.Server.SendPM(this.Id, CString.tokenize(this.richTextBox4.Text), true);
			this.richTextBox1.AppendText("I say:\r\n" + this.richTextBox4.Text + "\r\n\r\n");
			this.richTextBox4.Text = "";
		}

		private void RemovePMWindow(object sender, FormClosedEventArgs e)
		{
			//if (this.form.InvokeRequired)
			{
			//	PMRMCallback d = new PMRMCallback(RemovePMWindow);
			//	this.Invoke(d, new object[] { sender, e });
			}
			//else
			{
				this.form.PMWindowManager.DeletePMWindow(this.Id);
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			this.SendMessage();
		}
	}
}
