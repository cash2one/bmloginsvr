--服务器载入地图后调用 用于创建地图的NPC和怪物
--	祖玛大殿脚本
function OnMapLoaded(scene)
	--	门 封魔大陆
	scene:CreateDoorEvent(8, 71, 32, 209, 218)

	-- 普通祖玛
	scene:CreateMonster(20, 52, 52, 50, 4, 60000)
	scene:CreateMonster(46, 52, 52, 50, 4, 60000)
	scene:CreateMonster(49, 52, 52, 50, 4, 60000)
	scene:CreateMonster(46, 52, 52, 50, 4, 60000)

	--	暗之系列
	scene:CreateMonsterLater(48, 52, 52, 50, 2, 120000)
	scene:CreateMonsterLater(50, 52, 52, 50, 1, 120000)
	scene:CreateMonsterLater(47, 52, 52, 50, 2, 120000)

	--	教主
	scene:CreateMonsterLater(18, 52, 52, 1, 1, 240000)

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
	if monsterid == 47 then
		local curstep = player:GetQuest():GetQuestStep(1)
		if curstep == 14 then
			local emptybag = player:GetBagEmptySum()
			if emptybag > 0 and math.random(5) == 1 then
				player:AddItem(855)
			end
		end
	end

	if player:GetQuest():GetQuestStep(1) == 19 and monsterid == 18 then
		local emptybag = player:GetBagEmptySum()
		if emptybag >= 1 then
			if math.random(3) == 1 then
				player:AddItem(629)
			end
		end
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
