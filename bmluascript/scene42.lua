-- 挑战地图
local sceneref = nil

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	sceneref = scene

	--	无法存档
	scene:SetCannotStay()
	scene:SetAutoReset()

	--	副本NPC
	scene:CreateNPC(10020, 37, 21)

	--	注册事件 场景更新
	scene:RegisterCallback(SceneEvent_Update, "OnSceneUpdate", 5000)

	scene:GetLuaEngine():LoadModule("module_copy")
	onInitCopy(scene)
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

function OnSceneUpdate(_scene)
	clg_update(_scene)
end

function GetVersion()
end

--	10020
gNPCQuestTable[10020] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10020] =
function(bid, npc, player)
	if bid == 2 then
		--	离开
		player:FlyToHome()
		player:HideQuestDlg()
	elseif bid == 3 then
		--	开始挑战
		if clg_isBegin() then
			player:ShowQuestDlg(npc, 0, 2)
		else
			clg_setBegin(true)
			player:ShowQuestDlg(npc, 0, 1)

			--	开始挑战
		end
	end
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



--------------------------------------------------------------
--	副本回调
function onInitCopy(_scene)
	clg_setScene(_scene)
	clg_setRoundMax(3)
	clg_setRewardFunc(onReward)
	clg_setNextRoundFunc(onNextRound)
	clg_setTimeout(12 * 60)
end

function onReward()
	local maxItems = sceneref:GetPlayerSum()
	--local items = math.random(1, maxItems)
	local items = maxItems / 2
	if items == 0 then
		items = 1
	end

	local rewards = {265, 265, 265, 265, 265, 783, 783,
					783, 783, 117, 783, 864, 865, 866, 265,
					265, 265, 265, 265, 265, 265, 265, 265,
					265, 265, 265, 265, 265, 265, 265, 265,
					265, 265, 265, 265, 265, 265, 265, 265,
					265, 265, 265, 265, 265, 265, 265, 265,
					265, 265, 265, 265, 265, 265, 265, 265,
					265, 265, 265, 265, 265, 265, 265, 265}

	if items > maxItems then
		items = 1
	end

	for i = 1, items do
		local itemid = rewards[math.random(1, #rewards)]
		sceneref:ChallengeSuccess(itemid)
	end

	sceneref:CreateMonster(137, 26, 21)
	sceneref:BroadcastSceneSystemMessage("打开藏宝盒领取奖励")
end

function onNextRound(_round)
	local playerNum = sceneref:GetPlayerSum()
	local monsAddNum = playerNum / 2

	if sceneref:GetChallengeID() == 1 then
		--	第一波
		if 1 == _round then
			for i = 1, (4 + monsAddNum) do
				--	魔龙刀兵
				sceneref:CreateMonster(83)
			end
			for i = 1, (3 + monsAddNum) do
				--	魔龙破甲兵
				sceneref:CreateMonster(84)
			end
			for i = 1, (3 + monsAddNum) do
				--	魔龙射手
				sceneref:CreateMonster(85)
			end
			sceneref:CreateMonster(86)
		elseif 2 == _round then
			for i = 1, (3 + monsAddNum) do
				--	黑狐
				sceneref:CreateMonster(94)
			end
			for i = 1, (3 + monsAddNum) do
				--	赤狐
				sceneref:CreateMonster(95)
			end
			for i = 1, (1 + monsAddNum) do
				--	赤狐
				sceneref:CreateLeaderMonster(95, 26, 22)
			end
			for i = 1, (1 + monsAddNum) do
				--	素狐
				sceneref:CreateLeaderMonster(96, 26, 22)
			end
			for i = 1, (1 + monsAddNum) do
				--	素狐
				sceneref:CreateMonster(96)
			end
			if 3 == monsAddNum then
				sceneref:CreateLeaderMonster(98, 26, 22)
			elseif 2 == monsAddNum or 1 == monsAddNum then
				sceneref:CreateEliteMonster(98, 26, 22)
			else
				sceneref:CreateMonster(98, 26, 22)
			end
		elseif 3 == _round then
			for i = 1, (2 + monsAddNum) do
				--	白虎
				sceneref:CreateMonster(116)
			end
			for i = 1, (2 + monsAddNum) do
				--	法老
				sceneref:CreateEliteMonster(102, 26, 21)
			end
			for i = 1, (2 + monsAddNum) do
				--	法老
				sceneref:CreateLeaderMonster(102, 26, 21)
			end
			if 3 == monsAddNum then
				sceneref:CreateLeaderMonster(100, 26, 22)
			elseif 2 == monsAddNum or 1 == monsAddNum then
				sceneref:CreateEliteMonster(100, 26, 22)
			else
				sceneref:CreateMonster(100, 26, 22)
			end
		end
	else
	end
end
