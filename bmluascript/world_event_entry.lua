--	世界事件脚本

--	世界NPC列表
local registeredWorldNPCTable = {}

function RegisterWorldNPCHandler(_id, _func)
	local handlers = registeredWorldNPCTable[_id]
	if nil == handler then
		handlers = {}
		registeredWorldNPCTable[_id] = handlers
	end
	
	table.insert(handlers, _func)
end

function UnreigsterWorldNPCHandler(_id, _func)
	local handlers = registeredWorldNPCTable[_id]
	if nil == handler then
		return
	end
	
	for i = #handlers, 1, -1 do
		if handlers[i] == _func then
			table.remove(handlers, i)
		end
	end
end

function OnNPCActive(_event, _param, _npc, _player)
	local handlers = registeredWorldNPCTable[_npc:GetAttribID()]
	if nil == handlers then return end
	
	for _, v in ipairs(handlers) do
		v(_event, _param, _npc, _player)
	end
end

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