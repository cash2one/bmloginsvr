--	世界事件1 怪物攻城 cron:0 0 20 * * *
local worldEventId = 1
local worldEventHandler = {}

--	注册事件
RegisterWorldEvent(worldEventId, worldEventHandler)

--	事件回调
function worldEventHandler:OnInit()
	self:onInit()
end

function worldEventHandler:OnActive()
	self:onActive()
end

function worldEventHandler:OnUpdate()
	self;onUpdate()
end

function worldEventHandler:OnFinish()
	self:onFinish()
end


--	具体逻辑

--	逻辑步骤
local kEventStep_None = 0
local kEventStep_Start = 1
local kEventStep_StartEnd = 2
local kEventStep_Mob = 3
local kEventStep_End = 4

function worldEventHandler:onInit()
	self:switchStep(kEventStep_None)
	--	注册NPC
	RegisterWorldNPCHandler(10050, npcHandler)
end

function worldEventHandler:onActive()
	self:switchStep(kEventStep_Start)
end

function worldEventHandler:onUpdate()
	if nil == self.step then return end
	if kWorldStep_None == self.step then return end
	
	local stepInterval = self:getSwitchStepInterval()
	local ret = false
	
	if kEventStep_Start == self.step then
		--	开始了 发通知
		self:switchStep(kEventStep_StartEnd)
	elseif kEventStep_StartEnd == self.step then
		if stepInterval > 20 then
			self:switchStep(kEventStep_Mob)
			self:mobInit()
		end
	elseif kEventStep_Mob == self.step then
		--	刷怪了
		if self:mobUpdate() then
			--	刷怪完成
			self:switchStep(kEventStep_End)
		end
	elseif kEventStep_End == self.step then
		if stepInterval > 10 then
			--	退出逻辑
			ret = true
		end
	end
	
	return ret
end

function worldEventHandler:onFinish()
	--	进行结算什么的
	self:switchStep(kEventStep_None)
end

function worldEventHandler:switchStep(_step)
	self.step = _step
	self.lastSwitchStepTime = os.time()
end

function worldEventHandler:getSwitchStepInterval()
	local ct = os.time()
	return ct - self.lastSwitchStepTime
end

function worldEventHandler:mobInit()
	--	初始化积分榜
	self.killMonsterScore = {}
	--	轮数
	self.round = 0
	self.nextRoundTime = os.time() + 10
end

local maxRound = 10

function worldEventHandler:mobUpdate()
	if self.round >= maxRound then return true end
	if os.time() < self.nextRoundTime or 0 == self.nextRoundTime then return false end
	
	--	进行刷怪
end


--	NPC处理
local function npcHandler()
end