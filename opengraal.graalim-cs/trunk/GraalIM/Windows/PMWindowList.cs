using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenGraal.GraalIM
{
	public class PMWindowList : IEnumerable
	{
		/// <summary>
		/// PMWindow-List
		/// </summary>
		public Dictionary<Int16, PMWindow> PMWindowLister = new Dictionary<Int16, PMWindow>();
		public List<PMWindow> PMWindowList2 = new List<PMWindow>();
		protected Int32 Position = -1;
		private static PMWindowList _instance = new PMWindowList();
		 
		public static PMWindowList GetInstance()
		{
			return _instance;
		}

		/// <summary>
		/// Constructor
		/// </summary>
		private PMWindowList()
		{
		}

		/// <summary>
		/// Iterate PMWindowlist Manager
		/// </summary>
		public IEnumerator<PMWindow> GetEnumerator()
		{
			foreach (KeyValuePair<Int16, PMWindow> PMWindow in PMWindowLister)
				yield return PMWindow.Value;
		}

		IEnumerator IEnumerable.GetEnumerator()
		{
			return GetEnumerator();
		}

		/// <summary>
		/// Add PMWindow to PMWindowlist (or return PMWindow)
		/// </summary>
		public PMWindow AddPMWindow(Int16 Id)
		{
			PMWindow pl = FindPMWindow(Id);
			
			if (pl == null)
			{
				PMWindow PMWindow = new PMWindow(Id);
				PMWindow.Id = Id;
				PMWindowLister[Id] = PMWindow;
				PMWindowList2.Add(PMWindow);
				return PMWindow;
			}

			return pl;
		}

		/// <summary>
		/// Delete PMWindow from PMWindowlist
		/// </summary>
		public bool DeletePMWindow(Int16 pId)
		{
			PMWindow PM = this.FindPMWindow(pId);
			PM = null;
			return PMWindowLister.Remove(pId);
		}

		/// <summary>
		/// Find PMWindow by Id
		/// </summary>
		public PMWindow FindPMWindow(Int16 Id)
		{
			PMWindow PMWindow;
			
			if (PMWindowLister.TryGetValue(Id, out PMWindow))
				return PMWindow;

			return null;
		}
		
		public int count()
		{
			return PMWindowLister.Count;
		}
	}
}
