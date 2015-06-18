--	牛魔大厅脚本
--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 封魔密道
	scene:CreateDoorEvent(20, 46, 16, 8, 81)

	--	小怪物
	scene:CreateMonster(69, 49, 41, 40, 15, 120000)
	scene:CreateMonster(70, 49, 41, 40, 15, 120000)
	scene:CreateMonster(72, 49, 41, 40, 8, 120000)
	--	将军
	scene:CreateMonsterLater(71, 49, 41, 40, 2, 60000)
	--	祭祀
	scene:CreateMonsterLater(73, 49, 41, 40, 2, 60000)

	--	牛魔王
	scene:CreateMonsterLater(16, 49, 41, 20, 1, 360000)

	--	变异龙神
	--scene:CreateMonsterLater(80, 49, 41, 20, 1, 780000)

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
	local monsterid = monster:GetMonsterID()
	local createseed = math.random(1, 4)
	if monsterid == 16 and
	createseed == 1 then
		local posX = monster:GetCoordXInt()
		local posY = monster:GetCoordYInt()
		player:GetLocateScene():CreateDoorEventWithNotification(36, posX, posY, 84, 85, 2 * 60 * 1000)
	end
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
