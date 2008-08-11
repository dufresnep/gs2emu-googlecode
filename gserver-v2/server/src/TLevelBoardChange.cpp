#include "TLevelBoardChange.h"
#include "TPlayer.h"

CString TLevelBoardChange::getBoardStr(const CString ignore) const
{
	return CString() >> (char)PLO_BOARDMODIFY >> (char)x >> (char)y >> (char)width >> (char)height << tiles;
}

void TLevelBoardChange::swapTiles()
{
	CString temp = tiles;
	tiles = oldTiles;
	oldTiles = temp;
}