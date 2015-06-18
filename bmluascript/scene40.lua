-- 天堂之路

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 五行之巅
	scene:CreateDoorEvent(32, 10, 134, 107, 22)
	--	门 月氏天宫
	scene:CreateDoorEvent(41, 16, 19, 54, 48)

	scene:CreateMonster(132, 20, 70, 50, 14, 60000)
	scene:CreateMonster(133, 20, 70, 50, 8, 60000)
	scene:CreateMonster(134, 20, 70, 50, 4, 60000)

	--[[scene:CreateMonster(132, 26, 67, 7, 5, 60000)
	scene:CreateMonster(133, 26, 67, 7, 3, 60000)
	scene:CreateMonster(134, 26, 67, 7, 2, 60000)

	scene:CreateMonster(132, 26, 120, 7, 5, 60000)
	scene:CreateMonster(133, 26, 120, 7, 3, 60000)
	scene:CreateMonster(134, 26, 120, 7, 2, 60000)]]

	scene:CreateMonsterLater(135, 26, 120, 7, 1, 200000)

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
