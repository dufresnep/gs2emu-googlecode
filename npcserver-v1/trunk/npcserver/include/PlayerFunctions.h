#include "gmThread.h"
#include "TNPCServer.h"
#include "TPlayer.h"

// Get Dot operator for table access
void GM_CDECL TPlayerGetDot(gmThread * a_thread, gmVariable * a_operands);
void GM_CDECL TPlayerSetDot(gmThread * a_thread, gmVariable * a_operands);

//Player functions
int GM_CDECL func_addweapon(gmThread * a_thread);
int GM_CDECL func_removeweapon(gmThread * a_thread);
int GM_CDECL func_setlevel2(gmThread* a_thread);