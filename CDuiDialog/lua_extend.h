#ifndef _INC_LUAEXTEND_
#define _INC_LUAEXTEND_
//////////////////////////////////////////////////////////////////////////
#include <lua.hpp>
//////////////////////////////////////////////////////////////////////////
//	all func_index need positive number index
void luaext_open(lua_State* L);
//	if success, return function ref id, else return 0
int luaext_registerLuaFunction(lua_State* L, int _nFuncIndex);
//	if success, return function ref id, else return 0
void luaext_unregisterLuaFunctionByRefId(lua_State* L, int _nRefId);
//	if success, return removed function count, else return 0
int luaext_unregisterLuaFunction(lua_State* L, int _nFuncIndex);
//	if success, return ref id , else return 0
int luaext_getLuaFunctionByRefId(lua_State* L, int _nRefId);
//////////////////////////////////////////////////////////////////////////
#endif