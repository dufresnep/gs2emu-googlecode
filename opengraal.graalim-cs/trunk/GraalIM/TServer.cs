using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenGraal;
using OpenGraal.Core;
using OpenGraal.GraalIM;

namespace OpenGraal.GraalIM
{
	public class TServer
	{ 
		/*
			Global Variables
		*/

		public string[] serverTypes = {"P ", "U ", "H ", "3 "};
		public string name, language, description, url, version, ip, type;
		public int playercount = 0, port = 0;
		//#define serverTypesCount 3

		/*
			Constructor
		*/

		public TServer()
		{
	
		}

		/*
			Destructor
		*/
		/*
		public void ~TServer()
		{
			if (item != NULL)
				delete item;

			ServerWindow->getServerList().remove(this);
		}
	*/

		/*
			Functions
		*/

		// get
		public string getDescription()
		{
			return this.description;
		}

		public string getIp()
		{
			return this.ip;
		}

		public string getLanguage()
		{
			return this.language;
		}

		public string getName()
		{
			return this.name;
		}

		public string getUrl()
		{
			return this.url;
		}

		public string getVersion()
		{
			return this.version;
		}

		public int getPCount()
		{
			return this.playercount;
		}

		public int getPort()
		{
			return this.port;
		}

		public string getType()
		{
			return this.type;
		}
		/*
		public void getItem(int pId)
		{
			// Declare Variables
			string imageName;

			// Set imageName by type
			switch (type)
			{
				case TYPE_HIDDEN: imageName = ":/RC/icon_hidden.png"; break;
				case TYPE_HOSTED: imageName = ":/RC/icon_hosted.png"; break;
				case TYPE_GOLD: imageName = ":/RC/icon_gold.png"; break;
				default: imageName = ":/RC/icon_classic.png"; break;
			}

			// Set Item
			item->setIcon(0, QIcon(imageName));
			item->setText(0, name.text());
			item->setText(1, toString(playercount).text());
			item->setStatusTip(0, toString(pId).text());
			return item;
		}
		*/
		// set
		public void setDescription(CString pBuffer)
		{
			this.description = pBuffer.Text.ToString();
		}

		public void setIp(CString pBuffer)
		{
			this.ip = pBuffer.Text.ToString();
		}

		public void setLanguage(CString pBuffer)
		{
			this.language = pBuffer.Text.ToString();
		}

		public void setName(CString pBuffer)
		{
			// Grab Type
			int i = 0;
			CString servername = pBuffer;
			string firsttwo = servername.ReadChars(2);
			this.type = "";
			foreach (string serverType in serverTypes)
			{
				if (firsttwo == serverType)
				{
					pBuffer.Remove(0, 2);
					//Console.WriteLine(pBuffer.Text);
					this.type = firsttwo.ToString();
					break;
				}
				i++;
			}
			
			if (this.type == "3 ")
				this.type = "1";
			else if (this.type == "P ")
				this.type = "2";
			else if (this.type == "")
				this.type = "3";
			else if (this.type == "H ")
				this.type = "4";
			else if (this.type == "U ")
				this.type = "5";
			// Set Name
			this.name = pBuffer.Text.ToString();
		}

		public void setPCount(CString pInt)
		{

			if (pInt != null)
				int.TryParse(pInt.Text.ToString(), out playercount);
		}

		public void setPort(int pInt)
		{
			port = pInt;
		}

		public void setType(string pInt)
		{
			type = pInt;
		}

		public void setUrl(CString pBuffer)
		{
			url = pBuffer.Text.ToString();
		}

		public void setVersion(CString pBuffer)
		{
			version = pBuffer.Text.ToString();
		}

	}
}
