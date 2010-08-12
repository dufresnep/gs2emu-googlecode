#ifndef MISCFUNCTIONS_H
#define MISCFUNCTIONS_H

#include "TNPCServer.h"
#include "TPlayer.h"

class gmThread;

// Global functions.
int GM_CDECL func_sendtorc(gmThread* a_thread);
int GM_CDECL func_sendtonc(gmThread* a_thread);
int GM_CDECL func_sendtorc_echo(gmThread* a_thread);
int GM_CDECL func_findplayer(gmThread* a_thread);
int GM_CDECL func_getPlayerCount(gmThread* a_thread);
int GM_CDECL func_sendPM(gmThread* a_thread);
int GM_CDECL func_sendRPGMessage(gmThread* a_thread);
int GM_CDECL func_getPlayers(gmThread* a_thread);

// Useful functions.
int GM_CDECL func_tokenize(gmThread* a_thread);
int GM_CDECL func_uppercase(gmThread* a_thread);
int GM_CDECL func_lowercase(gmThread* a_thread);
int GM_CDECL func_md5(gmThread* a_thread);

#endif
