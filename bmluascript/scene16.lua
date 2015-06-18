--	恶魔之地脚本

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 赤月峡谷东
	scene:CreateDoorEvent(15, 11, 26, 78, 12)

	--	赤月恶魔
	scene:CreateMonsterLater(51, 17, 21, 0, 1, 480000)
	--scene:CreateMonster(51, 17, 21)
	--	幻影蜘蛛
	scene:CreateMonster(52, 14, 18, 0, 1, 90000)
	--	月魔
	scene:CreateMonster(54, 19, 19, 16, 6, 120000)
	--	花吻蜘蛛
	scene:CreateMonster(55, 19, 19, 16, 6, 120000)
	--	黑锷蜘蛛
	scene:CreateMonster(56, 19, 19, 16, 6, 120000)

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
	if monsterid == 51 then
		local step = player:GetQuest():GetQuestStep(51)
		if step == 2 then
			if math.random(3) == 1 then
				local leftbag = player:GetBagEmptySum()
				local itemcounter = player:CountItem(129)
				if leftbag >= 1 and itemcounter == 0 then
					player:AddItem(129)
				--else
					--MirLog("Already have one...")
				end
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
