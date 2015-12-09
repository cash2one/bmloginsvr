--	世界事件脚本

local function WorldEventActive(_id)
	local activity = GetWorldActivity(_id)
	if nil == activity then
		return
	end
	
	activity:OnActive()
	activity.running = true
end
local handleWorldEventActive = 0

local function WorldEventUpdate()
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
local handleWorldEventUpdate = 0

--	世界启动
local function onWorldRunning()
	--	启动后 注册事件监听
	handleWorldEventActive = luaRegisterHandler(kLuaEvent_WorldScheduleActive, WorldEventActive)
	handleWorldEventUpdate = luaRegisterHandler(kLuaEvent_WorldUpdate, WorldEventUpdate)
end
local handleWorldRunning = luaRegisterHandler(kLuaEvent_WorldStartRunning, onWorldRunning)

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


--	载入各事件脚本
local worldEvent1 = require("server/world/world_event_1").new()
RegisterWorldActivity(worldEvent1)