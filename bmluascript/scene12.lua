--	赤月峡谷 脚本

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 矿窑
	scene:CreateDoorEvent(4, 46, 189, 134, 20)
	--	门 赤月峡谷西
	scene:CreateDoorEvent(13, 13, 25, 42, 91)
	--	门 赤月峡谷东
	scene:CreateDoorEvent(15, 178, 53, 13, 18)

	scene:CreateMonster(56 , 74, 135, 70, 8, 60000)
	scene:CreateMonster(55 , 74, 135, 70, 8, 60000)
	scene:CreateMonster(54 , 74, 135, 70, 5, 60000)

	scene:CreateMonster(56 , 107, 81, 70, 8, 60000)
	scene:CreateMonster(55 , 107, 81, 70, 8, 60000)
	scene:CreateMonster(54 , 107, 81, 70, 5, 60000)

	scene:CreateMonster(56 , 90, 134, 70, 8, 60000)
	scene:CreateMonster(55 , 90, 134, 70, 8, 60000)
	scene:CreateMonster(54 , 90, 134, 70, 5, 60000)

	--	血僵尸
	scene:CreateMonsterLater(57, 77, 96, 20, 1, 180000)
	scene:CreateMonsterLater(57, 64, 58, 20, 1, 180000)
	scene:CreateMonsterLater(57, 183, 122, 20, 1, 180000)

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

	local curstep = player:GetQuest():GetQuestStep(1)
	if curstep == 18 and monsterid == 57 then
		local empty = player:GetBagEmptySum()
		local itemcount = player:CountItem(858)
		if empty >= 1 and itemcount == 0 then
			if math.random(5) == 1 then
				player:AddItem(858)
			end
		end
	end

	if curstep == 17 then
		if monsterid == 54 or monsterid == 55 or monsterid == 56 then
			local empty = player:GetBagEmptySum()
			if empty >= 1 then
				if math.random(10) == 1 then
					player:AddItem(854)
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
