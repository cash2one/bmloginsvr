--	ֻ����GameWorld�߳��н��е���
ENGINE:LoadModule("module_up")

function CompItems(player, itemTags)
	--	��ս�������� ֱ�ӷ���


	--	��Ʒ����<2��
	if #itemTags < 2 then return false end

	local itemIDTable = {}

	for i, v in ipairs(itemTags) do
		local itemID = player:ItemTagToAttribID(v)

		if nil == itemIDTable[itemID] then
			itemIDTable[itemID] = {}
		end

		table.insert(itemIDTable[itemID], v)
	end

	--	2����������Ʒ ���Խ��кϳ�
	local typeSum = 0
	for _, _ in pairs(itemIDTable) do
		typeSum = typeSum + 1
	end

	if typeSum == 0 or
		typeSum > 2 then
		return false
	end

	--	һ������ ֱ�ӽ��кϳɲ���
	local hasStone = false
	local upgradeItemID = 0

	if 1 == typeSum then
		--	����Ƿ��ǿɺϳɵ�װ��
		for itemID, _ in pairs(itemIDTable) do
			upgradeItemID = itemID
		end

		--	��������
	elseif 2 == typeSum then
		--	�������ͣ�װ��+��Ԫʯ�����Ա������װ���ĵȼ�
		local hasStone = false
		local stoneID = 100

		--	�ȼ����û��ʯͷ
		local costItemTags = itemIDTable[stoneID]
		if nil == costItemTags then return false end

		--	���װ���Ƿ����
		hasStone = true
		for itemID, _ in pairs(itemIDTable) do
			if itemID ~= stoneID then
				upgradeItemID = itemID
			end
		end
	end

	--	���װ���Ƿ��������
	local equipItemTag = itemIDTable[upgradeItemID][1]
	if nil == equipItemTag then return false end

	local item = player:getItemByTag(equipItemTag)
	if not canUpgrade(item:GetType()) then
		return false
	end

	--	��������
	local itemCount = #itemIDTable[upgradeItemID]
	local upgradeProb = 5 * itemCount

	local randNumber = math.random(1, 100)
	if randNumber <= upgradeProb then
		
	end

	return false
end

function CubeItems(player, number, tag0, tag1, tag2, tag3, tag4, tag5, tag6, tag7)

	--	��һ�� װ���ĺϳ�
	local itemTags = {}
	if tag0 ~= 0 then table.insert(itemTags, tag0) end
	if tag1 ~= 0 then table.insert(itemTags, tag1) end
	if tag2 ~= 0 then table.insert(itemTags, tag2) end
	if tag3 ~= 0 then table.insert(itemTags, tag3) end
	if tag4 ~= 0 then table.insert(itemTags, tag4) end
	if tag5 ~= 0 then table.insert(itemTags, tag5) end
	if tag6 ~= 0 then table.insert(itemTags, tag6) end
	if tag7 ~= 0 then table.insert(itemTags, tag7) end

	if CompItems(player, itemTags) then return end

	local item0 = player:ItemTagToAttribID(tag0)
	local item1 = player:ItemTagToAttribID(tag1)
	local item2 = player:ItemTagToAttribID(tag2)
	local item3 = player:ItemTagToAttribID(tag3)
	local item4 = player:ItemTagToAttribID(tag4)
	local item5 = player:ItemTagToAttribID(tag5)
	local item6 = player:ItemTagToAttribID(tag6)
	local item7 = player:ItemTagToAttribID(tag7)

	if number == 3 then

		--	����������سǾ�
		if item0 == 404 and item1 == 404 and item2 == 404 then

			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:AddItem(402)

		end
	elseif number == 5 then

		if item0 == 625 and item1 == 626 and item2 == 627 and item3 == 628 and item4 == 629 then

			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:RemoveItem(tag3)
			player:RemoveItem(tag4)
			player:AddItem(1108)

		end

	elseif number == 1 then

		--	������ �޼��� ħ��
		if item0 == 48 then
			player:RemoveItem(tag0)
			player:AddItem(279)

		elseif item0 == 52 then
			player:RemoveItem(tag0)
			player:AddItem(279)

		elseif item0 == 42 then
			player:RemoveItem(tag0)
			player:AddItem(279)

		--	�þ�
		elseif item0 == 55 then
			player:RemoveItem(tag0)
			player:AddItem(287)

		elseif item0 == 59 then
			player:RemoveItem(tag0)
			player:AddItem(287)

		elseif item0 == 56 then
			player:RemoveItem(tag0)
			player:AddItem(287)

		--	��Ӣ��ͷ��ϵ��
		elseif item0 == 102 or item0 == 101 or item0 == 104 then
			player:RemoveItem(tag0)
			player:AddItem(279)

		--	�������� 1��5w ��ָ
		elseif item0 == 336 or item0 == 182 or item0 == 183 then
			player:RemoveItem(tag0)
			player:AddItem(279)

		--	�������� 1��5w ����
		elseif item0 == 214 or item0 == 209 or item0 == 208 then
			player:RemoveItem(tag0)
			player:AddItem(279)

		--	�������� 1��5w ����
		elseif item0 == 239 or item0 == 240 or item0 == 244 then
			player:RemoveItem(tag0)
			player:AddItem(279)

		--	��Ӣ������ 1��10w
		--	��ָ
		elseif item0 == 147 or item0 == 157 or item0 == 158 then
			player:RemoveItem(tag0)
			player:AddItem(287)

		--	����
		elseif item0 == 229 or item0 == 234 or item0 == 226 then
			player:RemoveItem(tag0)
			player:AddItem(287)

		--	����
		elseif item0 == 195 or item0 == 196 or item0 == 197 then
			player:RemoveItem(tag0)
			player:AddItem(287)

		end

	elseif number == 2 then

		--	������������ 2��5w
		--	��������
		if item0 == 247 and item1 == 247 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		--	��������
		elseif item0 == 218 and item1 == 218 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		--	��������
		elseif item0 == 320 and item1 == 320 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		--	��������
		elseif item0 == 328 and item1 == 328 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		--	˼��������
		elseif item0 == 210 and item1 == 210 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		--	��֮��ָ
		elseif item0 == 169 and item1 == 169 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		--	�챦ʯ��ָ
		elseif item0 == 166 and item1 == 166 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		--	�챦ʯ��ָ
		elseif item0 == 173 and item1 == 173 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:AddItem(279)

		end

	elseif number == 4 then

			--	����ս��
			if item0 == 920 and item1 == 921 and item2 == 922 and item3 == 923 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(773)

			--	��������
			elseif item0 == 916 and item1 == 917 and item2 == 918 and item3 == 919 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(766)

			--	��������
			elseif item0 == 940 and item1 == 941 and item2 == 942 and item3 == 943 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(781)

			--	����սѥ
			elseif item0 == 944 and item1 == 945 and item2 == 946 and item3 == 947 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(752)

			--	��������
			elseif item0 == 948 and item1 == 949 and item2 == 950 and item3 == 951 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(761)

			--	����ħ��
			elseif item0 == 956 and item1 == 957 and item2 == 958 and item3 == 959 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(771)

			--	���滤��
			elseif item0 == 952 and item1 == 953 and item2 == 954 and item3 == 955 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(762)

			--	��������
			elseif item0 == 980 and item1 == 981 and item2 == 982 and item3 == 983 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(775)

			--	����ħѥ
			elseif item0 == 960 and item1 == 961 and item2 == 962 and item3 == 963 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(750)

			--	��������
			elseif item0 == 984 and item1 == 985 and item2 == 986 and item3 == 987 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(759)

			--	��â����
			elseif item0 == 888 and item1 == 889 and item2 == 890 and item3 == 891 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(764)

			--	��â����
			elseif item0 == 880 and item1 == 881 and item2 == 882 and item3 == 883 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(772)

			--	��â����
			elseif item0 == 908 and item1 == 909 and item2 == 910 and item3 == 911 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(779)

			--	��â��ѥ
			elseif item0 == 884 and item1 == 885 and item2 == 886 and item3 == 887 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(751)

			--	��â����
			elseif item0 == 912 and item1 == 913 and item2 == 914 and item3 == 915 then
				player:RemoveItem(tag0)
				player:RemoveItem(tag1)
				player:RemoveItem(tag2)
				player:RemoveItem(tag3)
				player:AddItem(760)

			end

	elseif number == 8 then

		--	����ս����
		if item0 == 896 and item1 == 897 and item2 == 898 and item3 == 899 and item4 == 924 and item5 == 925 and item6 == 926 and item7 == 927 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:RemoveItem(tag3)
			player:RemoveItem(tag4)
			player:RemoveItem(tag5)
			player:RemoveItem(tag6)
			player:RemoveItem(tag7)
			player:AddItem(869)

		--	����ħ����
		elseif item0 == 896 and item1 == 897 and item2 == 898 and item3 == 899 and item4 == 964 and item5 == 965 and item6 == 966 and item7 == 967 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:RemoveItem(tag3)
			player:RemoveItem(tag4)
			player:RemoveItem(tag5)
			player:RemoveItem(tag6)
			player:RemoveItem(tag7)
			player:AddItem(871)

		--	��â������
		elseif item0 == 892 and item1 == 893 and item2 == 894 and item3 == 895 and item4 == 896 and item5 == 897 and item6 == 898 and item7 == 899 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:RemoveItem(tag3)
			player:RemoveItem(tag4)
			player:RemoveItem(tag5)
			player:RemoveItem(tag6)
			player:RemoveItem(tag7)
			player:AddItem(590)

		--	����ս��Ů
		elseif item0 == 904 and item1 == 905 and item2 == 906 and item3 == 907 and item4 == 924 and item5 == 925 and item6 == 934 and item7 == 935 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:RemoveItem(tag3)
			player:RemoveItem(tag4)
			player:RemoveItem(tag5)
			player:RemoveItem(tag6)
			player:RemoveItem(tag7)
			player:AddItem(870)

		--	����ħ��Ů
		elseif item0 == 904 and item1 == 905 and item2 == 906 and item3 == 907 and item4 == 964 and item5 == 965 and item6 == 974 and item7 == 975 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:RemoveItem(tag3)
			player:RemoveItem(tag4)
			player:RemoveItem(tag5)
			player:RemoveItem(tag6)
			player:RemoveItem(tag7)
			player:AddItem(872)

		--	��â����Ů
		elseif item0 == 892 and item1 == 893 and item2 == 902 and item3 == 903 and item4 == 904 and item5 == 905 and item6 == 906 and item7 == 907 then
			player:RemoveItem(tag0)
			player:RemoveItem(tag1)
			player:RemoveItem(tag2)
			player:RemoveItem(tag3)
			player:RemoveItem(tag4)
			player:RemoveItem(tag5)
			player:RemoveItem(tag6)
			player:RemoveItem(tag7)
			player:AddItem(591)

		end

	end
end

local OS_DEAD = 5

function DropDownMonsItems(_dbparam, _hero, _mons)
	if nil == _mons or
	nil == _hero or
	nil == _dbparam then
		--MirLog("invalid input params")
		return
	end

	if _mons:GetState() ~= OS_DEAD then
		--MirLog("mons not dead".._mons:GetState().." dead:"..OS_DEAD)
		return
	end

	if not _mons:GetCanDropItems() then
		--MirLog("can't drop items")
		return
	end

	if _mons:GetTotalRecvDamage() < _mons:GetMaxHP() then
		--MirLog("recv damage:".._mons:GetTotalRecvDamage().." max hp:".._mons:GetMaxHP())
		return
	end

	_dbparam:SetParam(4, _hero:Lua_GetActiveDropParam())

	DBThread:GetInstance():AsynExecute(_dbparam)
end