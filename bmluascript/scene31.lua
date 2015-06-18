-- 寒冰王座

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	门 狐月岛
	--scene:CreateDoorEvent(24, 66, 16, 43, 21)
	--	门 冰谷走廊
	scene:CreateDoorEvent(30, 33, 171, 448, 48)

	--	门 八卦峰
	scene:CreateDoorEvent(32, 141, 52, 12, 28)

	scene:CreateMonster(103, 100, 100, 100, 8, 6000)
	scene:CreateMonster(104, 100, 100, 100, 8, 6000)
	scene:CreateMonster(111, 100, 100, 100, 8, 6000)
	scene:CreateMonster(112, 100, 100, 100, 8, 6000)

	scene:CreateMonsterLater(105, 100, 100, 100, 2, 8000)
	scene:CreateMonsterLater(106, 101, 95, 2, 1, 30000)
	--scene:CreateMonsterLater(106, 104, 88, 10, 5, 30000)

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
