--矿窑脚本
--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	神秘学者
	scene:CreateNPC(10005, 13, 50)

	--	门 赤月峡谷
	scene:CreateDoorEvent(12, 134, 20, 46, 189)

	--	僵尸
	scene:CreateMonster(23, 38, 28, 50, 10, 60000)
	scene:CreateMonster(24, 38, 28, 50, 10, 60000)
	scene:CreateMonster(25, 38, 28, 50, 10, 60000)
	scene:CreateMonster(26, 38, 28, 50, 10, 60000)
	scene:CreateMonster(23, 102, 88, 50, 10, 60000)
	scene:CreateMonster(24, 102, 88, 50, 10, 60000)
	scene:CreateMonster(25, 102, 88, 50, 10, 60000)
	scene:CreateMonster(26, 102, 88, 50, 10, 60000)
	scene:CreateMonster(23, 65, 104, 50, 10, 60000)
	scene:CreateMonster(24, 65, 104, 50, 10, 60000)
	scene:CreateMonster(25, 65, 104, 50, 10, 60000)
	scene:CreateMonster(26, 65, 104, 50, 10, 60000)

	scene:CreateMonster(27, 122, 56, 5, 1, 120000)
	scene:CreateMonster(27, 102, 132, 5, 1, 120000)
	scene:CreateMonster(27, 109, 27, 5, 1, 120000)
	scene:CreateMonster(27, 26, 102, 5, 1, 120000)
	scene:CreateMonster(27, 38, 28, 5, 1, 120000)
	scene:CreateMonster(27, 102, 88, 5, 1, 120000)
	scene:CreateMonster(27, 49, 88, 5, 1, 120000)

	scene:CreateMonster(127, 102, 88, 80, 1, 60000)

end

--2秒钟调用一次 用来更新场景信息
function OnUpdateScene(scene)

end

function GetVersion()
end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	if x == 134 and y == 20 then
		local curstep = player:GetQuest():GetQuestStep(1)

		if curstep >= 17 then
			return true
		else
			player:SendSystemMessage("可怕的封印之地，无法进入")
			return false
		end
	else
		return true
	end
end

--NPC任务 function(npc, player)
gNPCQuestTable = {}
--NPC按钮点击 function(bid, npc, player)
gNPCButtonTable = {}


-- OnMonsterDead 怪物死亡回调
function OnMonsterDead(monster, player)
	local curstep = player:GetQuest():GetQuestStep(50)
	local mainstep = player:GetQuest():GetQuestStep(1)
	local monsterid = monster:GetMonsterID()

	if monster:GetMonsterID() == 127 then
		local posX = monster:GetCoordXInt()
		local posY = monster:GetCoordYInt()
		player:GetLocateScene():CreateDoorEventWithNotification(35, posX, posY, 15, 13, 2 * 60 * 1000)
	end

	if monsterid == 23 or monsterid == 24 or monsterid == 25 or monsterid == 26 then
		if curstep == 1 then
			local itemsum = player:CountItem(75)
			local emptybag = player:GetBagEmptySum()
			if math.random(4) >= 0 and itemsum == 0 and emptybag > 0 then
				player:AddItem(75)
				player:SendSystemMessage("任务完成")
			end
		elseif curstep == 3 then
			local qstcounter = player:GetQuest():GetQuestCounter(50)
			if qstcounter < 20 then
				qstcounter = qstcounter + 1
				player:GetQuest():SetQuestCounter(50, qstcounter)
				player:SendSystemMessage("杀死"..qstcounter.."只僵尸")
				if qstcounter == 20 then
					player:SendSystemMessage("任务完成")
					player:GetQuest():SetQuestStep(50, 4)
					player:GetQuest():SetQuestCounter(50, 0)
				end
				player:SyncQuestData(50)
			end
		end

		if mainstep == 10 then
			local emptybag = player:GetBagEmptySum()
			if math.random(20) == 1 and emptybag > 0 then
				player:AddItem(78)
				player:SendSystemMessage("任务完成")
			end
		end
	end

	if player:GetQuest():GetQuestStep(1) == 19 and monsterid == 27 then
		local emptybag = player:GetBagEmptySum()
		if emptybag >= 1 then
			if math.random(3) == 1 then
				player:AddItem(626)
			end
		end
	end

end

---------------------------------------------------------
--	支线任务从50开始
---------------------------------------------------------
--	神秘学者
gNPCQuestTable[10005] =
function(npc, player)
	local curstep = player:GetQuest():GetQuestStep(1)
	if curstep == 15 then
		local itemcount = player:CountItem(855)
		if itemcount > 0 then
			player:ClearItem(855, itemcount)
			player:AddItem(863)
			player:GainExp(20000)
			player:AddMoney(2000)
			player:ShowQuestDlg(npc, 0, 10)
			player:GetQuest():SetQuestStep(1, 16)
			player:SyncQuestData(1)
		else
			player:ShowQuestDlg(npc, 0, 0)
		end
	else
		player:ShowQuestDlg(npc, 0, 0)
	end
end

gNPCButtonTable[10005] =
function(bid, npc, player)
	if bid == 2 then
		player:FlyToMap(55, 55, 1)
		player:HideQuestDlg()

	elseif bid == 3 then
		local curstep = player:GetQuest():GetQuestStep(50)
		if curstep == 0 then
			local curlevel = player:GetLevel()
			if curlevel >= 15 then
				player:ShowQuestDlg(npc, 0, 1)
			else
				player:ShowQuestDlg(npc, 0, 3)
			end

		elseif curstep == 1 then
			player:ShowQuestDlg(npc, 0, 2)

		elseif curstep == 2 then
			player:ShowQuestDlg(npc, 0, 6)

		elseif curstep == 3 then
			player:ShowQuestDlg(npc, 0, 7)

		elseif curstep == 4 then
			player:ShowQuestDlg(npc, 0, 8)

		elseif curstep == 5 then
			player:ShowQuestDlg(npc, 0, 9)

		else
			player:ShowQuestDlg(npc, 0, 100)

		end

	elseif bid == 4 then
		player:GetQuest():SetQuestStep(50, 1)
		player:SyncQuestData(50)
		player:ShowQuestDlg(npc, 0, 2)

	elseif bid == 5 then
		local itemcount = player:CountItem(75)
		if itemcount == 0 then
			player:ShowQuestDlg(npc, 0, 4)
		else
			player:ClearItem(75, itemcount)
			player:GetQuest():SetQuestStep(50, 2)
			player:SyncQuestData(50)
			player:ShowQuestDlg(npc, 0, 5)
			player:GainExp(3000)
			player:AddMoney(5000)

		end

	elseif bid == 6 then
		player:ShowQuestDlg(npc, 0, 7)
		player:GetQuest():SetQuestStep(50, 3)
		player:SyncQuestData(50)

	elseif bid == 7 then
		player:GetQuest():SetQuestStep(50, 5)
		player:ShowQuestDlg(npc, 0, 9)
		player:GainExp(4000)
		player:AddMoney(5000)

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
