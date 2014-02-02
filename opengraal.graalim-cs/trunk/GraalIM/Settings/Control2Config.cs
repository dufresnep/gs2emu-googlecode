using System;

namespace OpenGraal.GraalIM
{
	public class Control2Config : OpenGraal.Core.IniFile
	{
		private static Control2Config _instance = new Control2Config();
		public static Control2Config GetInstance()
		{
			return _instance;
		}

		private Control2Config() : base()
		{
		}

		#region Private variables
		private bool _noMassMessages = false, _noMassIfClientOn = false, _noHtmlInPms = false, _noHtmlImages = true, _attachAway = false, _logRCChat = false,
			_dontSavePassword = false, _graphicalMenu = true, _globalPms = true, _buddyTracking = false, _showBuddies = false, _syntaxHighLighting = true,
			_autoIndenting = true, _smartHomeEnd = false, _showBrackets = true, _showLineNumbers = false, _loaded = false;

		private int _chatFontSize=10, _scriptTabWidth=2, _scriptFontSize=10;

		private string _chatLogFile = "", _downloadFolder = "", _nickname = null, _webBrowsers="firefox,mozilla,konqueror,netscape",
			_background = null, _iconPlayerList = null, _iconPlayerListPressed = null, _iconFileBrowser = null, _iconFileBrowserPressed = null, _iconAccounts = null,
			_iconAccountsPressed = null, _iconToalls = null, _iconToallsPressed = null,			_iconOptions = null, _iconOptionsPressed = null, _iconServerFlags = null,
			_iconServerFlagsPressed = null, _iconFolderOptions = null, _iconFolderOptionsPressed = null, _iconServerOptions = null, _iconServerOptionsPressed = null,
			_iconLocalNpcs = null, _iconLocalNpcsPressed = null, _iconLevelList = null, _iconLevelListPressed = null, _iconClassList = null, _iconClassListPressed = null,
			_iconWeaponList = null, _iconWeaponListPressed = null, _iconNpcList = null, _iconNpcListPressed = null, _colorEdit = null, _colorEditBack = null,
			_colorChat = null, _colorChatBack = null, _colorChatBold = null, _colorLabel = null, _colorLabelBack = null, _labelServers = null, _labelPlayers = null,
			_labelNpcServer = null;
		#endregion

		public bool Loaded
		{
			get { return this._loaded; }
			set { this._loaded = value; }
		}

		public bool NoMassMessages
		{
			get
			{
				this._noMassMessages = this.GetValueBool("", "nomassmessages", false);
				return this._noMassMessages;
			}
			set
			{
				this._noMassMessages = value;

				this.SetValue("", "nomassmessages", this._noMassMessages.ToString());
				this.Save();

			}
		}

		public bool NoMassIfClientOn
		{
			get
			{
				this._noMassIfClientOn = this.GetValueBool("", "nomassifclienton", false);
				return this._noMassIfClientOn;
			}
			set
			{
				this._noMassIfClientOn = value;

				this.SetValue("", "nomassifclienton", this._noMassIfClientOn.ToString());
				this.Save();

			}
		}

		public bool NoHtmlInPms
		{
			get
			{
				this._noHtmlInPms = this.GetValueBool("", "nohtmlinpms", false);
				return this._noHtmlInPms;
			}
			set
			{
				this._noHtmlInPms = value;

				this.SetValue("", "nohtmlinpms", this._noHtmlInPms.ToString());
				this.Save();

			}
		}

		public bool NoHtmlImages
		{
			get
			{
				this._noHtmlImages = this.GetValueBool("", "nohtmlimages", true);
				return this._noHtmlImages;
			}
			set
			{
				this._noHtmlImages = value;

				this.SetValue("", "nohtmlimages", this._noHtmlImages.ToString());
				this.Save();

			}
		}

		public bool AttachAway
		{
			get
			{
				this._attachAway = this.GetValueBool("", "attachaway", false);
				return this._attachAway;
			}
			set
			{
				this._attachAway = value;

				this.SetValue("", "attachaway", this._attachAway.ToString());
				this.Save();

			}
		}

		public bool LogRCChat
		{
			get
			{
				this._logRCChat = this.GetValueBool("", "logrcchat", false);
				return this._logRCChat;
			}
			set
			{
				this._logRCChat = value;

				this.SetValue("", "logrcchat", this._logRCChat.ToString());
				this.Save();

			}
		}

		public bool DontSavePassword
		{
			get
			{
				this._dontSavePassword = this.GetValueBool("", "dontsavepassword", false);
				return this._dontSavePassword;
			}
			set
			{
				this._dontSavePassword = value;

				this.SetValue("", "dontsavepassword", this._dontSavePassword.ToString());
				this.Save();

			}
		}

		public bool GraphicalMenu
		{
			get
			{
				this._graphicalMenu = this.GetValueBool("", "graphicalmenu", true);
				return this._graphicalMenu;
			}
			set
			{
				this._graphicalMenu = value;

				this.SetValue("", "graphicalmenu", this._graphicalMenu.ToString());
				this.Save();

			}
		}

		public bool GlobalPms
		{
			get
			{
				this._globalPms = this.GetValueBool("", "globalpms", true);
				return this._globalPms;
			}
			set
			{
				this._globalPms = value;

				this.SetValue("", "globalpms", this._globalPms.ToString());
				this.Save();

			}
		}

		public bool BuddyTracking
		{
			get
			{
				this._buddyTracking = this.GetValueBool("", "buddytracking", false);
				return this._buddyTracking;
			}
			set
			{
				this._buddyTracking = value;

				this.SetValue("", "buddytracking", this._buddyTracking.ToString());
				this.Save();

			}
		}

		public bool ShowBuddies
		{
			get
			{
				this._showBuddies = this.GetValueBool("", "showbuddies", false);
				return this._showBuddies;
			}
			set
			{
				this._showBuddies = value;

				this.SetValue("", "showbuddies", this._showBuddies.ToString());
				this.Save();

			}
		}

		public bool SyntaxHighLighting
		{
			get
			{
				this._syntaxHighLighting = this.GetValueBool("", "syntaxhighlighting", true);
				return this._syntaxHighLighting;
			}
			set
			{
				this._syntaxHighLighting = value;

				this.SetValue("", "syntaxhighlighting", this._syntaxHighLighting.ToString());
				this.Save();

			}
		}

		public bool AutoIndenting
		{
			get
			{
				this._autoIndenting = this.GetValueBool("", "autoindenting", true);
				return this._autoIndenting;
			}
			set
			{
				this._autoIndenting = value;

				this.SetValue("", "autoindenting", this._autoIndenting.ToString());
				this.Save();

			}
		}

		public bool SmartHomeEnd
		{
			get
			{
				this._smartHomeEnd = this.GetValueBool("", "smarthomeend", false);
				return this._smartHomeEnd;
			}
			set
			{
				this._smartHomeEnd = value;

				this.SetValue("", "smarthomeend", this._smartHomeEnd.ToString());
				this.Save();

			}
		}

		public bool ShowBrackets
		{
			get
			{
				this._showBrackets = this.GetValueBool("", "showbrackets", true);
				return this._showBrackets;
			}
			set
			{
				this._showBrackets = value;

				this.SetValue("", "showbrackets", this._showBrackets.ToString());
				this.Save();

			}
		}

		public bool ShowLineNumbers
		{
			get
			{
				this._showLineNumbers = this.GetValueBool("", "showlinenumbers", false);
				return this._showLineNumbers;
			}
			set
			{
				this._showLineNumbers = value;

				this.SetValue("", "showlinenumbers", this._showLineNumbers.ToString());
				this.Save();

			}
		}

		public int ChatFontSize
		{
			get
			{
				this._chatFontSize = this.GetValueInt("", "chatfontsize", 10);
				return this._chatFontSize;
			}
			set
			{
				this._chatFontSize = value;

				this.SetValue("", "chatfontsize", this._chatFontSize);
				this.Save();

			}
		}

		public int ScriptTabWidth
		{
			get
			{
				this._scriptTabWidth = this.GetValueInt("", "scripttabwidth", 2);
				return this._scriptTabWidth;
			}
			set
			{
				this._scriptTabWidth = value;

				this.SetValue("", "scripttabwidth", this._scriptTabWidth);
				this.Save();

			}
		}

		public int ScriptFontSize
		{
			get
			{
				this._scriptFontSize = this.GetValueInt("", "scriptfontsize", 10);
				return this._scriptFontSize;
			}
			set
			{
				this._scriptFontSize = value;

				this.SetValue("", "scriptfontsize", this._scriptFontSize);
				this.Save();

			}
		}

		public string Nickname
		{
			get
			{
				this._nickname = this.GetValue("", "nickname", "Guest");
				return this._nickname;
			}
			set
			{
				this._nickname = value;

				this.SetValue("", "nickname", this._nickname);
				this.Save();

			}
		}

		public string ChatLogFile
		{
			get
			{
				this._chatLogFile = this.GetValue("", "chatlogfile", "rclog.txt");
				return this._chatLogFile;
			}
			set
			{
				this._chatLogFile = value;

				this.SetValue("", "chatlogfile", this._chatLogFile);
				this.Save();
			}
		}

		public string DownloadFolder
		{
			get
			{
				this._downloadFolder = this.GetValue("", "downloadfolder", "");
				return this._downloadFolder;
			}
			set
			{
				this._downloadFolder = value;

				this.SetValue("", "downloadfolder", this._downloadFolder);
				this.Save();
			}
		}

		public string WebBrowsers
		{
			get
			{
				this._webBrowsers = this.GetValue("", "webbrowsers", "");
				return this._webBrowsers;
			}
			set
			{
				this._webBrowsers = value;

				this.SetValue("", "webbrowsers", this._webBrowsers);
				this.Save();
			}
		}

		public string Background
		{
			get
			{
				this._background = this.GetValue("", "background", "rc_opengraal.png");
				return this._background;
			}
			set
			{
				this._background = value;

				this.SetValue("", "background", this._background);
				this.Save();
			}
		}

		public string IconPlayerList
		{
			get
			{
				this._iconPlayerList = this.GetValue("", "iconplayerlist", "rc_playerlist_normal.png");
				return this._iconPlayerList;
			}
			set
			{
				this._iconPlayerList = value;

				this.SetValue("", "iconplayerlist", this._iconPlayerList);
				this.Save();
			}
		}

		public string IconPlayerListPressed
		{
			get
			{
				this._iconPlayerListPressed = this.GetValue("", "iconplayerlistpressed", "rc_playerlist_pressed.png");
				return this._iconPlayerListPressed;
			}
			set
			{
				this._iconPlayerListPressed = value;

				this.SetValue("", "iconplayerlistpressed", this._iconPlayerListPressed);
				this.Save();
			}
		}

		public string IconFileBrowser
		{
			get
			{
				this._iconFileBrowser = this.GetValue("", "iconfilebrowser", "rc_filebrowser_normal.png");
				return this._iconFileBrowser;
			}
			set
			{
				this._iconFileBrowser = value;

				this.SetValue("", "iconfilebrowser", this._iconFileBrowser);
				this.Save();
			}
		}

		public string IconFileBrowserPressed
		{
			get
			{
				this._iconFileBrowserPressed = this.GetValue("", "iconfilebrowserpressed", "rc_filebrowser_pressed.png");
				return this._iconFileBrowserPressed;
			}
			set
			{
				this._iconFileBrowserPressed = value;

				this.SetValue("", "iconfilebrowserpressed", this._iconFileBrowserPressed);
				this.Save();
			}
		}

		public string IconAccounts
		{
			get
			{
				this._iconAccounts = this.GetValue("", "iconaccounts", "rc_accounts_normal.png");
				return this._iconAccounts;
			}
			set
			{
				this._iconAccounts = value;

				this.SetValue("", "iconaccounts", this._iconAccounts);
				this.Save();
			}
		}

		public string IconAccountsPressed
		{
			get
			{
				this._iconAccountsPressed = this.GetValue("", "iconaccountspressed", "rc_accounts_pressed.png");
				return this._iconAccountsPressed;
			}
			set
			{
				this._iconAccountsPressed = value;

				this.SetValue("", "iconaccountspressed", this._iconAccountsPressed);
				this.Save();
			}
		}

		public string IconToalls
		{
			get
			{
				this._iconToalls = this.GetValue("", "icontoalls", "rc_toalls_normal.png");
				return this._iconToalls;
			}
			set
			{
				this._iconToalls = value;

				this.SetValue("", "icontoalls", this._iconToalls);
				this.Save();
			}
		}

		public string IconToallsPressed
		{
			get
			{
				this._iconToallsPressed = this.GetValue("", "icontoallspressed", "rc_toalls_pressed.png");
				return this._iconToallsPressed;
			}
			set
			{
				this._iconToallsPressed = value;

				this.SetValue("", "icontoallspressed", this._iconToallsPressed);
				this.Save();
			}
		}

		public string IconOptions
		{
			get
			{
				this._iconOptions = this.GetValue("", "iconoptions", "rc_options_normal.png");
				return this._iconOptions;
			}
			set
			{
				this._iconOptions = value;

				this.SetValue("", "iconoptions", this._iconOptions);
				this.Save();
			}
		}

		public string IconOptionsPressed
		{
			get
			{
				this._iconOptionsPressed = this.GetValue("", "iconoptionspressed", "rc_options_pressed.png");
				return this._iconOptionsPressed;
			}
			set
			{
				this._iconOptionsPressed = value;

				this.SetValue("", "iconoptionspressed", this._iconOptionsPressed);
				this.Save();
			}
		}

		public string IconServerFlags
		{
			get
			{
				this._iconServerFlags = this.GetValue("", "iconserverflags", "rc_serverflags_normal.png");
				return this._iconServerFlags;
			}
			set
			{
				this._iconServerFlags = value;

				this.SetValue("", "iconserverflags", this._iconServerFlags);
				this.Save();
			}
		}

		public string IconServerFlagsPressed
		{
			get
			{
				this._iconServerFlagsPressed = this.GetValue("", "iconserverflagspressed", "rc_serverflags_pressed.png");
				return this._iconServerFlagsPressed;
			}
			set
			{
				this._iconServerFlagsPressed = value;

				this.SetValue("", "iconserverflagspressed", this._iconServerFlagsPressed);
				this.Save();
			}
		}

		public string IconFolderOptions
		{
			get
			{
				this._iconFolderOptions = this.GetValue("", "iconfolderoptions", "rc_folderoptions_normal.png");
				return this._iconFolderOptions;
			}
			set
			{
				this._iconFolderOptions = value;

				this.SetValue("", "iconfolderoptions", this._iconFolderOptions);
				this.Save();
			}
		}

		public string IconFolderOptionsPressed
		{
			get
			{
				this._iconFolderOptionsPressed = this.GetValue("", "iconfolderoptionspressed", "rc_folderoptions_pressed.png");
				return this._iconFolderOptionsPressed;
			}
			set
			{
				this._iconFolderOptionsPressed = value;

				this.SetValue("", "iconfolderoptionspressed", this._iconFolderOptionsPressed);
				this.Save();
			}
		}

		public string IconServerOptions
		{
			get
			{
				this._iconServerOptions = this.GetValue("", "iconserveroptions", "rc_serveroptions_normal.png");
				return this._iconServerOptions;
			}
			set
			{
				this._iconServerOptions = value;

				this.SetValue("", "iconserveroptions", this._iconServerOptions);
				this.Save();
			}
		}

		public string IconServerOptionsPressed
		{
			get
			{
				this._iconServerOptionsPressed = this.GetValue("", "iconserveroptionspressed", "rc_serveroptions_pressed.png");
				return this._iconServerOptionsPressed;
			}
			set
			{
				this._iconServerOptionsPressed = value;

				this.SetValue("", "iconserveroptionspressed", this._iconServerOptionsPressed);
				this.Save();
			}
		}

		public string IconLocalNpcs
		{
			get
			{
				this._iconLocalNpcs = this.GetValue("", "iconlocalnpcs", "rc_localnpcs_normal.png");
				return this._iconLocalNpcs;
			}
			set
			{
				this._iconLocalNpcs = value;

				this.SetValue("", "iconlocalnpcs", this._iconLocalNpcs);
				this.Save();
			}
		}

		public string IconLocalNpcsPressed
		{
			get
			{
				this._iconLocalNpcsPressed = this.GetValue("", "iconlocalnpcspressed", "rc_localnpcs_pressed.png");
				return this._iconLocalNpcsPressed;
			}
			set
			{
				this._iconLocalNpcsPressed = value;

				this.SetValue("", "iconlocalnpcspressed", this._iconLocalNpcsPressed);
				this.Save();
			}
		}

		public string IconLevelList
		{
			get
			{
				this._iconLevelList = this.GetValue("", "iconlevellist", "rc_levellist_normal.png");
				return this._iconLevelList;
			}
			set
			{
				this._iconLevelList = value;

				this.SetValue("", "iconlevellist", this._iconLevelList);
				this.Save();
			}
		}

		public string IconLevelListPressed
		{
			get
			{
				this._iconLevelListPressed = this.GetValue("", "iconlevellistpressed", "rc_levellist_pressed.png");
				return this._iconLevelListPressed;
			}
			set
			{
				this._iconLevelListPressed = value;

				this.SetValue("", "iconlevellistpressed", this._iconLevelListPressed);
				this.Save();
			}
		} 

		public string IconClassList
		{
			get
			{
				this._iconClassList = this.GetValue("", "iconclasslist", "rc_classlist_normal.png");
				return this._iconClassList;
			}
			set
			{
				this._iconClassList = value;

				this.SetValue("", "iconclasslist", this._iconClassList);
				this.Save();
			}
		}

		public string IconClassListPressed
		{
			get
			{
				this._iconClassListPressed = this.GetValue("", "iconclasslistpressed", "rc_classlist_pressed.png");
				return this._iconClassListPressed;
			}
			set
			{
				this._iconClassListPressed = value;

				this.SetValue("", "iconclasslistpressed", this._iconClassListPressed);
				this.Save();
			}
		}

		public string IconWeaponList
		{
			get
			{
				this._iconWeaponList = this.GetValue("", "iconweaponlist", "rc_weaponlist_normal.png");
				return this._iconWeaponList;
			}
			set
			{
				this._iconWeaponList = value;

				this.SetValue("", "iconweaponlist", this._iconWeaponList);
				this.Save();
			}
		}

		public string IconWeaponListPressed
		{
			get
			{
				this._iconWeaponListPressed = this.GetValue("", "iconweaponlistpressed", "rc_weaponlist_pressed.png");
				return this._iconWeaponListPressed;
			}
			set
			{
				this._iconWeaponListPressed = value;

				this.SetValue("", "iconweaponlistpressed", this._iconWeaponListPressed);
				this.Save();
			}
		}

		public string IconNpclist
		{
			get
			{
				this._iconNpcList = this.GetValue("", "iconnpclist", "rc_npclist_normal.png");
				return this._iconNpcList;
			}
			set
			{
				this._iconNpcList = value;

				this.SetValue("", "iconnpclist", this._iconNpcList);
				this.Save();
			}
		}

		public string IconNpclistPressed
		{
			get
			{
				this._iconNpcListPressed = this.GetValue("", "iconnpclistpressed", "rc_npclist_pressed.png");
				return this._iconNpcListPressed;
			}
			set
			{
				this._iconNpcListPressed = value;

				this.SetValue("", "iconnpclistpressed", this._iconNpcListPressed);
				this.Save();
			}
		}

 		public string ColorEdit
		{
			get
			{
				this._colorEdit = this.GetValue("", "coloredit", "#00ff00");
				return this._colorEdit;
			}
			set
			{
				this._colorEdit = value;

				this.SetValue("", "coloredit", this._colorEdit);
				this.Save();
			}
		}

		public string ColorEditBack
		{
			get
			{
				this._colorEditBack = this.GetValue("", "coloreditback", "#006000");
				return this._colorEditBack;
			}
			set
			{
				this._colorEditBack = value;

				this.SetValue("", "coloreditback", this._colorEditBack);
				this.Save();
			}
		}

		public string ColorChat
		{
			get
			{
				this._colorChat = this.GetValue("", "colorchat", "#000000");
				return this._colorChat;
			}
			set
			{
				this._colorChat = value;

				this.SetValue("", "colorchat", this._colorChat);
				this.Save();
			}
		}

		public string ColorChatBack
		{
			get
			{
				this._colorChatBack = this.GetValue("", "colorchatback", "#ffffff");
				return this._colorChatBack;
			}
			set
			{
				this._colorChatBack = value;

				this.SetValue("", "colorchatback", this._colorChatBack);
				this.Save();
			}
		}

		public string ColorChatBold
		{
			get
			{
				this._colorChatBold = this.GetValue("", "colorchatbold", "#00C000");
				return this._colorChatBold;
			}
			set
			{
				this._colorChatBold = value;

				this.SetValue("", "colorchatbold", this._colorChatBold);
				this.Save();
			}
		}

		public string ColorLabel
		{
			get
			{
				this._colorLabel = this.GetValue("", "colorlabel", "#40ff40");
				return this._colorLabel;
			}
			set
			{
				this._colorLabel = value;

				this.SetValue("", "colorlabel", this._colorLabel);
				this.Save();
			}
		}

		public string ColorLabelBack
		{
			get
			{
				this._colorLabelBack = this.GetValue("", "colorlabelback", "#000000");
				return this._colorLabelBack;
			}
			set
			{
				this._colorLabelBack = value;

				this.SetValue("", "colorlabelback", this._colorLabelBack);
				this.Save();
			}
		}

		public string LabelServers
		{
			get
			{
				this._labelServers = this.GetValue("", "labelservers", "Server:");
				return this._labelServers;
			}
			set
			{
				this._labelServers = value;

				this.SetValue("", "labelservers", this._labelServers);
				this.Save();
			}
		}

		public string LabelPlayers
		{
			get
			{
				this._labelPlayers = this.GetValue("", "labelplayers", "Players:");
				return this._labelPlayers;
			}
			set
			{
				this._labelPlayers = value;

				this.SetValue("", "labelplayers", this._labelPlayers);
				this.Save();
			}
		}

		public string LabelNpcServer
		{
			get
			{
				this._labelNpcServer = this.GetValue("", "labelnpcserver", "Connected to NPC-Server");
				return this._labelNpcServer;
			}
			set
			{
				this._labelNpcServer = value;

				this.SetValue("", "labelnpcserver", this._labelNpcServer);
				this.Save();
			}
		}
	}
}