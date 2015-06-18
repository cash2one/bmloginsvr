-- 死亡深渊

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 魔炼之地
	scene:CreateDoorEvent(36, 193, 124, 84, 85)

	--	随机数
	math.randomseed(tostring(os.time()):reverse():sub(1, 6))

	--	无法存档
	scene:SetCannotStay()
	scene:SetAutoReset()

	--	蜈蚣
	scene:CreateMonster(39, 100, 100, 100, 10, 30 * 1000)
	--	黑色eq
	scene:CreateMonster(40, 100, 100, 100, 7, 30 * 1000)
	--	跳跳蜂
	scene:CreateMonster(41, 100, 100, 100, 10, 30 * 1000)
	--	蠕虫
	scene:CreateMonster(42, 100, 100, 100, 7, 30 * 1000)
	--	钳虫
	scene:CreateMonster(43, 100, 100, 100, 10, 30 * 1000)
	--	巨型钳虫
	scene:CreateMonsterLater(44, 100, 100, 100, 4, 2 * 60 * 1000)
	--	触龙神
	scene:CreateMonsterLater(19, 100, 100, 100, 1, 3 * 60 * 1000)
	--	暗之触龙神
	scene:CreateMonsterLater(131, 77, 170, 5, 1, 6 * 60 * 1000)

	--	注册事件 玩家进入
	scene:RegisterCallback(SceneEvent_PlayerEnter, "OnPlayerEnterScene", 0)

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

function OnPlayerEnterScene(scene, player)
	--player:SendSystemMessage("[系统]这里是封魔大陆的深处，到底隐藏着什么?")
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
