--	玲珑仙岛 副本脚本

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--scene:SetInstance(true)
	scene:SetTreasureMap(true)
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
	player:GetLocateScene():SetKilledMonster()
end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	return true
end

function GetVersion()
end

function OnStartInstance(scene)
	-- 黄泉教主
	scene:CreateMonster(65, 37, 24)

	scene:CreateMonster(20)
	scene:CreateMonster(20)
	scene:CreateMonster(20)
	scene:CreateMonster(20)
	scene:CreateMonster(20)
	scene:CreateMonster(20)
	scene:CreateMonster(20)
	scene:CreateMonster(20)
	scene:CreateMonster(46)
	scene:CreateMonster(46)
	scene:CreateMonster(46)
	scene:CreateMonster(46)
	scene:CreateMonster(46)
	scene:CreateMonster(46)
	scene:CreateMonster(46)
	scene:CreateMonster(46)
	scene:CreateMonster(49)
	scene:CreateMonster(49)
	scene:CreateMonster(49)
	scene:CreateMonster(49)
	scene:CreateMonster(49)
	scene:CreateMonster(49)
	scene:CreateMonster(49)
	scene:CreateMonster(49)
	scene:CreateMonster(45, 37, 24)
end

function GiveReward(scene)
	scene:CreateMonster(64, 20, 20)
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
