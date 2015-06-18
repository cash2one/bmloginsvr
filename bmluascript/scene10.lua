--	地牢二层
--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	-- 门 地牢一层
	scene:CreateDoorEvent(9, 33, 56, 31, 8)

	--	触龙神
	scene:CreateMonsterLater(19, 67, 36, 0, 1, 300000)

	scene:CreateMonster(39, 50, 35, 10, 4, 40000)
	scene:CreateMonster(40, 50, 35, 10, 4, 40000)
	scene:CreateMonster(41, 50, 35, 10, 4, 40000)
	scene:CreateMonster(42, 50, 35, 10, 4, 40000)
	scene:CreateMonster(43, 50, 35, 10, 4, 40000)

	scene:CreateMonster(39, 39, 49, 10, 4, 40000)
	scene:CreateMonster(40, 39, 49, 10, 4, 40000)
	scene:CreateMonster(41, 39, 49, 10, 4, 40000)
	scene:CreateMonster(42, 39, 49, 10, 4, 40000)
	scene:CreateMonster(43, 39, 49, 10, 4, 40000)

	scene:CreateMonsterLater(44, 50, 35, 5, 1, 80000)
	scene:CreateMonsterLater(44, 39, 49, 5, 1, 80000)

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
	local posX = monster:GetCoordXInt()
	local posY = monster:GetCoordYInt()
	if monster:GetMonsterID() == 19 then
		player:GetLocateScene():CreateDoorEventWithNotification(38, posX, posY, 9, 25, 120000)
	end

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
	if x == 67 and y == 36 then
		if player:GetLevel() < 30 then
			player:SendSystemMessage("一股强大的力量将你弹开，你需要更加强大才能进入")
			return false
		end
	end
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
