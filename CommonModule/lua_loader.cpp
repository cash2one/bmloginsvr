#include "lua_loader.h"
#include <string.h>
//////////////////////////////////////////////////////////////////////////
static int readable (const char *filename) {
	FILE *f = fopen(filename, "r");  /* try to open file */
	if (f == NULL) return 0;  /* open failed */
	fclose(f);
	return 1;
}


static const char *pushnexttemplate (lua_State *L, const char *path) {
	const char *l;
	while (*path == *LUA_PATHSEP) path++;  /* skip separators */
	if (*path == '\0') return NULL;  /* no more templates */
	l = strchr(path, *LUA_PATHSEP);  /* find next separator */
	if (l == NULL) l = path + strlen(path);
	lua_pushlstring(L, path, l - path);  /* template */
	return l;
}

static void loaderror (lua_State *L, const char *filename) {
	luaL_error(L, "error loading module " LUA_QS " from file " LUA_QS ":\n\t%s",
		lua_tostring(L, 1), filename, lua_tostring(L, -1));
}

static const char *findfile (lua_State *L, const char *name,
							 const char *pname) {
								 const char *path;
								 name = luaL_gsub(L, name, ".", LUA_DIRSEP);
								 lua_getfield(L, LUA_ENVIRONINDEX, pname);
								 path = lua_tostring(L, -1);
								 if (path == NULL)
									 luaL_error(L, LUA_QL("package.%s") " must be a string", pname);
								 lua_pushliteral(L, "");  /* error accumulator */
								 while ((path = pushnexttemplate(L, path)) != NULL) {
									 const char *filename;
									 filename = luaL_gsub(L, lua_tostring(L, -1), LUA_PATH_MARK, name);
									 lua_remove(L, -2);  /* remove path template */
									 if (readable(filename))  /* does file exist and is readable? */
										 return filename;  /* return that file name */
									 lua_pushfstring(L, "\n\tno file " LUA_QS, filename);
									 lua_remove(L, -2);  /* remove file name */
									 lua_concat(L, 2);  /* add entry to possible error message */
								 }
								 return NULL;  /* not found */
}
//////////////////////////////////////////////////////////////////////////
bool RegisterBMLoader(lua_State* L)
{
	lua_getglobal(L,"package");  
	lua_getfield(L,-1,"loaders");//package.loaders  
	lua_pushnumber(L, 2);  
	lua_gettable(L, -2);//package.loaders[2]  
	lua_pushnumber(L,2);//先把索引压栈，呆会用来设置C函数到这个索引位置  
	lua_pushcfunction(L,BMLuaLoader);//压入自己的C函数  
	lua_getfenv(L,-3);//获取旧函数的环境表  
	int ret=lua_setfenv(L,-2);//设置到新函数  
	lua_settable(L,-4);//替换旧函数  
	lua_pop(L,3);//清理堆栈  

	return true;
}

int BMLuaLoader(lua_State* L)
{
	const char *filename;
	const char *name = luaL_checkstring(L, 1);
	filename = findfile(L, name, "path");
	if (filename == NULL)
	{
		//	try to read from buffer
		if(0 == strcmp(name, "script/read"))
		{
			luaL_loadstring(L, "local a = {} a.b = 3 print(\"required, result:\"..a.b) function read() print(\"func read\") end return a");
		}
		
		 return 1;  /* library not found in this path */
	}
	if (luaL_loadfile(L, filename) != 0)
		loaderror(L, filename);
	return 1;  /* library loaded successfully */
}