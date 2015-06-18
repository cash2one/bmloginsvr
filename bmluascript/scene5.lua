--	沃玛神殿一层脚本
--服务器载入地图后调用 用于创建地图的NPC和怪物

function OnMapLoaded(scene)
	--	骷髅洞二层的门
	scene:CreateDoorEvent(3, 60, 61, 35, 17)
	--	通往沃玛大殿
	scene:CreateDoorEvent(6, 12, 31, 14, 20)
	--	沃玛战士
	scene:CreateMonster(30, 50, 50, 50, 12, 60000)
	--	火焰沃玛
	scene:CreateMonster(31, 50, 50, 50, 6, 120000)
	--	沃玛勇士
	scene:CreateMonster(32, 50, 50, 50, 8, 120000)
	--	沃玛战将
	scene:CreateMonster(33, 50, 50, 50, 8, 120000)
	--	沃玛教主
	--scene:CreateMonsterLater(34, 16, 38, 1, 1, 180000)
	--	沃玛护卫
	scene:CreateMonsterLater(35, 41, 53, 1, 1, 160000)

	--	神秘商人
	scene:CreateNPC(10007, 65, 50)
	--	自动重置怪物HP
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
end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	return true
end


function GetVersion()
end

function InitNPC(npcid, npc)

	if npcid == 10007 then
		npc:ClearAllSellItem()
		npc:AddSellItem(402)
		npc:AddSellItem(165)
		npc:AddSellItem(163)
		npc:AddSellItem(161)
		npc:AddSellItem(62)
		npc:AddSellItem(63)
		npc:AddSellItem(64)
		npc:AddSellItem(82)
		npc:AddSellItem(83)
		npc:AddSellItem(84)
		npc:AddSellItem(47)
		npc:AddSellItem(49)
		npc:AddSellItem(37)
		npc:AddSellItem(278)
		npc:AddSellItem(396)
		npc:AddSellItem(400)
	end

end






--	10007	神秘商人
gNPCQuestTable[10007] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10007] =
function(bid, npc, player)
	if bid == 3 then
		--[[npc:ClearAllSellItem()
		npc:AddSellItem(403)
		npc:AddSellItem(165)
		npc:AddSellItem(163)
		npc:AddSellItem(161)
		npc:AddSellItem(62)
		npc:AddSellItem(63)
		npc:AddSellItem(64)
		npc:AddSellItem(82)
		npc:AddSellItem(83)
		npc:AddSellItem(84)
		npc:AddSellItem(47)
		npc:AddSellItem(49)
		npc:AddSellItem(37)
		npc:AddSellItem(278)]]
		player:ShowShopDlg(npc, 1)
		player:HideQuestDlg()

	elseif bid == 2 then
		player:ShowQuestDlg(npc, 0, 1)

	elseif bid == 4 then
		player:ShowQuestDlg(npc, 0, 2)

	elseif bid == 5 then
		local itemcount = player:CountItem(412)
		if itemcount >= 10 then
			player:ClearItem(412, 10)
			player:AddItem(41)
			player:ShowQuestDlg(npc, 0, 4)
		else
			player:ShowQuestDlg(npc, 0, 3)
		end

	elseif bid == 6 then
		local itemcount = player:CountItem(412)
		if itemcount >= 10 then
			player:ClearItem(412, 10)
			player:AddItem(42)
			player:ShowQuestDlg(npc, 0, 4)
		else
			player:ShowQuestDlg(npc, 0, 3)
		end

	elseif bid == 7 then
		local itemcount = player:CountItem(412)
		if itemcount >= 10 then
			player:ClearItem(412, 10)
			player:AddItem(38)
			player:ShowQuestDlg(npc, 0, 4)
		else
			player:ShowQuestDlg(npc, 0, 3)
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
