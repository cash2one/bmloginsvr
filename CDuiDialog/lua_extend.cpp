#include "lua_extend.h"
#include <assert.h>
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
#ifdef _DEBUG
	int nStackTop = lua_gettop(L);
#endif
	//	push key
	lua_pushlightuserdata(L, GetLuaFunctionRefKey());	//	stack: key
	//	push value
	lua_newtable(L);	//	stack: key table
	//	set global table key
	lua_rawset(L, LUA_REGISTRYINDEX);	//	stack:
#ifdef _DEBUG
	assert(nStackTop == lua_gettop(L));
#endif
}

int luaext_registerLuaFunction(lua_State* L, int _nFuncIndex)
{
	if(_nFuncIndex < 0)
	{
		return 0;
	}
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
#ifdef _DEBUG
	int nStackTop = lua_gettop(L);
#endif
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

#ifdef _DEBUG
	assert(lua_gettop(L) == nStackTop);
#endif
}

int luaext_unregisterLuaFunction(lua_State* L, int _nFuncIndex)
{
	if(_nFuncIndex < 0)
	{
		return 0;
	}
	if(!lua_isfunction(L, _nFuncIndex))
	{
		return 0;
	}

	int nCount = 0;

	//	push key
	lua_pushlightuserdata(L, GetLuaFunctionRefKey());	//	stack: key
	//	get ref table
	lua_rawget(L, LUA_REGISTRYINDEX);		//	stack: reftable

	if (!lua_istable(L, -1))
	{
		return 0;
	}

	//	push reftable index
	int nStackTop = lua_gettop(L);

	//	push init key
	lua_pushnil(L);		//	stack: reftable nil_key

	while(lua_next(L, nStackTop))
	{
		//	stack: reftable key value
		if(lua_isfunction(L, -1))
		{
			//	valid function
			if(lua_equal(L, _nFuncIndex, -1))
			{
#ifdef _DEBUG
				int nTop = lua_gettop(L);
#endif
				//	set refFunc[key] = nil
				//	push key
				lua_pushvalue(L, -2);
				//	push nil
				lua_pushnil(L);
				//	stack: reftable key value key nil
				lua_rawset(L, -5);	//	stack: reftable key value

#ifdef _DEBUG
				assert(lua_gettop(L) == nTop);
#endif
				++nCount;
			}
		}

		//	pop the value, and use the key to get the next key
		lua_pop(L, 1);	//	stack: reftable key 
	}

#ifdef _DEBUG
	assert(lua_gettop(L) == nStackTop);
#endif

	return nCount;
}

int luaext_getLuaFunctionByRefId(lua_State* L, int _nRefId)
{
#ifdef _DEBUG
	int nStackTop = lua_gettop(L);
#endif

	//	push key
	lua_pushlightuserdata(L, GetLuaFunctionRefKey());	//	stack: key
	//	get ref table
	lua_rawget(L, LUA_REGISTRYINDEX);		//	stack: reftable

	if (!lua_istable(L, -1))
	{
		return 0;
	}

	//	push key
	lua_pushnumber(L, _nRefId);	//	stack: reftable key
	//	get value
	lua_rawget(L, -2);		//	stack: reftable (value or nil)

	int nRetId = 0;

	if(lua_isfunction(L, -1))
	{
		nRetId = _nRefId;

		//	stack: reftable value
		lua_remove(L, -2);	//	stack: value(func)

#ifdef _DEBUG
		assert(lua_gettop(L) == nStackTop + 1);
#endif
	}
	else
	{
		//	pop reftable and nil
		lua_pop(L, 2);
#ifdef _DEBUG
		assert(lua_gettop(L) == nStackTop);
#endif
	}

	return nRetId;
}