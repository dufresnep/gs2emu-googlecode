namespace OpenGraal.MapGenerator
{
	partial class MapGenWindow
	{
		#region Member Variables
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer _components = null;
		private System.Windows.Forms.Label _mapPrefixLabel, _widthLabel, _heightLabel, _appVersionLabel;
		private System.Windows.Forms.TextBox _mapPrefixTextBox;
		private System.Windows.Forms.NumericUpDown _mapWidthNumeric, _mapHeightNumeric;
		private System.Windows.Forms.Button _mapGenerateButton;
		private System.Windows.Forms.CheckBox _openFolderCheck, _autoMappingCheck, _loadFullMapCheck;
		#endregion

		#region Protected Overrides
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (this._components != null))
			{
				_components.Dispose();
			}

			base.Dispose(disposing);
		}
		#endregion

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MapGenWindow));
			this._mapPrefixLabel = new System.Windows.Forms.Label();
			this._mapPrefixTextBox = new System.Windows.Forms.TextBox();
			this._mapWidthNumeric = new System.Windows.Forms.NumericUpDown();
			this._widthLabel = new System.Windows.Forms.Label();
			this._heightLabel = new System.Windows.Forms.Label();
			this._mapHeightNumeric = new System.Windows.Forms.NumericUpDown();
			this._mapGenerateButton = new System.Windows.Forms.Button();
			this._openFolderCheck = new System.Windows.Forms.CheckBox();
			this._autoMappingCheck = new System.Windows.Forms.CheckBox();
			this._loadFullMapCheck = new System.Windows.Forms.CheckBox();
			this._appVersionLabel = new System.Windows.Forms.Label();

			((System.ComponentModel.ISupportInitialize)(this._mapWidthNumeric)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this._mapHeightNumeric)).BeginInit();
			this.SuspendLayout();

			/// <summary>
			/// The label for which prefix the map should have
			/// </summary>
			this._mapPrefixLabel.AutoSize = true;
			this._mapPrefixLabel.Location = new System.Drawing.Point(6, 12);
			this._mapPrefixLabel.Name = "MapPrefixLabel";
			this._mapPrefixLabel.Size = new System.Drawing.Size(57, 13);
			this._mapPrefixLabel.TabIndex = 0;
			this._mapPrefixLabel.Text = "Map Prefix";

			// 
			// MapPrefixTextBox
			// 
			this._mapPrefixTextBox.Location = new System.Drawing.Point(69, 9);
			this._mapPrefixTextBox.Name = "MapPrefixTextBox";
			this._mapPrefixTextBox.Size = new System.Drawing.Size(116, 20);
			this._mapPrefixTextBox.TabIndex = 1;

			// 
			// MapWidthNumeric
			// 
			this._mapWidthNumeric.Location = new System.Drawing.Point(50, 38);
			this._mapWidthNumeric.Name = "MapWidthNumeric";
			this._mapWidthNumeric.ReadOnly = true;
			this._mapWidthNumeric.Size = new System.Drawing.Size(44, 20);
			this._mapWidthNumeric.TabIndex = 2;
			this._mapWidthNumeric.ValueChanged += new System.EventHandler(this.MapWidthNumeric_ValueChanged);

			// 
			// label2
			// 
			this._widthLabel.AutoSize = true;
			this._widthLabel.Location = new System.Drawing.Point(9, 41);
			this._widthLabel.Name = "label2";
			this._widthLabel.Size = new System.Drawing.Size(35, 13);
			this._widthLabel.TabIndex = 3;
			this._widthLabel.Text = "Width";

			// 
			// label3
			// 
			this._heightLabel.AutoSize = true;
			this._heightLabel.Location = new System.Drawing.Point(100, 41);
			this._heightLabel.Name = "label3";
			this._heightLabel.Size = new System.Drawing.Size(38, 13);
			this._heightLabel.TabIndex = 4;
			this._heightLabel.Text = "Height";

			// 
			// MapHeightNumeric
			// 
			this._mapHeightNumeric.Location = new System.Drawing.Point(141, 38);
			this._mapHeightNumeric.Name = "MapHeightNumeric";
			this._mapHeightNumeric.ReadOnly = true;
			this._mapHeightNumeric.Size = new System.Drawing.Size(44, 20);
			this._mapHeightNumeric.TabIndex = 5;
			this._mapHeightNumeric.ValueChanged += new System.EventHandler(this.MapHeightNumeric_ValueChanged);

			// 
			// MapGenerateButton
			// 
			this._mapGenerateButton.Enabled = false;
			this._mapGenerateButton.Location = new System.Drawing.Point(10, 134);
			this._mapGenerateButton.Name = "MapGenerateButton";
			this._mapGenerateButton.Size = new System.Drawing.Size(174, 23);
			this._mapGenerateButton.TabIndex = 6;
			this._mapGenerateButton.Text = "Generate";
			this._mapGenerateButton.UseVisualStyleBackColor = true;
			this._mapGenerateButton.Click += new System.EventHandler(this.MapGenerateButton_Click);

			// 
			// OpenFolderCheck
			// 
			this._openFolderCheck.AutoSize = true;
			this._openFolderCheck.Checked = true;
			this._openFolderCheck.CheckState = System.Windows.Forms.CheckState.Checked;
			this._openFolderCheck.Location = new System.Drawing.Point(12, 65);
			this._openFolderCheck.Name = "OpenFolderCheck";
			this._openFolderCheck.Size = new System.Drawing.Size(137, 17);
			this._openFolderCheck.TabIndex = 7;
			this._openFolderCheck.Text = "Open folder when done";
			this._openFolderCheck.UseVisualStyleBackColor = true;

			// 
			// AutoMappingCheck
			// 
			this._autoMappingCheck.AutoSize = true;
			this._autoMappingCheck.Location = new System.Drawing.Point(12, 88);
			this._autoMappingCheck.Name = "AutoMappingCheck";
			this._autoMappingCheck.Size = new System.Drawing.Size(107, 17);
			this._autoMappingCheck.TabIndex = 8;
			this._autoMappingCheck.Text = "No auto-mapping";
			this._autoMappingCheck.UseVisualStyleBackColor = true;

			// 
			// LoadFullMapCheck
			// 
			this._loadFullMapCheck.AutoSize = true;
			this._loadFullMapCheck.Location = new System.Drawing.Point(12, 111);
			this._loadFullMapCheck.Name = "LoadFullMapCheck";
			this._loadFullMapCheck.Size = new System.Drawing.Size(89, 17);
			this._loadFullMapCheck.TabIndex = 9;
			this._loadFullMapCheck.Text = "Load full map";
			this._loadFullMapCheck.UseVisualStyleBackColor = true;

			// 
			// AppVersionLabel
			// 
			this._appVersionLabel.AutoSize = true;
			this._appVersionLabel.Location = new System.Drawing.Point(11, 160);
			this._appVersionLabel.Name = "AppVersionLabel";
			this._appVersionLabel.Size = new System.Drawing.Size(18, 26);
			this._appVersionLabel.TabIndex = 10;
			this._appVersionLabel.Text = "";

			// 
			// MapGenWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(195, 193);

			this.Controls.Add(this._appVersionLabel);
			this.Controls.Add(this._loadFullMapCheck);
			this.Controls.Add(this._autoMappingCheck);
			this.Controls.Add(this._openFolderCheck);
			this.Controls.Add(this._mapGenerateButton);
			this.Controls.Add(this._mapHeightNumeric);
			this.Controls.Add(this._heightLabel);
			this.Controls.Add(this._widthLabel);
			this.Controls.Add(this._mapWidthNumeric);
			this.Controls.Add(this._mapPrefixTextBox);
			this.Controls.Add(this._mapPrefixLabel);

			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "MapGenWindow";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Graal Map Generator";

			((System.ComponentModel.ISupportInitialize)(this._mapWidthNumeric)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this._mapHeightNumeric)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();
		}
		#endregion
	}
}