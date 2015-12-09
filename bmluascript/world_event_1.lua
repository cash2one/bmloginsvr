--	世界事件1 刷宝箱 cron:0 0 */1 * * *
local worldEventId = 1
local worldEventHandler = class("WorldEventHandler")
worldEventHandler.id = worldEventId

--	事件回调
function worldEventHandler:OnRegisterd()
	--	注册事件成功
end

function worldEventHandler:OnActive()
	GameSceneManager:GetInstance():SendNotifyAllScene("[世界活动]珍藏着大量宝藏的藏宝箱出现在多个地图中，请大家速速去寻找夺取宝藏吧！")
end

local generateScenes = {1, 8, 48}

function worldEventHandler:OnUpdate()
	for _, v in ipairs(generateScenes) do
		local scene = GameSceneManager:GetInstance():GetScene(v)
		if nil ~= scene then
			scene:CreateLeaderMonster(v)
		end
	end
	
	return true
end

function worldEventHandler:OnFinish()
	
end

return worldEventHandler