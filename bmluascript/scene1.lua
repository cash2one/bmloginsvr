--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	随机数
	math.randomseed(tostring(os.time()):reverse():sub(1, 6))
	--	落霞岛门
	scene:CreateDoorEvent(0, 200, 252, 7, 6)
	--	兽人古墓门
	scene:CreateDoorEvent(2, 268, 107, 18, 101)
	--	王老伯
	scene:CreateNPC(10004, 54, 53)

	--scene:CreateGroundItem(257, 55, 53)

	--	初始化怪物
	--	稻草人
	scene:CreateMonster(13, 192, 240, 10, 5, 60000)
	--	多钩猫钉耙猫
	scene:CreateMonster(2, 117, 240, 10, 5, 60000)
	scene:CreateMonster(8, 117, 247, 10, 5, 60000)
	--	半兽勇士
	scene:CreateMonster(10, 53, 187, 30, 5, 120000)
	--	半兽战士
	scene:CreateMonster(28, 80, 80, 15, 5, 120000)

	--	多角虫
	scene:CreateMonster(77, 233, 91, 60, 10, 60000)
	--	300多角虫
	scene:CreateMonsterLater(76, 209, 135, 50, 6, 120000)
	--	900多角虫
	scene:CreateMonsterLater(75, 172, 71, 80, 3, 120000)
	--scene:CreateMonsterLater(75, 234, 72, 5, 1, 120000)
	--scene:CreateMonsterLater(75, 234, 72, 5, 1, 120000)

end

--2秒钟调用一次 用来更新场景信息
function OnUpdateScene(scene)

end

function GetVersion()
end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	return true
end

--NPC任务
gNPCQuestTable = {}
gNPCButtonTable = {}



--	10004 王老伯
gNPCQuestTable[10004] =
function(npc, player)
	local curstep = player:GetQuest():GetQuestStep(1)
	player:ShowQuestDlg(npc, 0, 0)

end

gNPCButtonTable[10004] =
function(bid, npc, player)

	local curstep = player:GetQuest():GetQuestStep(1)

	if bid == 2 then
		local itemsum = player:CountItem(257)
		if curstep == 5 then
			if itemsum == 1 then
				player:ClearItem(257, itemsum)
				player:GetQuest():SetQuestStep(1, 6)
				player:ShowQuestDlg(npc, 0, 2)
				player:GainExp(1500)
				player:SyncQuestData(1)
			else
				player:ShowQuestDlg(npc, 0, 1)
			end
		elseif curstep == 7 then
			--local itemsum = player:CountItem(736)
			--if itemsum == 0 then
			--	player:AddItem(736)
			--end
			player:ShowQuestDlg(npc, 0, 3)
			player:GetQuest():SetQuestStep(1, 8)
			player:SyncQuestData(1)

		elseif curstep == 8 then
			--local itemsum = player:CountItem(736)
			--if itemsum == 0 then
				--player:AddItem(736)
			--end
			player:ShowQuestDlg(npc, 0, 4)

		elseif curstep == 9 then
			player:ShowQuestDlg(npc, 0, 5)

		elseif curstep == 10 then
			player:ShowQuestDlg(npc, 0, 6)

		elseif curstep == 11 then
			player:ShowQuestDlg(npc, 0, 8)

		else
			player:ShowQuestDlg(npc, 0, 1)
		end

	elseif bid == 3 then
		local emptybag = player:GetBagEmptySum()
		if emptybag >= 2 then
			player:GainExp(1500)
			player:AddMoney(5000)
			player:ShowQuestDlg(npc, 0, 6)
			player:GetQuest():SetQuestStep(1, 10)
			player:SyncQuestData(1)

			if player:GetHeroSex() == 1 then
				player:AddItem(61)
			elseif player:GetHeroSex() == 2 then
				player:AddItem(81)
			end

			if player:GetHeroJob() == 0 then
				player:AddItem(44)
			elseif player:GetHeroJob() == 1 then
				player:AddItem(39)
			elseif player:GetHeroJob() == 2 then
				player:AddItem(46)
			end

		else
			player:ShowQuestDlg(npc, 0, 7)
		end

	elseif bid == 4 then
		local itemcount = player:CountItem(78)
		if itemcount > 0 then
			player:ClearItem(78, itemcount)
			player:AddItem(277)
			player:GetQuest():SetQuestStep(1, 11)
			player:SyncQuestData(1)
			player:ShowQuestDlg(npc, 0, 8)
		else
			player:ShowQuestDlg(npc, 0, 9)
		end

	elseif bid == 5 then
		local itemcount = player:CountItem(410)
		if itemcount > 0 then
			--player:ClearItem(410, itemcount)
			player:GetQuest():SetQuestStep(1, 12)
			player:SyncQuestData(1)
			player:ShowQuestDlg(npc, 0, 11)
			player:GainExp(50000)
			player:AddMoney(10000)
		else
			player:ShowQuestDlg(npc, 0, 10)
		end


	elseif bid == 111 then
		if curstep >= 9 then
			player:ShowQuestDlg(npc, 0, 111)
		else
			player:ShowQuestDlg(npc, 0, 255)
		end

	elseif bid == 112 then
		local money = player:GetMoney()
		if money < 2000 then
			player:ShowQuestDlg(npc, 0, 254)
		else
			player:MinusMoney(2)
			player:FlyToMap(12, 51, 4)
			player:HideQuestDlg()
		end
	end

end







-- OnMonsterDead
function OnMonsterDead(monster, player)
	local monsterid = monster:GetMonsterID()

	if player:GetQuest():GetQuestStep(0) == 11 and monsterid == 13 then
		local curcounter = player:GetQuest():GetQuestCounter(0)
			if curcounter < 9 then
				curcounter = curcounter + 1
				player:GetQuest():SetQuestCounter(0, curcounter)
				player:SendSystemMessage("任务信息:杀死"..curcounter.."个稻草人")
				player:SyncQuestData(0)
			elseif curcounter == 9 then
				player:GetQuest():SetQuestCounter(0, curcounter + 1)
				player:SendSystemMessage("任务信息:任务完成")
				player:SyncQuestData(0)
			end

	elseif player:GetQuest():GetQuestStep(0) == 14 and (monsterid == 2 or monsterid == 8) then
		local curcounter = player:GetQuest():GetQuestCounter(0)
			if curcounter < 9 then
				curcounter = curcounter + 1
				player:GetQuest():SetQuestCounter(0, curcounter)
				player:SendSystemMessage("任务信息:杀死"..curcounter.."只怪物")
				player:SyncQuestData(0)
			elseif curcounter == 9 then
				player:GetQuest():SetQuestCounter(0, curcounter + 1)
				player:SendSystemMessage("任务信息:任务完成")
				player:SyncQuestData(0)
			end
	elseif player:GetQuest():GetQuestStep(1) == 2 and monsterid == 28 then
		local bagsum = player:CountItem(1093)
		if bagsum == 0 then
			local empty = player:GetBagEmptySum()
			if empty >= 1 then
				if math.random(4) >= 0 then
					player:AddItem(1093)
				end
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
