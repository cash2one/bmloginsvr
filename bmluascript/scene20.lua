--	封魔密道 脚本
--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 封魔大陆
	scene:CreateDoorEvent(8, 111, 109, 105, 109)
	--	门 牛魔大殿
	scene:CreateDoorEvent(21, 8, 81, 46, 16)

	--	小怪
	scene:CreateMonster(67, 43, 66, 40, 10, 60000)
	scene:CreateMonster(68, 43, 66, 40, 10, 60000)
	scene:CreateMonster(69, 43, 66, 40, 10, 60000)

	scene:CreateMonster(67, 69, 64, 40, 10, 60000)
	scene:CreateMonster(68, 69, 64, 40, 10, 60000)
	scene:CreateMonster(69, 69, 64, 40, 10, 60000)

	--	将军
	scene:CreateMonster(71, 71, 65, 10, 1, 60000)
	--	祭祀
	scene:CreateMonster(73, 15, 84, 10, 1, 60000)

	--	虹魔猪卫
	scene:CreateMonsterLater(81, 71, 63, 10, 1, 240000)
	--	蝎卫
	scene:CreateMonsterLater(17, 14, 87, 5, 1, 240000)

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
