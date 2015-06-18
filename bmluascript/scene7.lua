--服务器载入地图后调用 用于创建地图的NPC和怪物
--	连接通道脚本
function OnMapLoaded(scene)
	-- 通往沃玛大殿门
	scene:CreateDoorEvent(6, 20, 169, 93, 101)
	-- 通往封魔大陆门
	scene:CreateDoorEvent(8, 188, 186, 17, 27)

	-- 怪物 僵尸 猪
	scene:CreateMonster(36, 95, 100, 50, 15, 60000)
	scene:CreateMonster(37, 95, 100, 50, 15, 60000)
	scene:CreateMonsterLater(38, 95, 100, 50, 2, 600000)
	scene:CreateMonsterLater(27, 95, 100, 50, 4, 600000)
	scene:CreateMonsterLater(115, 95, 100, 50, 1, 300000)
	scene:CreateMonster(26, 95, 100, 50, 8, 60000)
	scene:CreateMonster(25, 95, 100, 50, 8, 60000)
	scene:CreateMonster(24, 95, 100, 50, 8, 60000)
	scene:CreateMonster(23, 95, 100, 50, 8, 60000)

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

	if player:GetQuest():GetQuestStep(1) == 19 and monsterid == 38 then
		local emptybag = player:GetBagEmptySum()
		if emptybag >= 1 then
			if math.random(3) == 1 then
				player:AddItem(628)
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
