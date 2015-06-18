--	兽人古墓二层脚本文件

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	兽人古墓一层门
	scene:CreateDoorEvent(2, 37, 82, 86, 14)
	--	沃玛神殿门
	scene:CreateDoorEvent(5, 35, 17, 60, 61)
	--	NPC小宝
	scene:CreateNPC(10002, 11, 35)

	--	初始化怪物
	scene:CreateMonster(4, 50, 50, 20, 5, 60000)
	scene:CreateMonster(6, 50, 50, 20, 5, 60000)
	scene:CreateMonster(7, 50, 50, 20, 5, 60000)
	scene:CreateMonster(9, 50, 50, 20, 4, 60000)

end

--2秒钟调用一次 用来更新场景信息
function OnUpdateScene(scene)

end


function GetVersion()
end

--NPC任务
gNPCQuestTable = {}
gNPCButtonTable = {}

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	if x == 35 and y == 17 then
		local itemcount = player:CountItem(277)
		if itemcount > 0 then
			player:ClearItem(277, 1)
			return true
		else
			player:SendSystemMessage("没有神殿卷轴的力量无法进入")
			return false
		end
	end

	return true
end


-- NPC 小宝
gNPCQuestTable[10002] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10002] =
function(bid, npc, player)
	if bid == 2 then
		local curstep = player:GetQuest():GetQuestStep(0)
		if curstep == 25 then
			player:GetQuest():SetQuestStep(0, 26)
			player:ShowQuestDlg(npc, 0, 1)
			player:SyncQuestData(0)
		else
			player:ShowQuestDlg(npc, 0, 2)
		end

	end
end





function OnMonsterDead(monster, player)
	local monsterid = monster:GetMonsterID()

	if player:GetQuest():GetQuestStep(1) == 8 and monsterid == 9 then
		--if player:GetLocateScene():IsItemInGround(736, 33, 16) then
			--player:GetLocateScene():RemoveGroundItem(736, 33, 16)
			player:GetQuest():SetQuestStep(1, 9)
			player:SendSystemMessage("任务已完成")
			player:SyncQuestData(1)
		--end
	end

	if player:GetQuest():GetQuestStep(1) >= 11 and monsterid == 9 then
		local emptybag = player:GetBagEmptySum()
		if emptybag >= 1 then
			if math.random(2) == 1 then
				player:AddItem(277)
			end
		end
	end

	if player:GetQuest():GetQuestStep(1) == 19 and monsterid == 9 then
		local emptybag = player:GetBagEmptySum()
		if emptybag >= 1 then
			if math.random(3) == 1 then
				player:AddItem(625)
			end
		end
	end

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
