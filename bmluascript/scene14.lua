-- 沉睡之地脚本

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 赤月峡谷西
	scene:CreateDoorEvent(13, 21, 26, 87, 37)

	--	双头金刚
	scene:CreateMonsterLater(58, 16, 16, 16, 1, 240000)
	--	双头血魔
	scene:CreateMonsterLater(59, 16, 16, 16, 1, 240000)
	--	幻影蜘蛛
	scene:CreateMonster(52, 15, 18, 1, 1, 90000)
	--	月魔
	scene:CreateMonster(54, 16, 16, 16, 8, 120000)
	--	花吻蜘蛛
	scene:CreateMonster(55, 16, 16, 16, 8, 120000)
	--	黑锷蜘蛛
	scene:CreateMonster(56, 16, 16, 16, 8, 120000)

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
