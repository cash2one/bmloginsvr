-- 静之冰窟

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	-- 门 静寂雪原西
	scene:CreateDoorEvent(32, 88, 454, 262, 34)
	--	门 静寂雪原东
	scene:CreateDoorEvent(33, 458, 414, 256, 40)
	--	门 冰谷走廊
	scene:CreateDoorEvent(30, 419, 41, 29, 478)

	scene:CreateMonster(103, 133, 103, 40, 20, 6000)
	scene:CreateMonster(104, 133, 103, 40, 20, 6000)
	scene:CreateMonster(108, 133, 103, 40, 20, 6000)
	scene:CreateMonster(111, 133, 103, 40, 20, 6000)

	scene:CreateMonster(103, 338, 126, 40, 20, 6000)
	scene:CreateMonster(104, 338, 126, 40, 20, 6000)
	scene:CreateMonster(108, 338, 126, 40, 20, 6000)
	scene:CreateMonster(111, 338, 126, 40, 20, 6000)

	scene:CreateMonster(103, 317, 336, 40, 20, 6000)
	scene:CreateMonster(104, 317, 336, 40, 20, 6000)
	scene:CreateMonster(108, 317, 336, 40, 20, 6000)
	scene:CreateMonster(111, 317, 336, 40, 20, 6000)

	scene:CreateMonster(103, 133, 326, 40, 20, 6000)
	scene:CreateMonster(104, 133, 326, 40, 20, 6000)
	scene:CreateMonster(108, 133, 326, 40, 20, 6000)
	scene:CreateMonster(111, 133, 326, 40, 20, 6000)

	scene:CreateMonster(103, 203, 203, 40, 20, 6000)
	scene:CreateMonster(104, 203, 203, 40, 20, 6000)
	scene:CreateMonster(108, 203, 203, 40, 20, 6000)
	scene:CreateMonster(111, 203, 203, 40, 20, 6000)

	scene:SetAutoReset()
	scene:SetCannotStay()

end

--2秒钟调用一次 用来更新场景信息
function OnUpdateScene(scene)

end

--NPC任务 function(npc, player)
gNPCQuestTable = {}
--NPC按钮点击 function(bid, npc, player)
gNPCButtonTable = {}


-- OnMonsterDead 怪物死亡回调
function OnMonsterDead(monster, player)
end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)

	return true

end

function GetVersion()
end



---------------------------------------------------------
--	以下为分配函数 不必动

--玩家点击NPC调用 用于弹出对话框
function OnNPCClick(npc, player)

	local npcid = npc:GetAttribID()

	func = gNPCQuestTable[npcid]
	if(func ~= nil) then
		func(npc, player)
	else
		MirLog("不存在的NPC对话脚本"..npcid)
	end

end

--玩家点击按钮 根据按钮ID来做相应动作
function OnDlgButtonClick(bid, npc, player)

	local npcid = npc:GetAttribID()

	func = gNPCButtonTable[npcid]
	if(func ~= nil) then
		func(bid, npc, player)
	else
		MirLog("不存在的NPC按钮脚本"..npcid)
	end

end
