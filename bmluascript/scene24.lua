-- 狐月岛

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	门 凌霄天界
	scene:CreateDoorEvent(25, 54, 11, 14, 53)

	--	门 地狱之路
	scene:CreateDoorEvent(28, 43, 21, 66, 16)

	--	老兵
	scene:CreateNPC(10013, 8, 67)
	--	杂货商人
	scene:CreateNPC(10014, 45, 38)
	--	仓库老板
	scene:CreateNPC(10015, 38, 45)

	--	设置城市区域
	scene:SetCityRange(35, 32, 20, 20, 45, 42)


	--	各种狐狸
	--[[scene:CreateMonster(94, 33,63, 80, 4, 60000)
	scene:CreateMonster(95, 33,63, 80, 4, 60000)
	scene:CreateMonster(96, 33,63, 80, 4, 60000)

	scene:CreateMonster(94, 47,39, 80, 4, 60000)
	scene:CreateMonster(95, 47,39, 80, 4, 60000)
	scene:CreateMonster(96, 47,39, 80, 4, 60000)

	scene:CreateMonster(94, 56,19, 80, 4, 60000)
	scene:CreateMonster(95, 56,19, 80, 4, 60000)
	scene:CreateMonster(96, 56,19, 80, 4, 60000)]]

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
end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	--return true

	if x == 43 and y == 21 then

		if player:GetLevel() < 50 then
			player:SendSystemMessage("等修炼到50级再来吧")
			return false
		else
			return true
		end

	else

		return true

	end

end

function GetVersion()
end









--	老兵
--	10001 渔夫
gNPCQuestTable[10013] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10013] =
function(bid, npc, player)
	if bid == 2 then
		local curstep = player:GetQuest():GetQuestStep(1)

		if curstep == 21 then
			local itemcount = player:CountItem(690)
			if itemcount == 1 then
				player:ClearItem(690, itemcount)
				player:GainExp(20000)
				player:ShowQuestDlg(npc, 0, 2)
				player:GetQuest():SetQuestStep(1, 22)
				player:SyncQuestData(1)

			else
				player:ShowQuestDlg(npc, 0, 1)
			end

		elseif curstep == 22 then

			player:ShowQuestDlg(npc, 0, 2)

		elseif curstep == 23 then
			player:ShowQuestDlg(npc, 0, 3)

		elseif curstep == 24 then
			player:ShowQuestDlg(npc, 0, 5)

		else
			player:ShowQuestDlg(npc, 0, 1)
		end

	--	杀50只怪物
	elseif bid == 3 then
		local mons = player:GetQuest():GetQuestCounter(1)
		if mons == 50 then
			player:GainExp(1000000)
			player:AddMoney(100000)
			player:GetQuest():SetQuestCounter(1, 0)
			player:GetQuest():SetQuestStep(1, 23)
			player:SyncQuestData(1)
			player:ShowQuestDlg(npc, 0, 3)

		else
			player:ShowQuestDlg(npc, 0, 4)
		end

	--	得到熔岩之心
	elseif bid == 4 then
		local items = player:CountItem(1224)
		if items == 1 then
			player:GainExp(30000)
			player:AddMoney(30000)
			player:GetQuest():SetQuestStep(1, 24)
			player:SyncQuestData(1)
			player:ShowQuestDlg(npc, 0, 5)
		else
			player:ShowQuestDlg(npc, 0, 6)
		end

	--	回落霞村
	elseif bid == 5 then
		player:FlyToMap(56, 28, 0)
		player:HideQuestDlg()

	end
end


--	10014 杂货商人
gNPCQuestTable[10014] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10014] =
function(bid, npc, player)

	if bid == 3 then
		player:ShowShopDlg(npc, 1)
	end

end

--	10015 仓库老板
gNPCQuestTable[10015] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10015] =
function(bid, npc, player)

	if bid == 3 then

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





--	InitNPC
function InitNPC(npcid, npc)

	if npcid == 10014 then
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
		npc:AddSellItem(123)
		npc:AddSellItem(124)

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
