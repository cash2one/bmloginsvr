--	浴血之地

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 魔龙山
	scene:CreateDoorEvent(22, 29, 146, 226, 102)

	scene:CreateMonster(83, 55,126, 30, 10, 60000)
	scene:CreateMonster(84, 55,126, 30, 10, 60000)
	scene:CreateMonster(85, 55,126, 30, 10, 60000)

	scene:CreateMonster(83, 81,55, 30, 10, 60000)
	scene:CreateMonster(84, 81,55, 30, 10, 60000)
	scene:CreateMonster(85, 81,55, 30, 10, 60000)

	scene:CreateMonster(83, 144,101, 30, 10, 60000)
	scene:CreateMonster(84, 144,101, 30, 10, 60000)
	scene:CreateMonster(85, 144,101, 30, 10, 60000)

	scene:CreateMonsterLater(87, 80,59, 40, 3, 180000)
	scene:CreateMonsterLater(88, 80,59, 40, 3, 180000)
	scene:CreateMonsterLater(89, 80,59, 40, 3, 180000)
	scene:CreateMonsterLater(86, 80,59, 5, 1, 220000)

	scene:SetCannotStay()
	scene:SetAutoReset()

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
