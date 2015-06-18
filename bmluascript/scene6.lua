--服务器载入地图后调用 用于创建地图的NPC和怪物
--	沃玛大殿脚本
function OnMapLoaded(scene)
	--	门 通往沃玛大厅
	scene:CreateDoorEvent(5, 14, 20, 12, 31)
	-- 通往连接通道门
	scene:CreateDoorEvent(7, 93, 101, 20, 169)


	--	沃玛战士
	scene:CreateMonster(30, 60, 60, 50, 12, 60000)
	--	火焰沃玛
	scene:CreateMonster(31, 60, 60, 50, 6, 120000)
	--	沃玛勇士
	scene:CreateMonster(32, 60, 60, 50, 8, 120000)
	--	沃玛战将
	scene:CreateMonster(33, 60, 60, 50, 8, 120000)
	--	沃玛教主
	scene:CreateMonsterLater(34, 60, 60, 1, 1, 240000)
	--	沃玛护卫
	scene:CreateMonsterLater(35, 60, 62, 1, 1, 160000)

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

	if player:GetQuest():GetQuestStep(1) == 19 and monsterid == 34 then
		local emptybag = player:GetBagEmptySum()
		if emptybag >= 1 then
			if math.random(3) == 1 then
				player:AddItem(627)
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
