#ifndef NPCFUNCTIONS_H
#define NPCFUNCTIONS_H

#include "TNPCServer.h"
#include "TPlayer.h"
#include "TNPC.h"

class gmThread;
struct gmVariable;

int GM_CDECL func_message(gmThread* a_thread);
int GM_CDECL func_setTimer(gmThread* a_thread);
int GM_CDECL func_setshape(gmThread* a_thread);

void GM_CDECL TNPCGetDot(gmThread* a_thread, gmVariable* a_operands);
void GM_CDECL TNPCSetDot(gmThread* a_thread, gmVariable* a_operands);

#endif
