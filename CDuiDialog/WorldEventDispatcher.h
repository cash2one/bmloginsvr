#ifndef _INC_WORLDEVENTDISPATCHER_
#define _INC_WORLDEVENTDISPATCHER_
//////////////////////////////////////////////////////////////////////////
#include "LuaEventDispatcher.h"
//////////////////////////////////////////////////////////////////////////
enum WorldEvent
{
	kWorldEvent_None,
	kWorldEvent_Update,
};
//////////////////////////////////////////////////////////////////////////
class WorldEventDispatcher : public LuaEventDispatcher
{
public:
	WorldEventDispatcher();
	virtual ~WorldEventDispatcher();

public:
	virtual int OnDispatchEvent(const LuaDispatchEvent* _pEvent, LuaDispatchInfo* _pInfo);
	virtual void OnDispatchEventResult(const LuaDispatchEvent* _pEvent, bool bResult);
};
//////////////////////////////////////////////////////////////////////////
#endif