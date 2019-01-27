#ifndef lpquelib_h
#define lpquelib_h

#include <lua.hpp>

#define PQUEUE_LIBNAME "pqueue"

extern "C" int luaopen_pqueue(lua_State *L);

#endif
