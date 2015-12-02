#include "lua_extend.h"
//////////////////////////////////////////////////////////////////////////
//	static light userdata key
static int s_nLuaFunctionRefKey = 0;
void* GetLuaFunctionRefKey()
{
	return (void*)&s_nLuaFunctionRefKey;
}

//	static lua function reference id
static int s_nLuaFunctionRefId = 0;
int GetLuaFunctionRefId()
{
	return ++s_nLuaFunctionRefId;
}

//////////////////////////////////////////////////////////////////////////
void luaext_open(lua_State* L)
{
	//	push key
	lua_pushlightuserdata(L, GetLuaFunctionRefKey());	//	stack: key
	//	push value
	lua_newtable(L);	//	stack: key table
	//	set global table key
	lua_rawset(L, LUA_REGISTRYINDEX);	//	stack:
}

int luaext_registerLuaFunction(lua_State* L, int _nFuncIndex)
{
	if(!lua_isfunction(L, _nFuncIndex))
	{
		return 0;
	}

	//	push key
	lua_pushlightuserdata(L, GetLuaFunctionRefKey());	//	stack: func ... key
	//	get ref table
	lua_rawget(L, LUA_REGISTRYINDEX);		//	stack: func ... reftable

	if (!lua_istable(L, -1))
	{
		return 0;
	}

	//	push new key
	int nRefId = GetLuaFunctionRefId();
	lua_pushnumber(L, nRefId);			//	stack: func ... reftable key
	//	copy function value from stack
	lua_pushvalue(L, _nFuncIndex);		//	stack: func ... reftable key value
	//	set key
	lua_rawset(L, -3);	//	stack: func ... reftable
	//	pop table
	lua_pop(L, 1);

	return nRefId;
}

void luaext_unregisterLuaFunctionByRefId(lua_State* L, int _nRefId)
{
	//	push key
	lua_pushlightuserdata(L, GetLuaFunctionRefKey());	//	stack: func ... key
	//	get ref table
	lua_rawget(L, LUA_REGISTRYINDEX);		//	stack: func ... reftable

	if (!lua_istable(L, -1))
	{
		return;
	}

	//	push the key
	lua_pushnumber(L, _nRefId);	//	stack: reftable key
	//	push nil
	lua_pushnil(L);			//	stack: reftable key nil
	//	set nil
	lua_rawset(L, -3);
	//	pop reftable
	lua_pop(L, 1);
}

int luaext_callLuaFunctionByRefId(lua_State* L, int _nRefId, int _nArgs)
{
	//	push key
	lua_pushlightuserdata(L, GetLuaFunctionRefKey());	//	stack: func ... key
	//	get ref table
	lua_rawget(L, LUA_REGISTRYINDEX);		//	stack: func ... reftable

	if (!lua_istable(L, -1))
	{
		return 0;
	}
}