#ifndef _INC_LUAEVENTDISPATCHER_
#define _INC_LUAEVENTDISPATCHER_
//////////////////////////////////////////////////////////////////////////
#include <lua.hpp>
#include <list>
#include <map>
//////////////////////////////////////////////////////////////////////////
struct LuaDispatchEvent
{
	int nEventId;
	void* pEvent;

	LuaDispatchEvent()
	{
		nEventId = 0;
		pEvent = NULL;
	}
	LuaDispatchEvent(int _evtId, void* _pEvt)
	{
		nEventId = _evtId;
		pEvent = _pEvt;
	}
};

struct LuaEventHandler
{
	int nEvtId;
	int nHandlerId;

	LuaEventHandler()
	{
		nEvtId = nHandlerId = 0;
	}
};
typedef std::list<LuaEventHandler> LuaEventHandlerList;
typedef std::map<int, LuaEventHandlerList*> LuaEventHandlerListMap;

struct LuaDispatchInfo
{
	int nArgs;
	int nResults;

	LuaDispatchInfo()
	{
		nArgs = 0;
		nResults = 0;
	}
};

typedef std::list<int> WaitUnregisterHandlerList;
//////////////////////////////////////////////////////////////////////////
class LuaEventDispatcher
{
public:
	LuaEventDispatcher();
	virtual ~LuaEventDispatcher();

public:
	virtual int OnDispatchEvent(const LuaDispatchEvent* _pEvent, LuaDispatchInfo* _pInfo) {return 0;}
	virtual void OnDispatchEventResult(const LuaDispatchEvent* _pEvent, bool bResult) {};

public:
	void Bind(lua_State* L);
	void Unbind();

	inline lua_State* GetLuaState()	{return m_pLuaState;}
	inline bool IsDispatching()		{return m_bDispatching;}
	void PushWaitUnregisterHandler(int _nHandlerId)	{m_xWaitUnregisterList.push_back(_nHandlerId);}

public:
	//	helpers
	void RegisterHandler(int _nEvtId, int _nHandlerId);
	void UnregisterHandler(int _nHandlerId);
	void ClearHandlers();

	int DispatchEvent(const LuaDispatchEvent* _pEvent);
	int GetLuaFunction(int _nRefId);

	inline void PushNumber(lua_Number _nNumber)
	{
		lua_pushnumber(m_pLuaState, _nNumber);
	}
	inline void PushInt(int _n)
	{
		lua_pushinteger(m_pLuaState, _n);
	}
	inline void PushFloat(float _f)
	{
		lua_pushnumber(m_pLuaState, _f);
	}
	inline void PushString(const char* _pszStr)
	{
		lua_pushstring(m_pLuaState, _pszStr);
	}
	inline void PushString(const char* _pszStr, size_t _uLen)
	{
		lua_pushlstring(m_pLuaState, _pszStr, _uLen);
	}
	inline void PushBoolean(bool _b)
	{
		lua_pushboolean(m_pLuaState, _b ? 1 : 0);
	}
	inline void PushNil()
	{
		lua_pushnil(m_pLuaState);
	}

private:
	int __InternalDispatch(int _nHandlerId, const LuaDispatchEvent* _pEvent);

public:
	//	static
	static int luaRegisterHandler(lua_State* L);
	//static int luaUnregisterHandler(lua_State* L);
	static int luaUnregisterHandlerById(lua_State* L);

protected:
	lua_State* m_pLuaState;
	LuaEventHandlerListMap m_xHandlers;

	//	prevent unregister handler while dispatching
	bool m_bDispatching;
	WaitUnregisterHandlerList m_xWaitUnregisterList;
};
//////////////////////////////////////////////////////////////////////////
#endif