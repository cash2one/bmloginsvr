#ifndef _INC_LUAEXTEND_
#define _INC_LUAEXTEND_
//////////////////////////////////////////////////////////////////////////
#include <tolua++.h>
//////////////////////////////////////////////////////////////////////////
void luaext_open(lua_State* L);
//	if success, return function ref id, else return 0
int luaext_registerLuaFunction(lua_State* L, int _nFuncIndex);
//	if success, return function ref id, else return 0
void luaext_unregisterLuaFunctionByRefId(lua_State* L, int _nRefId);
//
int luaext_callLuaFunctionByRefId(lua_State* L, int _nRefId, int _nArgs);
//////////////////////////////////////////////////////////////////////////
#endif