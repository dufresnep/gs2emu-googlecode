#include "main.h"
#include "CFileSystem.h"
#include "TLevel.h"


const CString signText = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
				"0123456789!?-.,#>()#####\"####':/~&### <####;\n";
const CString signSymbols = "ABXYudlrhxyz#4.";
const int ctablen[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1};
const int ctabindex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 15};
const int ctab[] = {91, 92, 93, 94, 77, 78, 79, 80, 74, 75, 71, 72, 73, 87, 88, 67};

static CString encodeSignCode(CString& pText);
static CString decodeSignCode(CString pText);


bool TLevel::loadLevel(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels)
{
	CString ext(getExtension(pLevelName));
	if (ext == ".nw") return loadNW(pLevelName, images, levels);
	else if (ext == ".graal") return loadGraal(pLevelName, images, levels);
	else if (ext == ".zelda") return loadZelda(pLevelName, images, levels);
	return false;
}

bool TLevel::loadZelda(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels)
{
	// Load file
	CString fileData;
	if (fileData.load(pLevelName) == false) return false;

	// Grab file version.
	CString fileVersion = fileData.readChars(8);

	// Check if it is actually a .graal level.  The 1.39-1.41r1 client actually
	// saved .zelda as .graal.
	if (fileVersion.subString(0, 2) == "GR")
		return loadGraal(pLevelName, images, levels);

	int v = 0;
	if (fileVersion == "Z3-V1.03") v = 3;
	else if (fileVersion == "Z3-V1.04") v = 4;

	// Load tiles.
	{
		int bits = (v > 4 ? 13 : 12);
		int read = 0;
		unsigned int buffer = 0;
		unsigned short code = 0;
		short tiles[2] = {-1,-1};
		int boardIndex = 0;
		int count = 1;
		bool doubleMode = false;

		// Read the tiles.
		while (boardIndex < 64*64 && fileData.bytesLeft() != 0)
		{
			// Every control code/tile is either 12 or 13 bits.  WTF.
			// Read in the bits.
			while (read < bits)
			{
				buffer += ((unsigned char)fileData.readChar()) << read;
				read += 8;
			}

			// Pull out a single 12/13 bit code from the buffer.
			code = buffer & (bits == 12 ? 0xFFF : 0x1FFF);
			buffer >>= bits;
			read -= bits;

			// See if we have an RLE control code.
			// Control codes determine how the RLE scheme works.
			if (code & (bits == 12 ? 0x800 : 0x1000))
			{
				// If the 0x100 bit is set, we are in a double repeat mode.
				// {double 4}56 = 56565656
				if (code & 0x100) doubleMode = true;

				// How many tiles do we count?
				count = code & 0xFF;
				continue;
			}

			// If our count is 1, just read in a tile.  This is the default mode.
			if (count == 1)
			{
				boardIndex++;
				//levelTiles[boardIndex++] = (short)code;
				continue;
			}

			// If we reach here, we have an RLE scheme.
			// See if we are in double repeat mode or not.
			if (doubleMode)
			{
				// Read in our first tile.
				if (tiles[0] == -1)
				{
					tiles[0] = (short)code;
					continue;
				}

				// Read in our second tile.
				tiles[1] = (short)code;

				// Add the tiles now.
				for (int i = 0; i < count && boardIndex < 64*64-1; ++i)
				{
					boardIndex += 2;
					//levelTiles[boardIndex++] = tiles[0];
					//levelTiles[boardIndex++] = tiles[1];
				}

				// Clean up.
				tiles[0] = tiles[1] = -1;
				doubleMode = false;
				count = 1;
			}
			// Regular RLE scheme.
			else
			{
				for (int i = 0; i < count && boardIndex < 64*64; ++i)
					boardIndex++;
					//levelTiles[boardIndex++] = (short)code;
				count = 1;
			}
		}
	}

	// Load the links.
	{
		while (fileData.bytesLeft())
		{
			CString line = fileData.readString("\n");
			if (line.length() == 0 || line == "#") break;

			// Assemble the level string.
			std::vector<CString> vline = line.tokenize();
			CString level = vline[0];
			if (vline.size() > 7)
			{
				for (unsigned int i = 0; i < vline.size() - 7; ++i)
					level << " " << vline[1 + i];
			}

			// Add the level.
			levels[level] = 1;
		}
	}

	// Load the baddies.
	{
		while (fileData.bytesLeft())
		{
			char x = fileData.readChar();
			char y = fileData.readChar();
			char type = fileData.readChar();

			// Ends with an invalid baddy.
			if (x == -1 && y == -1 && type == -1)
			{
				fileData.readString("\n");	// Empty verses.
				break;
			}

			// Only v1.04+ baddies have verses.
			if (v > 3)
			{
				// Load the verses.
				std::vector<CString> bverse = fileData.readString("\n").tokenize("\\");
			}
		}
	}

	// Load signs.
	{
		while (fileData.bytesLeft())
		{
			CString line = fileData.readString("\n");
			if (line.length() == 0) break;

			char x = line.readGChar();
			char y = line.readGChar();
			CString text = line.readString("");
			decodeSignCode(text);

			int pos = 0;
			while ((pos = text.find("#i(", pos)) != -1)
			{
				CString image = text.subString(pos + 3);
				image.remove(image.find(")"));
				images[image] = 1;
				pos++;
			}
		}
	}

	return true;
}

bool TLevel::loadGraal(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels)
{
	// Load file
	CString fileData;
	if (fileData.load(pLevelName) == false) return false;

	// Grab file version.
	CString fileVersion = fileData.readChars(8);
	int v = -1;
	if (fileVersion == "GR-V1.00") v = 0;
	else if (fileVersion == "GR-V1.01") v = 1;
	else if (fileVersion == "GR-V1.02") v = 2;
	else if (fileVersion == "GR-V1.03") v = 3;
	if (v == -1)
		return false;

	// Load tiles.
	{
		int bits = (v > 0 ? 13 : 12);
		int read = 0;
		unsigned int buffer = 0;
		unsigned short code = 0;
		short tiles[2] = {-1,-1};
		int boardIndex = 0;
		int count = 1;
		bool doubleMode = false;

		// Read the tiles.
		while (boardIndex < 64*64 && fileData.bytesLeft() != 0)
		{
			// Every control code/tile is either 12 or 13 bits.  WTF.
			// Read in the bits.
			while (read < bits)
			{
				buffer += ((unsigned char)fileData.readChar()) << read;
				read += 8;
			}

			// Pull out a single 12/13 bit code from the buffer.
			code = buffer & (bits == 12 ? 0xFFF : 0x1FFF);
			buffer >>= bits;
			read -= bits;

			// See if we have an RLE control code.
			// Control codes determine how the RLE scheme works.
			if (code & (bits == 12 ? 0x800 : 0x1000))
			{
				// If the 0x100 bit is set, we are in a double repeat mode.
				// {double 4}56 = 56565656
				if (code & 0x100) doubleMode = true;

				// How many tiles do we count?
				count = code & 0xFF;
				continue;
			}

			// If our count is 1, just read in a tile.  This is the default mode.
			if (count == 1)
			{
				boardIndex++;
				//levelTiles[boardIndex++] = (short)code;
				continue;
			}

			// If we reach here, we have an RLE scheme.
			// See if we are in double repeat mode or not.
			if (doubleMode)
			{
				// Read in our first tile.
				if (tiles[0] == -1)
				{
					tiles[0] = (short)code;
					continue;
				}

				// Read in our second tile.
				tiles[1] = (short)code;

				// Add the tiles now.
				for (int i = 0; i < count && boardIndex < 64*64-1; ++i)
				{
					boardIndex+=2;
				//	levelTiles[boardIndex++] = tiles[0];
				//	levelTiles[boardIndex++] = tiles[1];
				}

				// Clean up.
				tiles[0] = tiles[1] = -1;
				doubleMode = false;
				count = 1;
			}
			// Regular RLE scheme.
			else
			{
				for (int i = 0; i < count && boardIndex < 64*64; ++i)
					boardIndex++;
				//	levelTiles[boardIndex++] = (short)code;
				count = 1;
			}
		}
	}

	// Load the links.
	{
		while (fileData.bytesLeft())
		{
			CString line = fileData.readString("\n");
			if (line.length() == 0 || line == "#") break;

			// Assemble the level string.
			std::vector<CString> vline = line.tokenize();
			CString level = vline[0];
			if (vline.size() > 7)
			{
				for (unsigned int i = 0; i < vline.size() - 7; ++i)
					level << " " << vline[1 + i];
			}

			// Add the level.
			levels[level] = 1;
		}
	}

	// Load the baddies.
	{
		while (fileData.bytesLeft())
		{
			char x = fileData.readChar();
			char y = fileData.readChar();
			char type = fileData.readChar();

			// Ends with an invalid baddy.
			if (x == -1 && y == -1 && type == -1)
			{
				fileData.readString("\n");	// Empty verses.
				break;
			}

			// Load the verses.
			std::vector<CString> bverse = fileData.readString("\n").tokenize("\\");
		}
	}

	// Load NPCs.
	{
		while (fileData.bytesLeft())
		{
			CString line = fileData.readString("\n");
			if (line.length() == 0 || line == "#") break;

			char x = line.readGChar();
			char y = line.readGChar();
			CString image = line.readString("#");
			CString code = line.readString("");

			images[image] = 1;

			// Search for all .nw
			CString codeNW = code.replaceAll(",", " ");
			if (codeNW.find(".nw") != -1)
			{
				while (codeNW.bytesLeft())
				{
					CString c = codeNW.readString(".nw");
					if (!c.isEmpty() && c.find(".nw") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".nw";
						image.trimI();
						levels[image] = 1;
					}
				}
			}

			// Search for all .graal
			CString codeGRAAL = code.replaceAll(",", " ");
			if (codeGRAAL.find(".graal") != -1)
			{
				while (codeGRAAL.bytesLeft())
				{
					CString c = codeGRAAL.readString(".graal");
					if (!c.isEmpty() && c.find(".graal") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".graal";
						image.trimI();
						levels[image] = 1;
					}
				}
			}

			// Search for all .zelda
			CString codeZELDA = code.replaceAll(",", " ");
			if (codeZELDA.find(".zelda") != -1)
			{
				while (codeZELDA.bytesLeft())
				{
					CString c = codeZELDA.readString(".zelda");
					if (!c.isEmpty() && c.find(".zelda") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".zelda";
						image.trimI();
						levels[image] = 1;
					}
				}
			}

			// Search for all .png
			CString codePNG = code.replaceAll(",", " ");
			if (codePNG.find(".png") != -1)
			{
				while (codePNG.bytesLeft())
				{
					CString c = codePNG.readString(".png");
					if (!c.isEmpty() && c.find(".png") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".png";
						image.trimI();
						images[image] = 1;
					}
				}
			}

			// Search for all .mng
			CString codeMNG = code.replaceAll(",", " ");
			if (codeMNG.find(".mng") != -1)
			{
				while (codeMNG.bytesLeft())
				{
					CString c = codeMNG.readString(".mng");
					if (!c.isEmpty() && c.find(".mng") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".mng";
						image.trimI();
						images[image] = 1;
					}
				}
			}

			// Search for all .gif
			CString codeGIF = code.replaceAll(",", " ");
			if (codeGIF.find(".gif") != -1)
			{
				while (codeGIF.bytesLeft())
				{
					CString c = codeGIF.readString(".gif");
					if (!c.isEmpty() && c.find(".gif") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".gif";
						image.trimI();
						images[image] = 1;
					}
				}
			}
		}
	}

	// Load chests.
	if (v > 0)
	{
		while (fileData.bytesLeft())
		{
			CString line = fileData.readString("\n");
			if (line.length() == 0 || line == "#") break;

			char x = line.readGChar();
			char y = line.readGChar();
			char item = line.readGChar();
			char signindex = line.readGChar();
		}
	}

	// Load signs.
	{
		while (fileData.bytesLeft())
		{
			CString line = fileData.readString("\n");
			if (line.length() == 0) break;

			char x = line.readGChar();
			char y = line.readGChar();
			CString text = line.readString("");
			decodeSignCode(text);

			int pos = 0;
			while ((pos = text.find("#i(", pos)) != -1)
			{
				CString image = text.subString(pos + 3);
				image.remove(image.find(")"));
				images[image] = 1;
				pos++;
			}
		}
	}

	return true;
}

bool TLevel::loadNW(const CString& pLevelName, std::map<CString, int>& images, std::map<CString, int>& levels)
{
	// Load File
	std::vector<CString> fileData = CString::loadToken(pLevelName, "\n", true);
	if (fileData.size() == 0)
		return false;

	// Parse Level
	for (std::vector<CString>::iterator i = fileData.begin(); i != fileData.end(); ++i)
	{
		// Tokenize
		std::vector<CString> curLine = i->tokenize();
		if (curLine.size() < 1)
			continue;

		// Parse Each Type
		if (curLine[0] == "BOARD")
		{
			continue;
		}
		else if (curLine[0] == "CHEST")
		{
			continue;
		}
		else if (curLine[0] == "LINK")
		{
			if (curLine.size() < 8)
				continue;

			// Get link string.
			std::vector<CString>::iterator i = curLine.begin();
			std::vector<CString> link(++i, curLine.end());

			// Find the whole level name.
			CString level(link[0]);
			if (link.size() > 7)
			{
				for (unsigned int i = 0; i < link.size() - 7; ++i)
					level << " " << link[1 + i];
			}

			// Add the level.
			levels[level] = 1;
		}
		else if (curLine[0] == "NPC")
		{
			unsigned int offset = 0;
			if (curLine.size() < 4)
				continue;

			// Grab the image properties.
			CString image(curLine[1]);
			if (curLine.size() > 4)
			{
				offset = curLine.size() - 4;
				for (unsigned int i = 0; i < offset; ++i)
					image << " " << curLine[2 + i];
			}

			images[image] = 1;

			// Grab the NPC location.
			float x = (float)strtofloat(curLine[2 + offset]);
			float y = (float)strtofloat(curLine[3 + offset]);

			// Grab the NPC code.
			CString code;
			++i;
			while (i != fileData.end())
			{
				if (*i == "NPCEND") break;
				code << *i << "\xa7";
				++i;
			}

			// Search for all .nw
			CString codeNW = code.replaceAll(",", " ");
			if (codeNW.find(".nw") != -1)
			{
				while (codeNW.bytesLeft())
				{
					CString c = codeNW.readString(".nw");
					if (!c.isEmpty() && c.find(".nw") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".nw";
						image.trimI();
						levels[image] = 1;
					}
				}
			}

			// Search for all .graal
			CString codeGRAAL = code.replaceAll(",", " ");
			if (codeGRAAL.find(".graal") != -1)
			{
				while (codeGRAAL.bytesLeft())
				{
					CString c = codeGRAAL.readString(".graal");
					if (!c.isEmpty() && c.find(".graal") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".graal";
						image.trimI();
						levels[image] = 1;
					}
				}
			}

			// Search for all .zelda
			CString codeZELDA = code.replaceAll(",", " ");
			if (codeZELDA.find(".zelda") != -1)
			{
				while (codeZELDA.bytesLeft())
				{
					CString c = codeZELDA.readString(".zelda");
					if (!c.isEmpty() && c.find(".zelda") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".zelda";
						image.trimI();
						levels[image] = 1;
					}
				}
			}

			// Search for all .png
			CString codePNG = code.replaceAll(",", " ");
			if (codePNG.find(".png") != -1)
			{
				while (codePNG.bytesLeft())
				{
					CString c = codePNG.readString(".png");
					if (!c.isEmpty() && c.find(".png") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".png";
						image.trimI();
						images[image] = 1;
					}
				}
			}

			// Search for all .mng
			CString codeMNG = code.replaceAll(",", " ");
			if (codeMNG.find(".mng") != -1)
			{
				while (codeMNG.bytesLeft())
				{
					CString c = codeMNG.readString(".mng");
					if (!c.isEmpty() && c.find(".mng") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".mng";
						image.trimI();
						images[image] = 1;
					}
				}
			}

			// Search for all .gif
			CString codeGIF = code.replaceAll(",", " ");
			if (codeGIF.find(".gif") != -1)
			{
				while (codeGIF.bytesLeft())
				{
					CString c = codeGIF.readString(".gif");
					if (!c.isEmpty() && c.find(".gif") != -1)
					{
						CString image = c.subString(c.findl(' '));
						image << ".gif";
						image.trimI();
						images[image] = 1;
					}
				}
			}
		}
		else if (curLine[0] == "SIGN")
		{
			if (curLine.size() != 3)
				continue;

			int x = strtoint(curLine[1]);
			int y = strtoint(curLine[2]);

			// Grab the sign text.
			CString text;
			++i;
			while (i != fileData.end())
			{
				if (*i == "SIGNEND") break;
				text << *i << "\n";
				++i;
			}

			//decodeSignCode(text);

			int pos = 0;
			while ((pos = text.find("#i(", pos)) != -1)
			{
				CString image = text.subString(pos + 3);
				image.remove(image.find(")"));
				images[image] = 1;
				pos++;
			}
		}
		else if (curLine[0] == "BADDY")
		{
			if (curLine.size() != 4)
				continue;

			int x = strtoint(curLine[1]);
			int y = strtoint(curLine[2]);
			int type = strtoint(curLine[3]);

			// Load the verses.
			std::vector<CString> bverse;
			++i;
			while (i != fileData.end())
			{
				if (*i == "BADDYEND") break;
				bverse.push_back(*i);
				++i;
			}
		}
		if (i == fileData.end()) break;
	}

	return true;
}


CString encodeSignCode(CString& pText)
{
	CString retVal;
	int txtLen = pText.length();
	for (int i = 0; i < txtLen; i++)
	{
		char letter = pText[i];
		if (letter == '#')
		{
			i++;
			if (i < txtLen)
			{
				letter = pText[i];
				int code = signSymbols.find(letter);
				if (code != -1)
				{
					for (int ii = 0; ii < ctablen[code]; ii++)
						retVal.writeGChar((char)ctab[ctabindex[code] + ii]);
					continue;
				}
				else letter = pText[--i];
			}
		}

		int code = signText.find(letter);
		if (letter == '#') code = 86;
		if (code != -1)
			retVal.writeGChar((char)code);
		else
		{
			// Write the character code directly into the sign.
			retVal >> (char)86 >> (char)10 >> (char)69;		// #K(
			CString scode((int)code);
			for (int i = 0; i < scode.length(); ++i)
			{
				int c = signText.find(scode[i]);
				if (scode != -1) retVal.writeGChar((char)c);
			}
			retVal >> (char)70;								// )
		}
	}
	return retVal;
}

CString decodeSignCode(CString pText)
{
	CString retVal;
	int txtLen = pText.length();
	for (int i = 0; i < txtLen; i++)
	{
		unsigned char letter = pText.readGUChar();
		bool isCode = false;
		int codeID = -1;
		for (int j = 0; j < 16; ++j)	// ctab length
		{
			if (letter == ctab[j])
			{
				codeID = j;
				isCode = true;
				break;
			}
		}

		if (isCode)
		{
			int codeIndex = -1;
			for (int j = 0; j < 14; ++j)	// ctabindex
			{
				if (ctabindex[j] == codeID)
				{
					codeIndex = j;
					break;
				}
			}
			if (codeIndex != -1)
				retVal << "#" << signSymbols[codeIndex];
		}
		else
			retVal << signText[letter];
	}
	return retVal;
}
