using System;
using System.Text;

namespace CS_NPCServer
{
	/// <summary>
	/// Class: ServerWeapon Reference
	/// </summary>
	public class ServerWeapon : IRefObject
	{
		/// <summary>
		/// Member Variables
		/// </summary>
		internal NPCServer Server;
		internal String Image, Name;

		/// <summary>
		/// Constructor
		/// </summary>
		internal ServerWeapon(NPCServer Server, String WeaponName, String WeaponImage, String WeaponScript)
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
		public ScriptWeapon(NPCServer Server, IRefObject Ref)
			: base(Server)
		{
			this.Ref = (ServerWeapon)Ref;
		}
	};
}
