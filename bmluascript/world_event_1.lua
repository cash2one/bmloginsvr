--	世界事件1 刷宝箱 cron:0 0 */1 * * *
local worldEventId = 1
local worldEventHandler = class("WorldEventHandler")
worldEventHandler.id = worldEventId

--	事件回调
function worldEventHandler:OnRegistered()
	--	注册事件成功
end

function worldEventHandler:OnActive()
	GameSceneManager:GetInstance():SendSystemNotifyAllScene("[世界活动]珍藏着大量宝藏的藏宝箱出现在多个地图中，请大家速速去寻找夺取宝藏吧！")
end

local generateScenes = {1, 2, 4, 7, 8, 12, 22, 30, 38, 48, 55}

function worldEventHandler:OnUpdate()
	--	给所有的地图一个宝箱
	for _, v in ipairs(generateScenes) do
		local scene = GameSceneManager:GetInstance():GetSceneInt(v)
		local randNumber = math.random(3)
		if nil ~= scene and 1 == randNumber then
			if scene:GetMonsterSum(64) == 0 then
				scene:CreateLeaderMonster(64)
			end
		end
	end
	
	--	给随机一个地图一个大宝箱
	local randomMapId = math.random(#generateScenes)
	if randomMapId >= 1 and randomMapId <= #generateScenes then
		local mapId = generateScenes[randomMapId]
		local scene = GameSceneManager:GetInstance():GetSceneInt(mapId)
		if nil ~= scene then
			if scene:GetMonsterSum(137) == 0 then
				debug("大藏宝箱生成在地图:"..mapId)
				scene:CreateLeaderMonster(137)
			end
		end
	end
	
	return true
end

function worldEventHandler:OnFinish()
	
end

function worldEventHandler:OnLoginServerConnected()
	--	设置schedule
	local expr = "0 30 */1 * * *"
	if DEBUG then
		expr = "30 */1 * * * *"
	end
	GameWorld:GetInstancePtr():SetSchedule(worldEventId, expr)
end

return worldEventHandler