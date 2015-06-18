gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)

--	10007神秘商人
gNPCTable[10007] =
function(ctx, questid, queststep)
	if questid == 0 and queststep == 0 then
		ctx:AddContextNode(0, 0, "神秘商人:", 0)
		ctx:AddContextNode(0, 20, "居然有人类达到了这儿，放心我不会杀你，我只是个商人而已，即使我是恶魔。在我这里有很多外面没有的物品！", 0)
		ctx:AddContextNode(30, 240, "买东西", 3)
		ctx:AddContextNode(30, 255, "关于沃玛之水", 2)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 1 then
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
		ctx:AddContextNode(30, 270, "关闭", 1)

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
