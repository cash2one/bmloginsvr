--	世界事件脚本

--	世界启动
local function onWorldRunning()
	debug("entry")
end

local handleWorldRunning = luaRegisterHandler(4, onWorldRunning)

--	世界事件注册表
local registeredWorldActivityTable = {}

function RegisterWorldActivity(_activity)
	local aid = _activity.id
	if nil == aid then
		return
	end
	
	registeredWorldActivityTable[aid] = _activity
end

function GetWorldActivity(_id)
	return registeredWorldActivityTable[_id]
end

function WorldEventActive(_id)
	local activity = GetWorldActivity(_id)
	if nil == activity then
		return
	end
	
	activity:OnActive()
	activity.running = true
end

function WorldEventUpdate()
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

function WorldEventInit()
	--	初始化所有的事件
	for _, v in pairs(registeredWorldActivityTable) do
		v.OnInit()
	end
end


--	载入各事件脚本
ENGINE:LoadModule("world_event_1")