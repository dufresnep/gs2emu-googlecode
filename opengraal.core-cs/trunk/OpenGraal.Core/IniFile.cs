﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

namespace OpenGraal.Core
{
	public class IniFile
	{
		private Dictionary<string, Dictionary<string, string>> _iniFileContent = new Dictionary<string, Dictionary<string, string>>();
		private readonly Regex _sectionRegex = new Regex(@"(?<=\[)(?<SectionName>[^\]]+)(?=\])");
		private readonly Regex _keyValueRegex = new Regex(@"(?<Key>[^=]+)=(?<Value>.+)");
		private readonly Regex _singleLineCommentRegex = new Regex(@"^\s*#.*$");
		private string _filename = "";

		public IniFile()
		{
		}

		/// <summary>
		/// Get a specific value from the .ini file
		/// </summary>
		/// <param name="sectionName"></param>
		/// <param name="key"></param>
		/// <returns>The value of the given key in the given section, or NULL if not found</returns>
		public string GetValue(string sectionName, string key, string def = "")
		{
			if (_iniFileContent.ContainsKey(sectionName) && _iniFileContent[sectionName].ContainsKey(key))
				return _iniFileContent[sectionName][key];
			return def;
		}

		public int GetValueInt(string sectionName, string key, int def)
		{
			String val = GetValue(sectionName, key, def.ToString());
			try
			{
				return int.Parse(val);
			}
			catch (Exception)
			{
				return def;
			}
		}

		public bool GetValueBool(string sectionName, string key, bool def)
		{
			String val = GetValue(sectionName, key, def.ToString());
			try
			{
				return Convert.ToBoolean(val);
			}
			catch (Exception)
			{
				return def;
			}
		}

		/// <summary>
		/// Set a specified value in a section (int)
		/// </summary>
		public void SetValue(string sectionName, string key, int value)
		{
			SetValue(sectionName, key, value.ToString());
		}

		/// <summary>
		/// Set a specific value in a section
		/// </summary>
		/// <param name="sectionName"></param>
		/// <param name="key"></param>
		/// <param name="value"></param>
		public void SetValue(string sectionName, string key, string value)
		{
			if (!_iniFileContent.ContainsKey(sectionName))
				_iniFileContent[sectionName] = new Dictionary<string, string>();
			_iniFileContent[sectionName][key] = value;
		}

		/// <summary>
		/// Get all the Values for a section
		/// </summary>
		/// <param name="sectionName"></param>
		/// <returns>A Dictionary with all the Key/Values for that section (maybe empty but never null)</returns>
		public Dictionary<string, string> GetSection(string sectionName)
		{
			if (_iniFileContent.ContainsKey(sectionName))
				return new Dictionary<string, string>(_iniFileContent[sectionName]);
			else
				return new Dictionary<string, string>();
		}

		/// <summary>
		/// Set an entire sections values
		/// </summary>
		/// <param name="sectionName"></param>
		/// <param name="sectionValues"></param>
		public void SetSection(string sectionName, IDictionary<string, string> sectionValues)
		{
			if (sectionValues == null)
				return;
			_iniFileContent[sectionName] = new Dictionary<string, string>(sectionValues);
		}

		/// <summary>
		/// Load an .INI File
		/// </summary>
		/// <param name="filename"></param>
		/// <returns></returns>
		public bool Load(string filename)
		{
			this.Filename = filename;
			if (File.Exists(filename))
			{
				try
				{
					var content = File.ReadAllLines(filename);
					int com = 0;
					_iniFileContent = new Dictionary<string, Dictionary<string, string>>();
					string currentSectionName = string.Empty;
					foreach (var line in content)
					{
						Match m = _sectionRegex.Match(line);
						if (m.Success)
							currentSectionName = m.Groups["SectionName"].Value.Trim();
						else
						{
							m = _keyValueRegex.Match(line);
							if (m.Success)
							{
								string key = m.Groups["Key"].Value.Trim();
								string value = m.Groups["Value"].Value.Trim();

								Dictionary<string, string> kvpList;
								if (_iniFileContent.ContainsKey(currentSectionName))
									kvpList = _iniFileContent[currentSectionName];
								else
									kvpList = new Dictionary<string, string>();
								kvpList[key] = value;
								_iniFileContent[currentSectionName] = kvpList;
							}

							m = _singleLineCommentRegex.Match(line);
							if (m.Success)
							{
								com++;
								string key = m.Value.Trim();

								Dictionary<string, string> kvpList;
								if (_iniFileContent.ContainsKey(currentSectionName))
									kvpList = _iniFileContent[currentSectionName];
								else
									kvpList = new Dictionary<string, string>();
								kvpList["commentkey" + com.ToString()] = key;
								_iniFileContent[currentSectionName] = kvpList;
							}
						}
					}
					return true;
				}
				catch
				{
					return false;
				}

			}
			else
			{
				return false;
			}
		}

		public bool Save()
		{
			return this.Save(this.Filename);
		}

		/// <summary>
		/// Save the content of this class to an INI File
		/// </summary>
		/// <param name="filename"></param>
		/// <returns></returns>
		public bool Save(string filename)
		{
			var sb = GetContentAsStringBuilder();
			try
			{
				File.WriteAllText(filename, sb.ToString());
				return true;
			}
			catch
			{
				return false;
			}
		}

		private StringBuilder GetContentAsStringBuilder()
		{
			var sb = new StringBuilder();
			if (_iniFileContent != null)
			{
				foreach (var sectionName in _iniFileContent)
				{
					if (sectionName.Key != string.Empty)
						sb.AppendFormat("[{0}]\r\n", sectionName.Key);
					
					foreach (var keyValue in sectionName.Value)
					{
						if (keyValue.Key.Contains("commentkey"))
							sb.AppendFormat("{0}\r\n", keyValue.Value);
						else
							sb.AppendFormat("{0} = {1}\r\n", keyValue.Key, keyValue.Value);
					}
					sb.AppendLine();
				}
			}
			return sb;
		}

		public string Filename
		{
			get
			{
				return this._filename;
			}
			set
			{
				this._filename = value;
			}
		}

		public override string ToString()
		{
			var sb = GetContentAsStringBuilder();
			return sb.ToString();
		}
	}
}