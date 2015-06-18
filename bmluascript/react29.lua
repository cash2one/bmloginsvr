gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)

--	异界之门
gNPCTable[10018] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "异界之门:", 0)
		ctx:AddContextNode(0, 20, "你进入了危险的寒冰世界。这里很少人能进来，或许是某位曾经战胜过恶魔的英雄送进来的吧。也不清楚他怎么样了，当时虽然战胜了这儿深处的恶魔，但也被恶魔的黑暗魔法深深的影响了。", 0)
		ctx:AddContextNode(30, 255, "我想出去", 3)
		ctx:AddContextNode(30, 270, "聊聊", 2)
		ctx:AddContextNode(30, 285, "不用了", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "异界之门:", 0)
		ctx:AddContextNode(0, 20, "恶魔已经沉睡。", 0)
		--ctx:AddContextNode(30, 255, "确定", 3)
		ctx:AddContextNode(30, 270, "关闭", 1)
		
	elseif questid == 0 and queststep == 2 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "异界之门:", 0)
		ctx:AddContextNode(0, 20, "传说寒冰王座深处隐藏着可怕的玛法大陆最可怕的恶魔，它从不走动，即使有人想要接近它，它只需动几根手指就能将对方解决，是一个深不可测的家伙。收集七颗佛珠可以召唤出守护它的侍卫们，而仅仅是这些侍卫，就足以称霸玛法大陆。", 0)
		ctx:AddContextNode(30, 255, "确定", 5)
		ctx:AddContextNode(30, 270, "关闭", 1)
		
	elseif questid == 0 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "异界之门:", 0)
		ctx:AddContextNode(0, 20, "假如你想唤醒恶魔的侍卫们就带着七颗佛珠来找我吧。不过想要唤醒它们的都是不怕死的。", 0)
		ctx:AddContextNode(30, 255, "带来了", 4)
		ctx:AddContextNode(30, 270, "关闭", 1)
		
	elseif questid == 0 and queststep == 4 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "异界之门:", 0)
		ctx:AddContextNode(0, 20, "八卦符咒已经做好了", 0)
		--ctx:AddContextNode(30, 255, "带来了", 4)
		ctx:AddContextNode(30, 270, "关闭", 1)
		
	elseif questid == 0 and queststep == 5 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "异界之门:", 0)
		ctx:AddContextNode(0, 20, "带来七颗佛珠再来吧。", 0)
		--ctx:AddContextNode(30, 255, "带来了", 4)
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
