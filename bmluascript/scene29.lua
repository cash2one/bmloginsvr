-- 静之冰谷

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	门 封魔大陆
	--scene:CreateDoorEvent(8, 18, 56, 130,123)
	--	门 冰谷走廊
	scene:CreateDoorEvent(30, 18, 56, 29, 478)
	--	NPC 异界之门
	scene:CreateNPC(10018, 429, 28)

	scene:CreateMonster(113, 225, 225, 225, 20, 6000)
	scene:CreateMonster(109, 225, 225, 225, 15, 6000)
	scene:CreateMonster(110, 225, 225, 225, 15, 6000)
	scene:CreateMonster(107, 225, 225, 225, 15, 6000)
	--scene:CreateMonster(113, 339, 298, 100, 20, 6000)
	--scene:CreateMonster(109, 200, 275, 100, 20, 6000)
	--scene:CreateMonster(110, 84, 300, 100, 20, 6000)
	scene:CreateMonsterLater(103, 116, 115, 2, 1, 6000)
	scene:CreateMonsterLater(103, 397, 286, 2, 1, 6000)
	scene:CreateMonsterLater(111, 210, 210, 2, 1, 6000)

	scene:SetAutoReset()
	--scene:SetCannotStay()

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




--	10018 异界之门
gNPCQuestTable[10018] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10018] =
function(bid, npc, player)
	if bid == 3 then
		player:FlyToMap(115, 115, 8)
		player:HideQuestDlg()
	elseif bid == 2 then
		local lv = player:GetLevel()
		if lv < 55 then
			player:ShowQuestDlg(npc, 0, 1)
		else
			--player:ShowQuestDlg(npc, 0, 2)
			local bstep2 = player:GetQuest():GetQuestStep(52)
			if bstep2 == 0 then
				player:ShowQuestDlg(npc, 0, 2)
			elseif bstep2 == 1 then
				player:ShowQuestDlg(npc, 0, 3)
			end
		end
	elseif bid == 5 then
		player:GetQuest():SetQuestStep(52, 1)
		player:SyncQuestData(52)
		player:ShowQuestDlg(npc, 0, 3)
	elseif bid == 4 then
		local item1 = player:CountItem(784)
		local item2 = player:CountItem(785)
		local item3 = player:CountItem(786)
		local item4 = player:CountItem(787)
		local item5 = player:CountItem(788)
		local item6 = player:CountItem(789)
		local item7 = player:CountItem(790)

		if item1 >= 1 and item2 >= 1 and item3 >= 1 and item4 >= 1 and item5 >= 1 and item6 >= 1 and item7 >= 1 then
			player:ShowQuestDlg(npc, 0, 4)
			player:GetQuest():SetQuestStep(52, 0)
			player:ClearItem(784, 1)
			player:ClearItem(785, 1)
			player:ClearItem(786, 1)
			player:ClearItem(787, 1)
			player:ClearItem(788, 1)
			player:ClearItem(789, 1)
			player:ClearItem(790, 1)
			player:AddItem(1229)
			player:AddItem(1229)
			player:AddItem(1031)
			--player:AddItem(1229)
			--player:AddItem(117)
			player:AddItem(265)
			player:AddItem(265)
			player:AddItem(265)
			player:AddItem(265)
			player:AddItem(265)
			player:AddItem(265)
		else
			player:ShowQuestDlg(npc, 0, 5)
		end
	else
		player:ShowQuestDlg(npc, 0, 1)
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
