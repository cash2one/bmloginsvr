if DEBUG then
	--MirLog("module_up load begin.")
end

local ITEM_NO		=	0
local ITEM_CLOTH	=	2
local ITEM_NECKLACE=	3
local ITEM_BRACELAT=	4
local ITEM_RING	=	5
local ITEM_MEDAL	=	6
local ITEM_HELMET	=	7
local ITEM_WEAPON	=	9
local ITEM_SHOE	=	14
local ITEM_BELT	=	15
local ITEM_GEM		=	16
local ITEM_CHARM	=	17

local ATTRIB_DC	=	0
local ATTRIB_MC	=	1
local ATTRIB_AC	=	2
local ATTRIB_SC	=	3
local ATTRIB_MAC	=	4
local ATTRIB_LUCKY	=	5
local ATTRIB_ACCU	=	6
local ATTRIB_HIDE	=	7
local ATTRIB_ATKSPD	=	8

local upgradeTable = {
	[ITEM_WEAPON] = {
		ATTRIB_DC,
		ATTRIB_DC,
		ATTRIB_DC,
		ATTRIB_MC,
		ATTRIB_MC,
		ATTRIB_MC,
		ATTRIB_SC,
		ATTRIB_SC,
		ATTRIB_SC,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU
	},
	[ITEM_RING] = {
		ATTRIB_DC,
		ATTRIB_MC,
		ATTRIB_SC
	},
	[ITEM_BRACELAT] = {
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_DC,
		ATTRIB_MC,
		ATTRIB_SC
	},
	[ITEM_NECKLACE] = {
		ATTRIB_DC,
		ATTRIB_DC,
		ATTRIB_DC,
		ATTRIB_SC,
		ATTRIB_SC,
		ATTRIB_SC,
		ATTRIB_MC,
		ATTRIB_MC,
		ATTRIB_MC,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_ACCU,
		ATTRIB_HIDE,
		ATTRIB_LUCKY
	},
	[ITEM_CLOTH] = {
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_DC,
		ATTRIB_SC,
		ATTRIB_MC
	},
	[ITEM_HELMET] = {
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_DC,
		ATTRIB_SC,
		ATTRIB_MC
	},
	[ITEM_SHOE] = {
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_DC,
		ATTRIB_SC,
		ATTRIB_MC
	},
	[ITEM_BELT] = {
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_DC,
		ATTRIB_SC,
		ATTRIB_MC
	},
	[ITEM_GEM] = {
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_DC,
		ATTRIB_SC,
		ATTRIB_MC
	},
	[ITEM_MEDAL] = {
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_AC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_MAC,
		ATTRIB_DC,
		ATTRIB_SC,
		ATTRIB_MC
	}
}

function canUpgrade(_type)
	if type(_type) ~= type(1) then
		return false
	end

	if _type == ITEM_CLOTH or
	_type == ITEM_NECKLACE or
	_type == ITEM_BRACELAT or
	_type == ITEM_RING or
	_type == ITEM_MEDAL or
	_type == ITEM_HELMET or
	_type == ITEM_WEAPON or
	_type == ITEM_SHOE or
	_type == ITEM_BELT or
	_type == ITEM_CHARM then
		return true
	end

	return false
end

function upgradeAttrib(_item, _index, _up)
	local ok = false

	if _index == ATTRIB_DC then
		_item:SetMaxDC(_item:GetMaxDC() + _up)
		ok = true
	elseif _index == ATTRIB_MC then
		_item:SetMaxMC(_item:GetMaxMC() + _up)
		ok = true
	elseif _index == ATTRIB_SC then
		_item:SetMaxSC(_item:GetMaxSC() + _up)
		ok = true
	elseif _index == ATTRIB_AC then
		_item:SetMaxAC(_item:GetMaxAC() + _up)
		ok = true
	elseif _index == ATTRIB_MAC then
		_item:SetMaxMAC(_item:GetMaxMAC() + _up)
		ok = true
	elseif _index == ATTRIB_LUCKY then
		_item:SetLucky(_item:GetLucky() + _up)
		ok = true
	elseif _index == ATTRIB_ACCU then
		_item:SetAccuracy(_item:GetAccuracy() + _up)
		ok = true
	elseif _index == ATTRIB_HIDE then
		_item:SetHide(_item:GetHide() + _up)
		ok = true
	end

	if ok then
		local level = Lua_MakeItemUpgrade(_item:GetLevel(), _up)
		_item:SetLevel(level)
	end

	return ok
end

local crashNumber = 1000

function upgradeItem(_item, _magicProb, _magicMinProb)
	if not canUpgrade(_item:GetType()) then
		return
	end

	local prob = _magicProb
	if prob < _magicMinProb then
		prob = _magicMinProb
	end

	if crashNumber > prob then
		crashNumber = 1000
	end

	local randomNumber = math.random(1, prob) - 1
	if randomNumber <= crashNumber and
	randomNumber >= (crashNumber - 1000) then
		--	nothing
	else
		--MirLog("random not in range")
		return
	end

	local totalUp = 0
	randomNumber = randomNumber - (crashNumber - 1000)

	if randomNumber == 0 then
		totalUp = 8
	elseif randomNumber >= 1 and
	randomNumber <= 4 then
		totalUp = 7
	elseif randomNumber >= 5 and
	randomNumber <= 19 then
		totalUp = 6
	elseif randomNumber >= 20 and
	randomNumber <= 44 then
		totalUp = 5
	elseif randomNumber >= 45 and
	randomNumber <= 94 then
		totalUp = 4
	elseif randomNumber >= 95 and
	randomNumber <= 194 then
		totalUp = 3
	elseif randomNumber >= 195 and
	randomNumber <= 394 then
		totalUp = 2
	else
		totalUp = 1
	end

	if totalUp == 0 then
		return
	end

	if DEBUG then
		--MirLog("u with number "..totalUp)
	end

	local itemTable = upgradeTable[_item:GetType()]
	if nil == itemTable then
		return
	end

	local size = #itemTable
	local loopCounter = 0
	while totalUp > 0 do
		loopCounter = loopCounter + 1
		if loopCounter > 100 then
			break
		end

		local index = math.random(1, size)
		local attrib = itemTable[index]
		local up = math.random(1, totalUp)

		if nil ~= attrib then
			--	necklace
			if _item:GetType() == ITEM_NECKLACE and
			_item:GetLucky() + up > 3 then
				--	nothing
			else
				if upgradeAttrib(_item, attrib, up) then
					totalUp = totalUp - up
				end
			end
		end
	end
end

function mustUpgradeItem(item, upgradeNumber)
	if LuaItemHelper:IsEncrypt(item) then
		MirLog("trying to upgrade an encrypt item")
		return
	end

	local itemTable = upgradeTable[item:GetType()]
	if nil == itemTable then
		return
	end

	local size = #itemTable
	local loopCounter = 0
	local totalUp = upgradeNumber
	
	while totalUp > 0 do
		loopCounter = loopCounter + 1
		if loopCounter > 100 then
			break
		end

		local index = math.random(1, size)
		local attrib = itemTable[index]
		local up = math.random(1, totalUp)

		if nil ~= attrib then
			--	necklace
			if item:GetType() == ITEM_NECKLACE and
			item:GetLucky() + up > 3 then
				--	nothing
			else
				if upgradeAttrib(item, attrib, up) then
					totalUp = totalUp - up
				end
			end
		end
	end
end