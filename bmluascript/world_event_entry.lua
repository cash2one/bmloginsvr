--	世界事件脚本

--	世界事件注册表
local registeredWorldActivityTable = {}

local function RegisterWorldActivity(_activity)
	local aid = _activity.id
	if nil == aid then
		return
	end
	
	registeredWorldActivityTable[aid] = _activity
	_activity:OnRegistered()
end

local function GetWorldActivity(_id)
	return registeredWorldActivityTable[_id]
end

--	回调
local function OnWorldEventActive(_id)
	debug("world event:".._id.." active")
	local activity = GetWorldActivity(_id)
	if nil == activity then
		debug("OnWorldEventActive: Invalid id:".._id)
		return
	end
	
	activity:OnActive()
	activity.running = true
end
local handleOnWorldEventActive = 0

local function OnWorldEventUpdate()
	for _, v in pairs(registeredWorldActivityTable) do
		if v.running then
			local ret = v:OnUpdate()
			if ret then
				v.running = false
				v:OnFinish()
			end
		end
	end
end
local handleOnWorldEventUpdate = 0

local function OnWorldLoginServerConnected()
	for _, v in pairs(registeredWorldActivityTable) do
		v:OnLoginServerConnected()
	end
end
local handleOnWorldLoginServerConnected = 0

--	世界启动
local function onWorldRunning()
	--	启动后 注册事件监听
	handleWorldEventActive = luaRegisterHandler(kLuaEvent_WorldScheduleActive, OnWorldEventActive)
	handleWorldEventUpdate = luaRegisterHandler(kLuaEvent_WorldUpdate, OnWorldEventUpdate)
	handleOnWorldLoginServerConnected = luaRegisterHandler(kLuaEvent_WorldLoginServerConnected, OnWorldLoginServerConnected)
end
local handleWorldRunning = luaRegisterHandler(kLuaEvent_WorldStartRunning, onWorldRunning)


--	载入各事件脚本
local worldEvent1 = require("server/world/world_event_1").new()
RegisterWorldActivity(worldEvent1)