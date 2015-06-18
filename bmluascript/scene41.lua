-- 月氏天宫

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 五行之巅
	--scene:CreateDoorEvent(32, 10, 134, 107, 22)

	--	门 月氏之路
	scene:CreateDoorEvent(40, 54, 48, 16, 19)

	scene:CreateMonster(132, 35, 35, 35, 8, 60000)
	scene:CreateMonster(133, 35, 35, 35, 2, 60000)
	scene:CreateMonster(134, 35, 35, 35, 4, 60000)

	scene:CreateMonsterLater(136, 42, 39, 4, 1, 180000)

	--	无法存档
	scene:SetCannotStay()
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
