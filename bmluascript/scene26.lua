-- 凌霄殿

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	门 凌霄天界
	scene:CreateDoorEvent(25, 4, 54, 35, 30)

	--	各种狐狸
	--[[scene:CreateMonster(94, 27, 21, 80, 4, 60000)
	scene:CreateMonster(95, 27, 21, 80, 4, 60000)
	scene:CreateMonster(96, 27, 21, 80, 4, 60000)

	scene:CreateMonster(94, 34, 38, 80, 4, 60000)
	scene:CreateMonster(95, 34, 38, 80, 4, 60000)
	scene:CreateMonster(96, 34, 38, 80, 4, 60000)

	scene:CreateMonster(94, 52, 26, 80, 4, 60000)
	scene:CreateMonster(95, 52, 26, 80, 4, 60000)
	scene:CreateMonster(96, 52, 26, 80, 4, 60000)]]

	--	BOSS 天界守护神
	scene:CreateMonsterLater(98, 40, 14, 5, 1, 300000)

	--	小怪
	scene:CreateMonster(94, 16, 39, 25, 3, 60000)
	scene:CreateMonster(95, 16, 39, 25, 3, 60000)
	scene:CreateMonster(96, 16, 39, 25, 3, 60000)

	scene:CreateMonster(94, 16, 26, 25, 3, 60000)
	scene:CreateMonster(95, 16, 26, 25, 3, 60000)
	scene:CreateMonster(96, 16, 26, 25, 3, 60000)

	scene:CreateMonster(94, 33, 26, 25, 3, 60000)
	scene:CreateMonster(95, 33, 26, 25, 3, 60000)
	scene:CreateMonster(96, 33, 26, 25, 3, 60000)

	scene:CreateMonster(94, 33, 40, 25, 3, 60000)
	scene:CreateMonster(95, 33, 40, 25, 3, 60000)
	scene:CreateMonster(96, 33, 40, 25, 3, 60000)

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
