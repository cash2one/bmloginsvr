#include "WorldEventDispatcher.h"
//////////////////////////////////////////////////////////////////////////
WorldEventDispatcher::WorldEventDispatcher()
{

}

WorldEventDispatcher::~WorldEventDispatcher()
{

}

//////////////////////////////////////////////////////////////////////////
int WorldEventDispatcher::OnDispatchEvent(const LuaDispatchEvent* _pEvent, LuaDispatchInfo* _pInfo)
{
	return 0;
}

void WorldEventDispatcher::OnDispatchEventResult(const LuaDispatchEvent* _pEvent, bool bResult)
{

}