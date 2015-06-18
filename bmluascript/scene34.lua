-- 血色魔窟

--服务器载入地图后调用 用于创建地图的NPC和怪物
function OnMapLoaded(scene)

	--	随机数
	math.randomseed(tostring(os.time()):reverse():sub(1, 6))

	scene:SetAutoReset()
	scene:SetCannotStay()

	scene:CreateDoorEvent(33, 40, 134, 288, 175)

	scene:CreateMonster(121, 75, 75, 75, 2, 24000)
	scene:CreateMonster(123, 75, 75, 75, 3, 12000)
	scene:CreateMonster(119, 75, 75, 75, 6, 6000)
	scene:CreateMonster(120, 75, 75, 75, 6, 6000)

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
	--MirLog("dead"..monsterid)
	local monsterid = monster:GetMonsterID()
	if monsterid == 121 then
		local scene = player:GetLocateScene()

		--MirLog("here 39")

		if nil ~= scene then
			--MirLog("here 42")

			if math.random(3) == 1 then
				--MirLog("here 45")

				if scene:GetMonsterSum(125) == 0 then
					--MirLog("here 48")

					local xpos = player:GetCoordX()
					local ypos = player:GetCoordY()

					scene:CreateMonster(125, 69, 72)
					player:SendSystemMessage("野兽已被唤醒")

				end

			end

		end

	end
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
