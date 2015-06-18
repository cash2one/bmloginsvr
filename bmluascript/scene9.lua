--服务器载入地图后调用 用于创建地图的NPC和怪物
--	地牢一层脚本
function OnMapLoaded(scene)
	--	门 通往封魔大陆
	scene:CreateDoorEvent(8, 16, 29, 204, 22)
	--	门 通往地牢二层
	scene:CreateDoorEvent(10, 31, 8, 33, 56)

	--	刷怪物
	scene:CreateMonster(39, 17, 17, 5, 4, 50000)
	scene:CreateMonster(40, 17, 17, 5, 4, 50000)
	scene:CreateMonster(41, 17, 17, 5, 4, 50000)
	scene:CreateMonster(42, 17, 17, 5, 4, 50000)
	scene:CreateMonster(43, 17, 17, 5, 4, 50000)

	scene:CreateMonster(39, 30, 20, 70, 4, 40000)
	scene:CreateMonster(40, 30, 20, 70, 4, 40000)
	scene:CreateMonster(41, 30, 20, 70, 4, 40000)
	scene:CreateMonster(42, 30, 20, 70, 4, 40000)
	scene:CreateMonster(43, 30, 20, 70, 4, 40000)

	scene:CreateMonsterLater(44, 23, 9, 2, 1, 120000)
	scene:CreateMonsterLater(44, 27, 27, 2, 1, 120000)
	--scene:CreateMonster(18, 17, 17)
	--scene:CreateMonster(48, 18, 17)
	--scene:CreateMonster(50, 17, 18)

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
	if curstep == 13 then
		if monsterid == 43 or monsterid == 39 then
			local monssum = player:GetQuest():GetQuestCounter(1)
			if monssum < 50 then
				monssum = monssum + 1
				--monssum = 50
				player:GetQuest():SetQuestCounter(1, monssum)
				player:SendSystemMessage("杀死"..monssum.."只怪物")
				player:SyncQuestData(1)
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
