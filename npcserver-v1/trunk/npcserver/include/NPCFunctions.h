#include "gmThread.h"
#include "TNPCServer.h"
#include "TPlayer.h"
#include "TNPC.h"

int GM_CDECL func_message(gmThread * a_thread);
int GM_CDECL func_setTimer(gmThread * a_thread);
int GM_CDECL func_setshape(gmThread * a_thread);

void GM_CDECL TNPCGetDot(gmThread * a_thread, gmVariable * a_operands);
void GM_CDECL TNPCSetDot(gmThread * a_thread, gmVariable * a_operands);
