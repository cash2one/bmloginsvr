gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)

--	10019恶魔商人
gNPCTable[10019] =
function(ctx, questid, queststep)
	if questid == 0 and queststep == 0 then
		ctx:AddContextNode(0, 0, "恶魔商人:", 0)
		ctx:AddContextNode(0, 20, "居然能在这里看到人类，真是不可思议。别担心，只要给钱我就是一个商人，给你看看我这里有什么吧！", 0)
		ctx:AddContextNode(30, 240, "买东西", 3)
		ctx:AddContextNode(30, 255, "打开仓库", 2)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:AddContextNode(0, 0, "神秘商人:", 0)
		ctx:AddContextNode(0, 20, "我这里的仓库可需要10000金币才能使用！", 0)
		--ctx:AddContextNode(30, 255, "我要换", 4)
		ctx:AddContextNode(30, 270, "关闭", 1)

	--[[elseif questid == 0 and queststep == 1 then
		ctx:AddContextNode(0, 0, "神秘商人:", 0)
		ctx:AddContextNode(0, 20, "我正在收集神奇的沃玛之水，我愿意高价收购，我就拿装备来换吧！", 0)
		ctx:AddContextNode(30, 255, "我要换", 4)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:AddContextNode(0, 0, "神秘商人:", 0)
		ctx:AddContextNode(0, 20, "我愿意用这些装备来换你的10瓶沃玛神水。", 0)
		ctx:AddContextNode(5, 40, "炼狱", 5)
		ctx:AddContextNode(5, 55, "魔杖", 6)
		ctx:AddContextNode(5, 70, "银蛇", 7)
		ctx:AddContextNode(5, 85, "关闭", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:AddContextNode(0, 0, "神秘商人:", 0)
		ctx:AddContextNode(0, 20, "你还没有收集齐哦！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:AddContextNode(0, 0, "神秘商人:", 0)
		ctx:AddContextNode(0, 20, "已经换好了！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)]]

	end
end

------------------------------------------
--	分配函数
function OnSetDialogContext(ctx, npcid, questid, queststep)
	local func = gNPCTable[npcid]

	MirLog("Parameters:NPCID:"..npcid.."QuestID:"..questid.."QuestStep:"..queststep)
	if func ~= nil then
		ctx:Clear()
		func(ctx, questid, queststep)
	else
		MirLog("In react.lua:无法找到NPC["..npcid.."]")
	end
end
