#include "lpquelib.h"
#include <stdexcept>
#include <lua.hpp>
#include "pqueue.h"

#define PQUEUE_LUAT "pqueue"
#define checkpqueue(L, index) *(struct pqueue<int> **)luaL_checkudata(L, index, PQUEUE_LUAT)

static int pqueue_new(lua_State *L)
{
	size_t size;

	if (lua_gettop(L) != 0)
	{
		size = luaL_checkinteger(L, 1);
		luaL_argcheck(L, size > 0, 1, "invalid size");
	}
	else
	{
		size = 32;
	}

	struct pqueue<int> **queue = (struct pqueue<int> **)lua_newuserdata(L, sizeof(struct pqueue<int>*));
	*queue = pq_create<int>(size);

	luaL_getmetatable(L, PQUEUE_LUAT);
	lua_setmetatable(L, -2);
	return 1;
}

static int pqueue_gc(lua_State *L)
{
	struct pqueue<int> *queue = checkpqueue(L, 1);
	pq_destroy(queue);
	return 0;
}

static int pqueue_enqueue(lua_State *L)
{
	struct pqueue<int> *queue = checkpqueue(L, 1);
	int priority = luaL_checkinteger(L, 2);
	luaL_checkany(L, 3);
	lua_settop(L, 3);
	int ref = luaL_ref(L, LUA_REGISTRYINDEX);
	pq_enqueue(queue, priority, ref);
	return 0;
}

static int pqueue_update(lua_State *L)
{
	luaL_error(L, "not implemented yet");
	return 0;
}

static int pqueue_dequeue(lua_State *L)
{
	struct pqueue<int> *queue = checkpqueue(L, 1);

	try
	{
		int priority;
		int ref = pq_dequeue(queue, &priority);
		lua_pushinteger(L, priority);
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
		luaL_unref(L, LUA_REGISTRYINDEX, ref);
		return 2;
	}
	catch (const std::exception& e)
	{
		lua_pushnil(L);
		return 1;
	}
}

static int pqueue_peek(lua_State *L)
{
	struct pqueue<int> *queue = checkpqueue(L, 1);

	try
	{
		int priority;
		int ref = pq_peek(queue, &priority);
		lua_pushinteger(L, priority);
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
		return 2;
	}
	catch (const std::exception& e)
	{
		lua_pushnil(L);
		return 1;
	}
}

static int pqueue_purge(lua_State *L)
{
	struct pqueue<int> *queue = checkpqueue(L, 1);
	pq_purge(queue);
	return 0;
}

static int pqueue_count(lua_State *L)
{
	struct pqueue<int> *queue = checkpqueue(L, 1);
	lua_pushinteger(L, pq_count(queue));
	return 1;
}

static int pqueue_pairs(lua_State *L)
{
	lua_pushcfunction(L, pqueue_dequeue);
	lua_insert(L, 1);
	return 2;
}

static void buffadd(lua_State *L, struct pqueue<int> *queue, size_t index, luaL_Buffer *buf)
{
	int priority;
	int ref = pq_get(queue, index, &priority);
	lua_pushinteger(L, priority);
	luaL_addvalue(buf);
	luaL_addstring(buf, ": ");
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	luaL_tolstring(L, -1, NULL);
	luaL_addvalue(buf);
	lua_pop(L, 1);
}

static int pqueue_tostring(lua_State *L)
{
	struct pqueue<int> *queue = checkpqueue(L, 1);
	luaL_Buffer buf;
	luaL_buffinit(L, &buf);
	luaL_addstring(&buf, PQUEUE_LUAT);
	luaL_addstring(&buf, " {");

	if (pq_count(queue) > 0)
	{
		buffadd(L, queue, 0, &buf);
		
		for (size_t i = 1; i < queue->count; i++)
		{
			luaL_addstring(&buf, ", ");
			buffadd(L, queue, i, &buf);
		}
	}

	luaL_addchar(&buf, '}');
	luaL_pushresult(&buf);
	return 1;
}

static const struct luaL_Reg pqueue_f [] = {
	{"new", pqueue_new},
	{NULL, NULL}
};

static const struct luaL_Reg pqueue_m [] = {
	{"__tostring", pqueue_tostring},
	{"__gc", pqueue_gc},
	{"__len", pqueue_count},
	{"enqueue", pqueue_enqueue},
	{"update", pqueue_update},
	{"dequeue", pqueue_dequeue},
	{"peek", pqueue_peek},
	{"purge", pqueue_purge},
	{"pairs", pqueue_pairs},
	{NULL, NULL}
};

int luaopen_pqueue(lua_State *L)
{
	luaL_newmetatable(L, PQUEUE_LUAT);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, pqueue_m, 0);
  luaL_newlib(L, pqueue_f);
	return 1;
}
