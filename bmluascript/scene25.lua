-- 凌霄天界

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	门 狐月岛
	scene:CreateDoorEvent(24, 14, 53, 54, 11)
	--	门 凌霄殿
	scene:CreateDoorEvent(26, 35, 30, 4, 54)

	--	各种狐狸
	scene:CreateMonster(94, 27, 21, 80, 4, 60000)
	scene:CreateMonster(95, 27, 21, 80, 4, 60000)
	scene:CreateMonster(96, 27, 21, 80, 4, 60000)

	scene:CreateMonster(94, 34, 38, 80, 4, 60000)
	scene:CreateMonster(95, 34, 38, 80, 4, 60000)
	scene:CreateMonster(96, 34, 38, 80, 4, 60000)

	scene:CreateMonster(94, 52, 26, 80, 4, 60000)
	scene:CreateMonster(95, 52, 26, 80, 4, 60000)
	scene:CreateMonster(96, 52, 26, 80, 4, 60000)

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
	--MirLog("STEP"..player:GetQuest():GetQuestStep(1).."monsterid"..monsterid)
	local monsterid = monster:GetMonsterID()
	if player:GetQuest():GetQuestStep(1) == 22 then
		if monsterid == 94 or monsterid == 95 or monsterid == 96 then
			local curcounter = player:GetQuest():GetQuestCounter(1)
			if curcounter < 50 then
				--curcounter = curcounter + 1
				curcounter = 50
				player:GetQuest():SetQuestCounter(1, curcounter)
				player:SendSystemMessage("杀死"..curcounter.."只怪物")
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
