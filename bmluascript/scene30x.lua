-- 冰谷走廊

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	门 静之冰谷
	scene:CreateDoorEvent(29, 29, 478, 256, 310)
	--	门 寒冰王座
	scene:CreateDoorEvent(34, 448, 48, 33, 171)

	scene:CreateMonster(103, 250, 250, 250, 30, 6000)
	scene:CreateMonster(104, 250, 250, 250, 30, 6000)
	scene:CreateMonster(108, 250, 250, 250, 30, 6000)
	scene:CreateMonster(111, 250, 250, 250, 30, 6000)

	--scene:CreateMonster(103, 272, 136, 40, 20, 6000)
	--scene:CreateMonster(104, 272, 136, 40, 20, 6000)
	--scene:CreateMonster(108, 272, 136, 40, 20, 6000)
	--scene:CreateMonster(111, 272, 136, 40, 20, 6000)

	--scene:CreateMonster(103, 438, 60, 40, 20, 6000)
	--scene:CreateMonster(104, 438, 60, 40, 20, 6000)
	--scene:CreateMonster(108, 438, 60, 40, 20, 6000)
	--scene:CreateMonster(111, 438, 60, 40, 20, 6000)

	--scene:CreateMonster(103, 395, 279, 40, 20, 6000)
	--scene:CreateMonster(104, 395, 279, 40, 20, 6000)
	--scene:CreateMonster(108, 395, 279, 40, 20, 6000)
	--scene:CreateMonster(111, 395, 279, 40, 20, 6000)

	--scene:CreateMonster(103, 109, 348, 40, 20, 6000)
	--scene:CreateMonster(104, 109, 348, 40, 20, 6000)
	--scene:CreateMonster(108, 109, 348, 40, 20, 6000)
	--scene:CreateMonster(111, 109, 348, 40, 20, 6000)

	scene:CreateMonsterLater(105, 173, 408, 3, 1, 12000)
	scene:CreateMonsterLater(105, 396, 288, 3, 1, 12000)
	scene:CreateMonsterLater(105, 232, 155, 3, 1, 12000)

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
