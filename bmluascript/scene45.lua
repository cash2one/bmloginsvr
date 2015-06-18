-- 地狱神殿 挑战地图

local sceneref = nil

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	注册事件 玩家进入
	--scene:RegisterCallback(SceneEvent_PlayerEnter, "OnPlayerEnterScene", 0)
	sceneref = scene

	--	无法存档
	scene:SetCannotStay()
	scene:SetAutoReset()

	--	副本NPC
	scene:CreateNPC(10020, 87, 159)

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

function OnPlayerEnterScene(scene, player)
	player:SendSystemMessage("这里是封魔大陆的深处，到底隐藏着什么?")
end

function GetVersion()
end

--	副本更新
function OnSceneUpdate(_scene)
	clg_update(_scene)
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
	clg_setTimeout(22 * 60)
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
					117, 117, 117, 117, 117, 117, 117, 117,
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

	sceneref:CreateMonster(137, 93, 96)
	sceneref:BroadcastSceneSystemMessage("打开藏宝盒领取奖励")
end

function onNextRound(_round)
	local playerNum = sceneref:GetPlayerSum()
	local monsAddNum = playerNum / 2

	if sceneref:GetChallengeID() == 2 then
		--	第一波
		if 1 == _round then
			GameSceneManager:GetInstance():SendSystemNotifyAllScene("英雄们开始了地狱神殿的远征，加油吧！")
			for i = 1, (2 + monsAddNum) do
				--	阎魔御卫
				sceneref:CreateMonster(139)
			end
		elseif 2 == _round then
			for i = 1, (1 + monsAddNum) do
				--	阎魔御卫
				sceneref:CreateMonster(139)
			end
			if 3 == monsAddNum then
				sceneref:CreateLeaderMonster(114, 96, 90)
			elseif 2 == monsAddNum or 1 == monsAddNum then
				sceneref:CreateEliteMonster(114, 96, 90)
			else
				sceneref:CreateMonster(114, 96, 90)
			end
		elseif 3 == _round then
			GameSceneManager:GetInstance():SendSystemNotifyAllScene("地狱神殿深处的阎魔太子已被唤醒！")
			sceneref:CreateMonster(138, 96, 90)
		end
	else
		MirLog("Invalid challenge id "..sceneref:GetChallengeID())
	end
end
