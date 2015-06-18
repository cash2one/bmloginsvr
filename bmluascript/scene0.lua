--服务器载入地图后调用 用于创建地图的NPC和怪物
local theScene = nil

function OnMapLoaded(scene)
	theScene = scene

--	新手指引
	scene:CreateNPC(10010, 21, 14)
--	铁匠
	scene:CreateNPC(10000, 49, 10)
--	渔夫
	scene:CreateNPC(10001, 59, 28)
--	小贩
	scene:CreateNPC(10003, 13, 30)
--	仓库老板
	scene:CreateNPC(10006, 25, 13)
--	门
	scene:CreateDoorEvent(1, 7, 6, 200, 252)
--	城市区域
	scene:SetCityRange(0, 0, 59, 29, 20, 17)
--	地图怪物
	scene:CreateMonster(21, 18, 25, 20, 7, 60000)

	scene:CreateMonster(22, 28 , 18, 20, 7, 60000)

	scene:CreateMonster(1, 77, 119, 20, 7, 60000)

end

function GetVersion()
end

--2秒钟调用一次 用来更新场景信息
function OnUpdateScene(scene)

end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	return true
end

--NPC任务
gNPCQuestTable = {}
gNPCButtonTable = {}

--10010 新手指引 新手任务(questid 0)
gNPCQuestTable[10010] =
function(npc, player)
	local step = player:GetQuest():GetQuestStep(0)
	if step >= 18 then
		player:ShowQuestDlg(npc, 0, 21)
	else
		player:ShowQuestDlg(npc, 0, step)
	end
end
gNPCButtonTable[10010] =
function(bid, npc, player)

	--ButtonID 2 取鸡肉
	if bid == 2 then
		local itemnumber = player:CountItem(13)
		if itemnumber < 1 then
			player:ShowQuestDlg(npc, 0, 1)
		else
			player:GetQuest():SetQuestStep(0, 2)
			player:SyncQuestData(0)
			player:ShowQuestDlg(npc, 0, 2)
		end
	--ButtonID 3 送5小红
	elseif bid == 3 then
		local leftbag = player:GetBagEmptySum()

		if leftbag < 5 then
			player:ShowQuestDlg(npc, 0, 3)
		else
			player:ClearItem(13, 1)
			player:AddItem(398)
			player:AddItem(398)
			player:AddItem(398)
			player:AddItem(398)
			player:AddItem(398)
			player:GetQuest():SetQuestStep(0, 4)
			player:SyncQuestData(0)
			player:ShowQuestDlg(npc, 0, 4)
		end

	--ButtonID 4 取鹿肉
	elseif bid == 4 then
		local itemnumber = player:CountItem(1)
		if itemnumber < 1 then
			player:ShowQuestDlg(npc, 0, 5)
		else
			local leftbag = player:GetBagEmptySum()
			if leftbag < 1 then
				player:ShowQuestDlg(npc, 0, 6)
			else
				player:ClearItem(1, 1)
				player:AddItem(35)
				player:ShowQuestDlg(npc, 0, 7)
				player:GetQuest():SetQuestStep(0, 7)
				player:SyncQuestData(0)
			end
		end

	--ButtonID 5 送木料
	elseif bid == 5 then
		local leftbag = player:GetBagEmptySum()
		if(leftbag < 1) then
			player:ShowQuestDlg(npc, 0, 8)
		else
			player:AddItem(368)
			player:GetQuest():SetQuestStep(0, 9)
			player:SyncQuestData(0)
			player:ShowQuestDlg(npc, 0, 9)
		end

	--ButtonID 6 杀稻草人
	elseif bid == 6 then
		local leftbag = player:GetBagEmptySum()
		if(leftbag < 5) then
			player:ShowQuestDlg(npc, 0, 8)
		else
			player:AddItem(398)
			player:AddItem(398)
			player:AddItem(398)
			player:AddItem(398)
			player:AddItem(398)
			player:GetQuest():SetQuestStep(0, 11)
			player:SyncQuestData(0)
			player:ShowQuestDlg(npc, 0, 11)
		end

	elseif bid == 7 then
		local curcounter = player:GetQuest():GetQuestCounter(0)
		if curcounter >= 10 then
			player:ShowQuestDlg(npc, 0, 13)
			player:GetQuest():SetQuestStep(0, 13)
			player:SyncQuestData(0)
			player:GainExp(500)
			player:GetQuest():SetQuestCounter(0, 0)
		else
			player:ShowQuestDlg(npc, 0, 12)
		end

	--BID 8 接受杀猫任务
	elseif bid == 8 then
		player:ShowQuestDlg(npc, 0, 14)
		player:GetQuest():SetQuestStep(0, 14)
		player:SyncQuestData(0)

	--BID 9 检测是否完成杀蛇任务
	elseif bid == 9 then
		local curcounter = player:GetQuest():GetQuestCounter(0)
		if curcounter >= 10 then
			player:ShowQuestDlg(npc, 0, 15)
			player:GetQuest():SetQuestStep(0, 15)
			player:SyncQuestData(0)
			player:GainExp(1000)
			player:AddMoney(2000)
			player:GetQuest():SetQuestCounter(0, 0)
		else
			player:ShowQuestDlg(npc, 0, 12)
		end

	--BID 10 到7级了
	elseif bid == 10 then
		local level = player:GetLevel()
		if level >= 7 then
			player:ShowQuestDlg(npc, 0, 17)
			player:GetQuest():SetQuestStep(0, 17)
			player:SyncQuestData(0)

			local empty = player:GetBagEmptySum()

			if empty < 1 then
				player:ShowQuestDlg(npc, 0, 8)

			else
				local job = player:GetHeroJob()
				if job == 0 then
					player:AddItem(291)
				elseif job == 1 then
					player:AddItem(292)
				elseif job == 2 then
					player:AddItem(293)
				end
			end

		else
			player:ShowQuestDlg(npc, 0, 16)
		end

	--BID 11 学会第一个技能
	elseif bid == 11 then
		local job = player:GetHeroJob()
		local nextstep = 0
		if job == 0 then
			nextstep = 18
		elseif job == 1 then
			nextstep = 19
		elseif job == 2 then
			nextstep = 20
		end

		player:GetQuest():SetQuestStep(0, nextstep)
		player:SyncQuestData(0)
		player:ShowQuestDlg(npc, 0, nextstep)

	end

end
--	10000 铁匠 可接 questid 1 的任务 等级>=10
gNPCQuestTable[10000] =
function(npc, player)
	--	简单的显示对话框 我想买卖东西 打听骷髅精灵 离开
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10000] =
function(bid, npc, player)

	if bid == 2 then
	--	2:买卖东西
		--[[npc:ClearAllSellItem()
		npc:AddSellItem(31)
		npc:AddSellItem(32)
		npc:AddSellItem(33)
		npc:AddSellItem(35)
		npc:AddSellItem(61)
		npc:AddSellItem(81)]]
		player:ShowShopDlg(npc, 1)
	end

	--  BID 3 打听
	if bid == 3 then
		player:ShowQuestDlg(npc, 0, 21)

	-- BID 4 帮忙对话
	elseif bid == 4 then
		local level = player:GetLevel()
		if level < 7 then
			player:ShowQuestDlg(npc, 0, 22)
		else
			local curstep = player:GetQuest():GetQuestStep(0)

			if curstep == 18 or curstep == 19 or curstep == 20 then
				player:ShowQuestDlg(npc, 0, 23)
			elseif curstep < 18 then
				player:ShowQuestDlg(npc, 0, 22)
			elseif curstep > 20 and curstep <= 26 then
				player:ShowQuestDlg(npc, 0, curstep)

				if curstep == 26 then
					player:GetQuest():SetQuestStep(0, 27)
					player:SyncQuestData(0)
					player:GainExp(800)
					player:AddMoney(1000)
					--player:SetQuest():SetQuestStep(1, 1)
				end
			elseif curstep == 27 then
				player:ShowQuestDlg(npc, 0, 27)

			elseif curstep == 28 then
				player:ShowQuestDlg(npc, 0, 28)

			end
		end

	-- BID 5 接任务
	elseif bid == 5 then
		player:ShowQuestDlg(npc, 0, 24)
		player:GetQuest():SetQuestStep(0, 25)
		player:SyncQuestData(0)

	elseif bid == 6 then
		--	提升武器
		local curstep = player:GetQuest():GetQuestStep(0)
		if curstep == 27 then
			player:GetQuest():SetQuestStep(0, 28)
			player:SyncQuestData(0)
		end

	end

end

--	10001 渔夫
gNPCQuestTable[10001] =
function(npc, player)
	local step = player:GetQuest():GetQuestStep(0)
	if step == 9 then
		local itemnumber = player:CountItem(368)
		if itemnumber == 1 then
			player:ClearItem(368, 1)
			player:ShowQuestDlg(npc, 0, 1)
			player:GetQuest():SetQuestStep(0, 10)
			player:SyncQuestData(0)
			player:AddMoney(500)
		else
			player:ShowQuestDlg(npc, 0, 0)
		end
	else
		player:ShowQuestDlg(npc, 0, 0)
	end
end

gNPCButtonTable[10001] =
function(bid, npc, player)
	if bid == 2 then
		player:ShowQuestDlg(npc, 0, 2)

	elseif bid == 3 then
		if player:GetLevel() >= 50 then
			player:FlyToMap(9, 67, 24)
			player:HideQuestDlg()
		else
			player:ShowQuestDlg(npc, 0, 3)
		end
	end
end

--	10006 仓库老板
gNPCQuestTable[10006] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10006] =
function(bid, npc, player)
	if bid == 2 then
		local money = player:GetMoney()
		if money > 500 then
			player:MinusMoney(500)
			player:HideQuestDlg()
			player:ShowShopDlg(npc, 2)
		else
			player:ShowQuestDlg(npc, 0, 1)
		end

	end
end


--	10003 小贩
gNPCQuestTable[10003] =
function(npc, player)
	local curstep = player:GetQuest():GetQuestStep(1)
	player:ShowQuestDlg(npc, 1, 0)

end

gNPCButtonTable[10003] =
function(bid, npc, player)

	local curstep = player:GetQuest():GetQuestStep(1)
	local prestep = player:GetQuest():GetQuestStep(0)
	--MirLog("Current step:"..curstep)

	if bid == 2 then
		--[[npc:ClearAllSellItem()
		npc:AddSellItem(394)
		npc:AddSellItem(398)
		npc:AddSellItem(400)
		npc:AddSellItem(396)
		npc:AddSellItem(395)
		npc:AddSellItem(399)
		npc:AddSellItem(401)
		npc:AddSellItem(397)
		npc:AddSellItem(404)
		npc:AddSellItem(402)
		npc:AddSellItem(403)
		npc:AddSellItem(405)]]
		player:ShowShopDlg(npc, 1)

	elseif bid == 3 then

		if curstep == 0 then
			if prestep >= 27 then
				player:ShowQuestDlg(npc, 1, 1)
			else
				player:ShowQuestDlg(npc, 1, 2)
			end

		elseif curstep == 1 then
			player:ShowQuestDlg(npc, 1, 3)

		elseif curstep == 2 then
			player:ShowQuestDlg(npc, 1, 4)

		elseif curstep == 3 then
			player:ShowQuestDlg(npc, 1, 6)

		elseif curstep == 4 then
			player:ShowQuestDlg(npc, 1, 7)

		elseif curstep == 5 or curstep == 6 then
			player:ShowQuestDlg(npc, 1, 9)

		else
			player:ShowQuestDlg(npc, 1, 12)

		end

	elseif bid ==4 then
		player:GetQuest():SetQuestStep(1, 1)
		player:SyncQuestData(1)
		player:ShowQuestDlg(npc, 1, 3)
		player:AddItem(402)

	elseif bid == 5 then
		player:FlyToMap(80, 80, 1)
		player:GetQuest():SetQuestStep(1, 2)
		player:SyncQuestData(1)
		player:HideQuestDlg()

	elseif bid == 6 then
		local itemsum = player:CountItem(1093)
		local emptybag = player:GetBagEmptySum()

		if itemsum >= 1 then
			if emptybag >= 3 then
				player:ClearItem(1093, itemsum)
				--player:AddItem(401)
				player:AddItem(400)
				player:AddItem(400)
				player:AddItem(400)
				player:AddItem(400)
				player:AddItem(400)
				player:AddItem(400)
				--player:AddItem(401)
				--player:AddItem(403)
				player:AddItem(402)
				player:AddItem(402)
				player:AddItem(402)
				player:GetQuest():SetQuestStep(1, 3)
				player:SyncQuestData(1)
				player:ShowQuestDlg(npc, 1, 6)
			else
				player:ShowQuestDlg(npc, 1, 5)
			end
		end

	elseif bid == 7 then
		player:GetQuest():SetQuestStep(1, 4)
		player:SyncQuestData(1)
		player:ShowQuestDlg(npc, 1, 7)

	elseif bid == 8 then
		local itemsum = player:CountItem(254)
		if itemsum >= 1 then
			player:ClearItem(254, itemsum)
			player:AddItem(257)
			player:GetQuest():SetQuestStep(1, 5)
			player:SyncQuestData(1)
			player:ShowQuestDlg(npc, 1, 9)
		else
			player:ShowQuestDlg(npc, 1, 8)
		end

	elseif bid == 9 then
		if curstep == 5 then
			player:ShowQuestDlg(npc, 1, 10)
		elseif curstep == 6 then
			player:ShowQuestDlg(npc, 1, 11)
			player:GetQuest():SetQuestStep(1, 7)
			player:SyncQuestData(1)

		end

	end
end







-- OnMonsterDead
function OnMonsterDead(monster, player)
end

--	InitNPC
function InitNPC(npcid, npc)

	if npcid == 10003 then
		npc:ClearAllSellItem()
		npc:AddSellItem(394)
		npc:AddSellItem(398)
		npc:AddSellItem(400)
		npc:AddSellItem(396)
		--npc:AddSellItem(395)
		--npc:AddSellItem(399)
		--npc:AddSellItem(401)
		--npc:AddSellItem(397)
		npc:AddSellItem(404)
		npc:AddSellItem(402)
		--npc:AddSellItem(403)
		--npc:AddSellItem(405)
		--npc:AddSellItem(313)
		--npc:AddSellItem(314)
		npc:AddSellItem(28)
		npc:AddSellItem(29)

	elseif npcid == 10000 then
		npc:ClearAllSellItem()
		npc:AddSellItem(31)
		npc:AddSellItem(32)
		npc:AddSellItem(33)
		npc:AddSellItem(35)
		npc:AddSellItem(61)
		npc:AddSellItem(81)

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
