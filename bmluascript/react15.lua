--	赤月峡谷东

gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)
gNPCTable[10012] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "这里原来是一个和平的山洞，然而赤月恶魔的苏醒引起了大部分动物的变异，这里也成为了充斥着死亡气味的迷 宫。我在很久很久以前曾作为领袖长期的进行探索，在这里发现了一个邪恶的洞窟，这个也就是邪恶的根源。后来法神和圣战为了维护落霞的安宁，和我一起踏上了消灭恶魔的旅途，由于恶魔太过于强大，在紧要关头我使用传送之书，从那以后就再也没有见到他们了，而我依然肩负着消灭恶魔的使命。", 0)
		ctx:AddContextNode(30, 240, "聊聊", 2)
		ctx:AddContextNode(120, 240, "关闭", 1)

	elseif questid == 0 and queststep == 1 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "能进入这么险恶地方的人不多，不，你是英雄！为了表达我的敬意，我可以帮你合成一把沾满了赤月恶魔鲜血的魔剑 : 赤血魔剑。这需要一把罗刹，2瓶鹿血和骨玉权杖，裁决之杖，龙纹剑各一把，给我这些，我就帮你合成。", 0)
		ctx:AddContextNode(120, 240, "我准备好了", 3)

	elseif questid == 0 and queststep == 2 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "帮你合成好了，希望你有一天能消灭赤月恶魔。", 0)
		ctx:AddContextNode(120, 240, "谢谢", 1)

	elseif questid == 0 and queststep == 3 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "没有材料我也无能为力啊!", 0)
		ctx:AddContextNode(120, 240, "我会努力的", 1)
		
	elseif questid == 0 and queststep == 4 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "这的确是血魔身上的宝物，但也是圣战最后留给我们的东西，曾经在赤月恶魔那儿还遗留了一把圣战使用的火把，带来交给我吧。", 0)
		ctx:AddContextNode(30, 240, "明白", 4)
		ctx:AddContextNode(120, 240, "关闭", 1)
		
	elseif questid == 0 and queststep == 5 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "完成的话跟我说一下，我等你的好消息。", 0)
		ctx:AddContextNode(30, 240, "完成了", 5)
		ctx:AddContextNode(120, 240, "关闭", 1)
		
	elseif questid == 0 and queststep == 6 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "太谢谢你了，这是给你的奖励，我会从中打探圣战和法神的下落的。", 0)
		--ctx:AddContextNode(30, 240, "完成了", 5)
		ctx:AddContextNode(120, 240, "关闭", 1)

	elseif questid == 0 and queststep == 7 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "完成后再来找我吧", 0)
		--ctx:AddContextNode(30, 240, "完成了", 5)
		ctx:AddContextNode(120, 240, "关闭", 1)
		
	elseif questid == 0 and queststep == 8 then

		ctx:AddContextNode(0, 0, "天尊:", 0)
		ctx:AddContextNode(0, 20, "你的血魔之血呢？", 0)
		--ctx:AddContextNode(30, 240, "完成了", 5)
		ctx:AddContextNode(120, 240, "关闭", 1)

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
