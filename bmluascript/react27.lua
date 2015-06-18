gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)

--	阎罗
gNPCTable[10016] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "阎罗:", 0)
		ctx:AddContextNode(0, 20, "您被检测到可能存在作弊行为，若您想继续游戏，我可以送你出去，不过所有的道具装备都将清空。", 0)
		ctx:AddContextNode(30, 255, "我想出去", 2)
		ctx:AddContextNode(30, 270, "不用了", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "阎罗:", 0)
		ctx:AddContextNode(0, 20, "出去将清空所有的道具，确定要出去？", 0)
		ctx:AddContextNode(30, 255, "确定", 3)
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
