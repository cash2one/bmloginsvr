--	赤月峡谷东 脚本

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 赤月峡谷
	scene:CreateDoorEvent(12, 13, 18, 178, 53)
	--	门 恶魔之地
	scene:CreateDoorEvent(16, 78, 12, 11, 26)

	scene:CreateMonster(56 , 51, 43, 42, 10, 120000)
	scene:CreateMonster(55 , 51, 43, 42, 10, 120000)
	scene:CreateMonster(54 , 51, 43, 42, 10, 120000)

	--	双头血魔 弱
	scene:CreateMonsterLater(61, 51, 43, 45, 3, 180000)

	--	NPC 天尊
	scene:CreateNPC(10012, 51, 23)

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
	if monsterid == 61 then
		local step = player:GetQuest():GetQuestStep(51)

		if player:GetLevel() >= 40 and step == 0 then
			local leftbag = player:GetBagEmptySum()
			if leftbag >= 1 then
				if math.random(10) == 1 then
					player:AddItem(14)
					player:GetQuest():SetQuestStep(51, 1)
					player:SyncQuestData(51)
				end
			end
		end
	end

	--[[if step == 2 then
		local counter = player:GetQuest():GetQuestCounter(51)
		--local counter = 49
		if counter <= 49 then
			counter = counter + 1
			player:GetQuest():SetQuestCounter(51, counter)
			player:SyncQuestData(51)
		end
	end]]

end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	return true
end

function GetVersion()
end



--	NPC
--	10012 天尊
gNPCQuestTable[10012] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10012] =
function(bid, npc, player)
	if bid == 2 then
		local step = player:GetQuest():GetQuestStep(51)

		if step == 0 then
			player:ShowQuestDlg(npc, 0, 1)
		elseif step == 1 then
			--	支线任务窗口
			player:ShowQuestDlg(npc, 0, 4)
		elseif step == 2 then
			player:ShowQuestDlg(npc, 0, 5)
		end

	elseif bid == 3 then

		local luochacount = player:CountItem(96)
		local luxuecount  = player:CountItem(2)
		local gyqzcount = player:CountItem(59)
		local cjzzcount	= player:CountItem(55)
		local lwjcount = player:CountItem(56)

		if luochacount == 1 and luxuecount == 2 and gyqzcount == 1 and cjzzcount == 1 and lwjcount == 1 then

			player:ClearItem(96, 1)
			player:ClearItem(2, 2)
			player:ClearItem(59, 1)
			player:ClearItem(55, 1)
			player:ClearItem(56, 1)
			player:AddItem(66)
			player:ShowQuestDlg(npc, 0, 2)

		else

			player:ShowQuestDlg(npc, 0, 3)

		end

	elseif bid == 4 then
	--	换血魔之血
		local counter = player:CountItem(14)
		if counter == 1 then
			player:GetQuest():SetQuestStep(51, 2)
			player:SyncQuestData(51)
			player:ShowQuestDlg(npc, 0, 5)
			player:GainExp(1000)
			player:ClearItem(14, 1)
		else
			player:ShowQuestDlg(npc, 0, 8)
		end

	elseif bid == 5 then
	--	是否完成任务
		local counter = player:GetQuest():GetQuestCounter(51)
		local items = player:CountItem(129)

		--MirLog("counter:"..counter.."items"..items)

		if items == 1 then
			player:ShowQuestDlg(npc, 0, 6)
			player:GetQuest():SetQuestStep(51, 0)
			player:GetQuest():SetQuestCounter(51, 0)
			player:SyncQuestData(51)
			player:ClearItem(129, 1)
			player:GainExp(250000)
			player:AddMoney(150000)
		else
			player:ShowQuestDlg(npc, 0, 7)
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
