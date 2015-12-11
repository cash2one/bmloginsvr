--	世界NPC 处理
local worldNPCTable = {}

local function OnNPCActive(_player, _npc, _bid)
	local npcHandler = worldNPCTable[_npc:GetAttribID()]
	if nil == npcHandler then
		debug("Can't handle world npc command, npc id:".._npc:GetAttribID())
		return
	end
	
	npcHandler:OnNPCActive(_player, _npc, _bid)
end

local handlerOnNPCActive = luaRegisterHandler(kLuaEvent_WorldNPCActive, OnNPCActive)