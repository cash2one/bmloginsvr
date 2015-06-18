-- 魔炼之地

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)
	--	门 龙神殿
	scene:CreateDoorEvent(37, 19, 20, 13, 32)

	--	随机数
	math.randomseed(tostring(os.time()):reverse():sub(1, 6))

	--	无法存档
	scene:SetCannotStay()
	scene:SetAutoReset()

	--	恶魔商人
	scene:CreateNPC(10019, 17, 23)

	--	怪物 暗之弓箭手
	scene:CreateMonsterLater(47, 50, 50, 50, 15, 3 * 60 * 1000)
	--	暗之雕像
	scene:CreateMonsterLater(48, 50, 50, 50, 10, 3 * 60 * 1000)
	--	暗之卫士
	scene:CreateMonsterLater(50, 50, 50, 50, 10, 3 * 60 * 1000)
	--	暗之教主
	scene:CreateMonsterLater(129, 50, 50, 50, 1, 4 * 60 * 1000)
	--	qi e
	scene:CreateMonsterLater(128, 50, 50, 50, 15, 2 * 60 * 1000)
	--	小白
	scene:CreateMonsterLater(38, 50, 50, 50, 5, 3 * 60 * 1000)
	--	暗之虹魔教主
	scene:CreateMonsterLater(130, 50, 50, 50, 1, 8 * 60 * 1000)

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
	player:SendSystemMessage("封魔大陆居然还有这样邪恶的地方，勇士小心了")
end

function GetVersion()
end

function InitNPC(npcid, npc)

	if npcid == 10019 then
		npc:ClearAllSellItem()
		npc:AddSellItem(402)
		npc:AddSellItem(404)
		npc:AddSellItem(260)
		npc:AddSellItem(28)
		npc:AddSellItem(29)
		--npc:AddSellItem(393)
		npc:AddSellItem(550)
		npc:AddSellItem(551)
		npc:AddSellItem(560)
		npc:AddSellItem(561)
	end

end

--	10007	神秘商人
gNPCQuestTable[10019] =
function(npc, player)
	player:ShowQuestDlg(npc, 0, 0)
end

gNPCButtonTable[10019] =
function(bid, npc, player)
	if bid == 3 then
		player:ShowShopDlg(npc, 1)
		player:HideQuestDlg()
	elseif bid == 2 then
		local money = player:GetMoney()
		if money > 10000 then
			player:MinusMoney(10000)
			player:HideQuestDlg()
			player:ShowShopDlg(npc, 2)
		else
			player:ShowQuestDlg(npc, 0, 1)
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
