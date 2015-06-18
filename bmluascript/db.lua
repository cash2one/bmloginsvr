--	只能在DBThread线程中进行调用
math.randomseed(tostring(os.time()):reverse():sub(1, 6))
DBThread:GetInstance():GetLuaEngine():LoadModule("module_up")

local function div(x, y)
	return x/y
end

local function getDropItemIDTable(_data, _prob)
	local finddata = _data
	local itemIDTable = {}

	local fstIdx = 0
	local secIdx = 0
	local prevIdx = 0

	local dropDownItems = {}

	if _prob == nil or
	_prob == 0 then
		_prob = 1
	end

	while fstIdx ~= nil do
		fstIdx = string.find(finddata, ",", prevIdx + 1)

		if fstIdx ~= nil then
			local subString = string.sub(finddata, prevIdx + 1, fstIdx - 1)
			table.insert(itemIDTable, subString)
			prevIdx = fstIdx
		end

	end

	--	last one
	table.insert(itemIDTable, string.sub(finddata, prevIdx + 1, string.len(finddata)))

	--	calculate property
	local probMulti = _prob

	for i, v in ipairs(itemIDTable) do
		local idIndex = 0
		idIndex = string.find(v, "|")

		local prob = string.sub(v, idIndex + 1)
		local strID = string.sub(v, 1, idIndex - 1)

		local actProb = tonumber(prob)
		if _prob ~= 1 then
			--actProb = actProb / probMulti
			actProb = div(actProb, probMulti)
		end

		if actProb < 1 then
			actProb = 1
		end

		local random = math.random(1, actProb)
		if random == 1 then
			table.insert(dropDownItems, tonumber(strID))
		end
	end

	return dropDownItems
end

function DropDownMonsterItems(_data, _context)
	if not _context:IsDropValid() then
		return
	end

	local dropDownItems = getDropItemIDTable(_data, _context:GetDropMultiple())

	local dropItemsCounter = #dropDownItems
	if dropItemsCounter == 0 then
		return
	end

	local canDropSum = _context:InitDropPosition(dropItemsCounter)

	if DEBUG then
		--MirLog("可掉落物品数:"..canDropSum)
		--MirLog("掉落系数:".._context:GetDropMultiple())
		--MirLog("魔法掉落系数".._context:GetMagicDropMultiple())
	end

	for i = 0, canDropSum - 1 do
		local posX = _context:GetDropPosX(i)
		local posY = _context:GetDropPosY(i)

		if 0 == posX or
		0 == posY then
			break
		end

		local dropItemID = dropDownItems[i + 1]
		local dropItem = _context:NewGroundItem(dropItemID, posX, posY)
		if nil == dropItem then
			break
		end

		--	upgrade items
		upgradeItem(dropItem:GetItemAttrib(), _context:GetMagicDropMultiple(), 3000)
		--upgradeItem(dropItem:GetItemAttrib(), 1000, 1000)
	end
end
