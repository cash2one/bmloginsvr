--	副本逻辑
local scene = nil

local copy_begin = false
local copy_begin_time = os.time()
local copy_end_tip = false
local current_round = 0
local copy_timeout = 10 * 60

local round_begin = false
local round_begin_time = os.time()
local round_end_time = os.time()
local round_finnal = false
local round_max = 5

local last_tip_remain_monster_time = os.time()

local func_reward = nil
local func_nextRound = nil

---------------------	functions	-----------------------------
function clg_setRewardFunc(_func)
	func_reward = _func
end

function clg_setNextRoundFunc(_func)
	func_nextRound = _func
end

function clg_setScene(_scene)
	scene = _scene
end

function clg_setRoundMax(_max)
	round_max = _max
end

function clg_setTimeout(_timeout)
	copy_timeout = _timeout
end

function clg_reset()
	copy_begin = false
	copy_begin_time = os.time()
	copy_end_tip = false

	current_round = 0
	round_begin = false
	round_begin_time = os.time()
	round_end_time = os.time()

	round_finnal = false
end

function clg_isBegin()
	return copy_begin
end

function clg_setBegin(_begin)
	if copy_begin == _begin then
		return
	end

	copy_begin = _begin

	if copy_begin then
		clg_onBegin()
	end
end

function clg_onBegin()
	local playerCount = scene:GetPlayerCount()

	if playerCount > 1 then
		local multi = 1 + playerCount / 6
		scene:SetMonsterDefenceMulti(multi)
		scene:SetMonsterAttackMulti(multi)
	end

	copy_begin_time = os.time()

	local timeoutMinute = copy_timeout / 60
	scene:BroadcastSceneSystemMessage("挑战开始，时间限制["..timeoutMinute.."分钟]")
end

function clg_onNextRound()
	--	is round finnal?
	if current_round >= round_max then
		round_finnal = true
		--	give rewards
		if nil ~= func_reward then
			func_reward()
		end
		return
	end

	round_begin = true

	current_round = current_round + 1

	--	create monsters
	if func_nextRound then
		func_nextRound(current_round)
	end

	scene:BroadcastSceneSystemMessage("第"..current_round.."波挑战开始")
end

function clg_update(_scene)
	if not clg_isBegin() then
		return
	end

	if nil == scene then
		return
	end

	if round_finnal then
		return
	end

	if scene:GetPlayerCount() == 0 then
		return
	end

	if scene:GetChallengeID() == 0 then
		return
	end

	--	超过时间了
	if os.time() - copy_begin_time > copy_timeout then
		round_finnal = true
		scene:BroadcastSceneSystemMessage("挑战超时！失败！")
		scene:AllHeroFlyToHome()
		return
	end

	if os.time() - copy_begin_time > copy_timeout - 1 * 60 and
	not copy_end_tip then
		scene:BroadcastSceneSystemMessage("挑战还剩1分钟")
		copy_end_tip = true
	end

	if scene:GetMonsterCount() == 0 then
		if round_begin then
			round_begin = false
			round_end_time = os.time()
			return
		else
			if os.time() - round_end_time > 5 then
				clg_onNextRound()
				return
			end
		end
	else
		if os.time() - last_tip_remain_monster_time > 10 then
			last_tip_remain_monster_time = os.time()
			scene:BroadcastSceneSystemMessage("还剩"..scene:GetMonsterCount().."只怪物")
		end
	end
end
