--	兽人古墓一层脚本文件

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	落霞山谷门
	scene:CreateDoorEvent(1, 18, 101, 268, 107)
	--	兽人古墓二层门
	scene:CreateDoorEvent(3, 86, 14, 37, 82)

	--	初始怪物 骷髅等
	scene:CreateMonster(11, 40, 103, 50, 5, 60000)
	scene:CreateMonster(4, 40, 103, 50, 5, 60000)
	scene:CreateMonster(5, 40, 103, 50, 5, 60000)
	scene:CreateMonster(6, 40, 103, 50, 5, 60000)
	scene:CreateMonster(7, 40, 103, 50, 5, 60000)
	scene:CreateMonster(7, 64, 46, 50, 3, 60000)
	scene:CreateMonster(6, 64, 46, 50, 3, 60000)
	scene:CreateMonster(5, 64, 46, 50, 3, 60000)
	scene:CreateMonster(4, 64, 46, 50, 3, 60000)
end

--2秒钟调用一次 用来更新场景信息
function OnUpdateScene(scene)

end

-- CanOpenDoor 是否能进门
function CanOpenDoor(x, y, player)
	return true
end

function OnMonsterDead(monster, player)
end

function GetVersion()
end

--NPC任务
gNPCQuestTable = {}
gNPCButtonTable = {}








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
