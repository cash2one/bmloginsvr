--	魔龙岭脚本

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 封魔大陆
	scene:CreateDoorEvent(8, 49, 12, 239, 298)
	--	门 浴血之地
	scene:CreateDoorEvent(23, 226, 102, 29, 146)

	scene:CreateMonster(83, 41, 49, 20, 3, 60000)
	scene:CreateMonster(84, 41, 49, 20, 3, 60000)
	scene:CreateMonster(85, 41, 49, 20, 3, 60000)

	scene:CreateMonster(83, 61,84, 20, 3, 60000)
	scene:CreateMonster(84, 61,84, 20, 3, 60000)
	scene:CreateMonster(85, 61,84, 20, 3, 60000)

	scene:CreateMonster(83, 54,164, 20, 3, 60000)
	scene:CreateMonster(84, 54,164, 20, 3, 60000)
	scene:CreateMonster(85, 54,164, 20, 3, 60000)

	scene:CreateMonster(83, 98,172, 20, 3, 60000)
	scene:CreateMonster(84, 98,172, 20, 3, 60000)
	scene:CreateMonster(85, 98,172, 20, 3, 60000)

	scene:CreateMonster(83, 156,176, 20, 3, 60000)
	scene:CreateMonster(84, 156,176, 20, 3, 60000)
	scene:CreateMonster(85, 156,176, 20, 3, 60000)

	scene:CreateMonster(83, 134,58, 20, 3, 60000)
	scene:CreateMonster(84, 134,58, 20, 3, 60000)
	scene:CreateMonster(85, 134,58, 20, 3, 60000)

	scene:CreateMonster(83, 123,126, 20, 3, 60000)
	scene:CreateMonster(84, 123,126, 20, 3, 60000)
	scene:CreateMonster(85, 123,126, 20, 3, 60000)

	scene:CreateMonster(83, 193,80, 20, 3, 60000)
	scene:CreateMonster(84, 193,80, 20, 3, 60000)
	scene:CreateMonster(85, 193,80, 20, 3, 60000)
	scene:CreateMonsterLater(92, 55, 121, 0, 1, 300000)

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
