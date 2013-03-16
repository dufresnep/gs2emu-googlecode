using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reflection;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using OpenGraal;
using OpenGraal.Common;
using OpenGraal.Common.Levels;

namespace OpenGraal.MapGenerator
{
	public partial class MapGenWindow : Form
	{
		#region Member Variables
		private string _appVersion = "";
		#endregion

		#region	Get-Value Functions
		public string AppVersion
		{
			get
			{
				return _appVersion;
			}
		}
		#endregion

		#region Constructor / Destructor
		public MapGenWindow()
		{
			this.InitializeComponent();
			this._appVersion = this.GetProductVersion();
			this._appVersionLabel.Text = "Graal Map Generator v" + this.AppVersion + "\nCreator: Emera (Aaron Yarborough)";
		}

		~MapGenWindow()
		{
			this.Dispose();
		}
		#endregion

		#region Private functions
		private void MapPrefixTextBox_TextChanged(object sender, EventArgs e)
		{
			this.CheckIfCanGenerate();
		}

		private void MapWidthNumeric_ValueChanged(object sender, EventArgs e)
		{
			this.CheckIfCanGenerate();
		}

		private void MapHeightNumeric_ValueChanged(object sender, EventArgs e) {
			this.CheckIfCanGenerate();
		}

		private void MapGenerateButton_Click(object sender, EventArgs e)
		{
			String TemplateFileName = "template.nw";
			
			String GmapPrefix = _mapPrefixTextBox.Text;
			String GmapFileName = GmapPrefix + ".gmap";

			GraalMap GMap = new GraalMap(GmapPrefix);
			GMap.MapWidth = Convert.ToInt32(_mapWidthNumeric.Value);
			GMap.MapHeight = Convert.ToInt32(_mapHeightNumeric.Value);
			GMap.AutoMapping = _autoMappingCheck.Checked;
			GMap.LoadFullMap = _loadFullMapCheck.Checked;

			int ret = GMap.Generate(TemplateFileName);

			String ApplicationDirectory = AppDomain.CurrentDomain.BaseDirectory;
			String TemplateFile = (ApplicationDirectory + TemplateFileName);
			String GmapDirectory = ApplicationDirectory + (GMap.MapName + "/");
			
			if (ret == 0) //Throw an error and close program if template not found
			{
				MessageBox.Show("Couldn't find the template level file (" + TemplateFileName + ")");
				Application.Exit();
			}

			this.Enabled = false;

			int ret2 = GMap.Save(GmapDirectory);
			if (ret2 == 0) //Throw an error if the directory already exists
			{
				MessageBox.Show("That map is already saved under this new map's directory. Please choose another name for your gmap or delete the old directory", "Application Error");
				return;
			}

			Boolean OpenFolderWhenDone = _openFolderCheck.Checked;

			if (OpenFolderWhenDone == true)
			{
				System.Diagnostics.Process.Start(@GmapDirectory);
			}
			MessageBox.Show(GMap.GetLevelAt(2,3));
			this.Enabled = true;
		}

		private string GetProductVersion()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			FileVersionInfo fvi = FileVersionInfo.GetVersionInfo(assembly.Location);
			return fvi.FileVersion.Substring(0, 3) + ((fvi.FileVersion.Substring(4, 1) != "0") ? fvi.FileVersion.Substring(4, 1) : "") + ((fvi.FileVersion.Substring(6, 1) != "0")?"r" + fvi.FileVersion.Substring(6, 1):"");
		}

		private void CheckIfCanGenerate()
		{
			Boolean CanGenerate = true;
			if (_mapPrefixTextBox.Text == "" || _mapWidthNumeric.Value < 2 || _mapHeightNumeric.Value < 2)
				CanGenerate = false;

			if (CanGenerate == false)
			{
				_mapGenerateButton.Enabled = false;
				_mapGenerateButton.Text = "Generate";
			}
			else
			{
				_mapGenerateButton.Enabled = true;
				_mapGenerateButton.Text = "Generate (" + (Convert.ToInt32(_mapWidthNumeric.Value) * Convert.ToInt32(_mapHeightNumeric.Value)) + " Levels)";
			}
		}
		#endregion

		#region Public functions
		
		#endregion
	}
}
