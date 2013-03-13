using System;
using System.Text;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.NpcServer;
using OpenGraal.NpcServer.Players;

namespace OpenGraal.NpcServer
{
	/// <summary>
	/// Class: ServerWeapon Reference
	/// </summary>
	public class ServerWeapon : IRefObject
	{
		/// <summary>
		/// Member Variables
		/// </summary>
		internal Framework Server;
		internal String Image, Name;

		/// <summary>
		/// Constructor
		/// </summary>
		internal ServerWeapon(Framework Server, String WeaponName, String WeaponImage, String WeaponScript)
			: base(ScriptType.WEAPON)
		{
			this.Server = Server;
			this.UpdateWeapon(WeaponName, WeaponImage, WeaponScript);
		}

		/// <summary>
		/// Update Weapon Properties
		/// </summary>
		internal void UpdateWeapon(String WeaponName, String WeaponImage, String WeaponScript)
		{
			this.Name = WeaponName;
			this.Image = WeaponImage;
			this.Script = WeaponScript.Replace("\xa7", "\n");
		}

		/// <summary>
		/// Override -> Error Text
		/// </summary>
		override internal string GetErrorText()
		{
			return new StringBuilder(Name).ToString();
		}
	}

	/// <summary>
	/// Class: ScriptWeapon Object
	/// </summary>
	public class ScriptWeapon : ScriptObj
	{
		// -- Member Variables -- //
		internal readonly ServerWeapon Ref;
		public readonly bool isweapon = true;

		/// <summary>
		/// Name -> Read Only
		/// </summary>
		public string name
		{
			get { return Ref.Name; }
		}

		/// <summary>
		/// Image -> Read Only
		/// </summary>
		public string image
		{
			get { return Ref.Image; }
		}

		/// <summary>
		/// Constructor
		/// </summary>
		public ScriptWeapon() { }
		public ScriptWeapon(Framework Server, IRefObject Ref)
			: base(Server)
		{
			this.Ref = (ServerWeapon)Ref;
		}
	};
}
