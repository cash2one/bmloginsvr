#include "LuaEventDispatcher.h"
#include "lua_extend.h"
#include <map>
#include <assert.h>
//////////////////////////////////////////////////////////////////////////
typedef std::map<void*, void*> LuaEventDispatcherChain;
//////////////////////////////////////////////////////////////////////////
//	LuaEventDispatcher chain
LuaEventDispatcherChain g_xLuaEventDispatcherChain;

LuaEventDispatcher* GetLuaEventDispatcherByLuaState(lua_State* L)
{
	LuaEventDispatcherChain::iterator fndIter = g_xLuaEventDispatcherChain.find(L);
	if(fndIter == g_xLuaEventDispatcherChain.end())
	{
		return NULL;
	}
	return (LuaEventDispatcher*)fndIter->second;
}

void RegisterLuaEventDispatcher(LuaEventDispatcher* _pDispatcher)
{
	lua_State* L = _pDispatcher->GetLuaState();

	if(NULL == L)
	{
		return;
	}

	LuaEventDispatcherChain::iterator fndIter = g_xLuaEventDispatcherChain.find(L);
	if(fndIter == g_xLuaEventDispatcherChain.end())
	{
		g_xLuaEventDispatcherChain.insert(std::make_pair(L, _pDispatcher));
	}
	else
	{
		assert("LuaState has registered a dispatcher");
	}
}

void UnregisterLuaEventDispatcher(LuaEventDispatcher* _pDispatcher)
{
	lua_State* L = _pDispatcher->GetLuaState();

	if(NULL == L)
	{
		return;
	}

	LuaEventDispatcherChain::iterator fndIter = g_xLuaEventDispatcherChain.find(L);
	if(fndIter != g_xLuaEventDispatcherChain.end())
	{
		g_xLuaEventDispatcherChain.erase(L);
	}
}
//////////////////////////////////////////////////////////////////////////
LuaEventDispatcher::LuaEventDispatcher()
{
	m_pLuaState = NULL;
	m_bDispatching = false;
}

LuaEventDispatcher::~LuaEventDispatcher()
{
	Unbind();
}

//////////////////////////////////////////////////////////////////////////
void LuaEventDispatcher::Bind(lua_State* L)
{
	if(NULL != m_pLuaState)
	{
		assert("dispatcher has been binded");
		return;
	}
	m_pLuaState = L;
	RegisterLuaEventDispatcher(this);
	luaext_open(L);

	//	register static functions
	lua_register(L, "luaRegisterHandler", &LuaEventDispatcher::luaRegisterHandler);
	lua_register(L, "luaUnregisterHandler", &LuaEventDispatcher::luaUnregisterHandlerById);
}

void LuaEventDispatcher::Unbind()
{
	UnregisterLuaEventDispatcher(this);
	m_pLuaState = NULL;
}

int LuaEventDispatcher::DispatchEvent(const LuaDispatchEvent* _pEvent)
{
	if(NULL == m_pLuaState)
	{
		return 0;
	}

	LuaEventHandlerList* pHandlers = NULL;

	LuaEventHandlerListMap::const_iterator fndIter = m_xHandlers.find(_pEvent->nEventId);
	if(fndIter == m_xHandlers.end())
	{
		return 0;
	}

	pHandlers = fndIter->second;
	int nCount = 0;
	m_bDispatching = true;

	for(LuaEventHandlerList::iterator handlerIter = pHandlers->begin();
		handlerIter != pHandlers->end();
		++handlerIter)
	{
		const LuaEventHandler& refHandler = *handlerIter;
		__InternalDispatch(refHandler.nHandlerId, _pEvent);
		++nCount;
	}

	m_bDispatching = false;

	//	remove all unregister handlers
	if(!m_xWaitUnregisterList.empty())
	{
		for(WaitUnregisterHandlerList::const_iterator begIter = m_xWaitUnregisterList.begin();
			begIter != m_xWaitUnregisterList.end();
			++begIter)
		{
			int nHandlerId = *begIter;
			UnregisterHandler(nHandlerId);
		}
		m_xWaitUnregisterList.clear();
	}

	return nCount;
}

int LuaEventDispatcher::__InternalDispatch(int _nHandlerId, const LuaDispatchEvent* _pEvent)
{
	//	find the ref function and invoke OnDispatchEvent
	int nRef = luaext_getLuaFunctionByRefId(m_pLuaState, _nHandlerId);
	if(nRef == 0)
	{
		return 0;
	}

	//	stack: func
	LuaDispatchInfo dispatchInfo;
	int nArgs = OnDispatchEvent(_pEvent, &dispatchInfo);

	int nCallResult = lua_pcall(m_pLuaState, dispatchInfo.nArgs, dispatchInfo.nResults, 0);
	if(0 != nCallResult)
	{
		OnDispatchEventResult(_pEvent, false);

		//	print the error msg
		const char* pszErrMsg = lua_tostring(m_pLuaState, -1);
		printf("%s:%d %s\n", __FUNCTION__, __LINE__, pszErrMsg);
		lua_pop(m_pLuaState, 1);

		return 0;
	}
	else
	{
		OnDispatchEventResult(_pEvent, true);

		//	pop the results
		lua_pop(m_pLuaState, dispatchInfo.nResults);

		return 1;
	}
}

int LuaEventDispatcher::GetLuaFunction(int _nRefId)
{
	if(NULL == m_pLuaState)
	{
		return 0;
	}

	int nRet = luaext_getLuaFunctionByRefId(m_pLuaState, _nRefId);
	return nRet;
}

void LuaEventDispatcher::RegisterHandler(int _nEvtId, int _nHandlerId)
{
	LuaEventHandlerListMap::iterator fndIter = m_xHandlers.find(_nEvtId);
	LuaEventHandlerList* pHandlers = NULL;

	if(fndIter == m_xHandlers.end())
	{
		//	new handler
		pHandlers = new LuaEventHandlerList;
		m_xHandlers.insert(std::make_pair(_nEvtId, pHandlers));
	}
	else
	{
		pHandlers = fndIter->second;
	}

	LuaEventHandler handler;
	handler.nEvtId = _nEvtId;
	handler.nHandlerId = _nHandlerId;
	pHandlers->push_back(handler);
}

void LuaEventDispatcher::UnregisterHandler(int _nHandlerId)
{
	LuaEventHandlerListMap::iterator begIter = m_xHandlers.begin();
	LuaEventHandlerListMap::iterator endIter = m_xHandlers.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		LuaEventHandlerList* pHandlers = begIter->second;

		for(LuaEventHandlerList::iterator handlerIter = pHandlers->begin();
			handlerIter != pHandlers->end();
			)
		{
			const LuaEventHandler& refHandler = *handlerIter;
			if(refHandler.nHandlerId == _nHandlerId)
			{
				handlerIter = pHandlers->erase(handlerIter);
			}
			else
			{
				++handlerIter;
			}
		}
	}
}

void LuaEventDispatcher::ClearHandlers()
{
	LuaEventHandlerListMap::iterator begIter = m_xHandlers.begin();
	LuaEventHandlerListMap::iterator endIter = m_xHandlers.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		LuaEventHandlerList* pHandlers = begIter->second;

		delete pHandlers;
		pHandlers = NULL;
	}

	m_xHandlers.clear();
}
//////////////////////////////////////////////////////////////////////////
//	static functions export to lua
int LuaEventDispatcher::luaRegisterHandler(lua_State* L)
{
	//	stack: evtid func
	int nRefId = 0;

	do 
	{
		LuaEventDispatcher* pDispatcher = GetLuaEventDispatcherByLuaState(L);
		if(NULL == pDispatcher)
		{
			break;
		}

		if(!lua_isnumber(L, -2))
		{
			break;
		}
		int nEvtId = int(lua_tonumber(L, -2));
		nRefId = luaext_registerLuaFunction(L, lua_gettop(L));

		if(0 == nRefId)
		{
			break;
		}

		//	register event and handler
		pDispatcher->RegisterHandler(nEvtId, nRefId);
	} while (false);

	lua_pushnumber(L, nRefId);
	return 1;
}

/*int LuaEventDispatcher::luaUnregisterFunction(lua_State* L)
{
	//	stack 
	return 0;
}*/

int LuaEventDispatcher::luaUnregisterHandlerById(lua_State* L)
{
	//	stack: refid
	int nRefId = 0;

	do 
	{
		LuaEventDispatcher* pDispatcher = GetLuaEventDispatcherByLuaState(L);
		if(NULL == pDispatcher)
		{
			break;
		}

		if(!lua_isnumber(L, -1))
		{
			break;
		}

		nRefId = int(lua_tonumber(L, -1));
		luaext_unregisterLuaFunctionByRefId(L, nRefId);

		//	unregister event handler
		if(!pDispatcher->IsDispatching())
		{
			pDispatcher->UnregisterHandler(nRefId);
		}
		else
		{
			pDispatcher->PushWaitUnregisterHandler(nRefId);
		}
	} while (false);

	lua_pushnumber(L, nRefId);
	return 1;
}