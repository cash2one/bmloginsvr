gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)

--	老兵
gNPCTable[10013] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "这是一座美丽而又危险的小岛，在这里，封印着传奇世界里面的最终魔王，暂时在岛上是非常安全的，记住不要冒然进入岛上的山洞，你会毙命的。当然这座岛上也藏着各种顶级珍宝，不要为了它们而丧命啊！", 0)
		ctx:AddContextNode(30, 240, "我想回去", 5)
		ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "听过**么？呵呵。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "我收到了封魔城主的问候，谢谢你。这羽毛的确是这座岛上的鸟儿的，不过这是曾经了，这座岛前几年发生了异变，远古英雄全部苏醒，支配了这个岛屿。不过，暗藏的黑手应该是让人胆战心惊的地狱魔王吧。首先帮我调查一下这岛上那些奇怪的动物们吧，杀死50只后回来告诉我。", 0)
		ctx:AddContextNode(30, 255, "完成了", 3)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "英雄我敬佩你，完成了那么险恶的任务。地狱在现实生活中是不存在的，不，或者说是是被远古英雄封印的地方，只有极恶之人才会落入其中永世不得翻身。地狱的出现打破了现实的平衡，需要远古英雄再次封印这个可怕的地方，为了让他再次苏醒，请得到熔岩之心，只有它才能破除远古英雄身上的暗黑魔法封印。", 0)
		ctx:AddContextNode(30, 255, "我得到了熔岩之心", 4)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "你还没有完成杀50只怪物的任务！", 0)
		ctx:AddContextNode(30, 270, "的确", 1)
		
	elseif questid == 0 and queststep == 5 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "赶快将熔岩之心带给封印的英雄吧！", 0)
		--ctx:AddContextNode(30, 255, "我得到了熔岩之心", 4)
		ctx:AddContextNode(30, 270, "明白", 1)
		
	elseif questid == 0 and queststep == 6 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "从熔岩地狱恶魔那里得到熔岩之心！", 0)
		--ctx:AddContextNode(30, 255, "我得到了熔岩之心", 4)
		ctx:AddContextNode(30, 270, "明白", 1)

	end

end



--	杂货商人
gNPCTable[10014] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "杂货商人:", 0)
		ctx:AddContextNode(0, 20, "今天天气不错。", 0)
		ctx:AddContextNode(30, 240, "我想买东西", 3)
		--ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "的确", 1)

	--[[elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "听过**么？呵呵。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "我收到了封魔城主的问候，谢谢你。这羽毛的确是这座岛上的鸟儿的，不过这是曾经了，这座岛前几年发生了异变，远古英雄全部苏醒，支配了这个岛屿。不过，暗藏的黑手应该是让人胆战心惊的**吧。首先帮我调查一下这岛上那些奇怪的动物们吧，杀死50只后回来告诉我。", 0)
		ctx:AddContextNode(30, 255, "完成了", 3)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "的确很厉害！", 0)
		ctx:AddContextNode(30, 255, "完成了", 4)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "你还没有完成杀50只怪物的任务！", 0)
		ctx:AddContextNode(30, 270, "的确", 1)]]

	end

end



--	仓库老板
gNPCTable[10015] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "仓库老板:", 0)
		ctx:AddContextNode(0, 20, "仓库服务可是要收取一定银两的！", 0)
		ctx:AddContextNode(30, 240, "存取东西", 3)
		--ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "的确", 1)
		
	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "仓库老板:", 0)
		ctx:AddContextNode(0, 20, "英雄，快去挣点钱把。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	--[[elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "听过**么？呵呵。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "我收到了封魔城主的问候，谢谢你。这羽毛的确是这座岛上的鸟儿的，不过这是曾经了，这座岛前几年发生了异变，远古英雄全部苏醒，支配了这个岛屿。不过，暗藏的黑手应该是让人胆战心惊的**吧。首先帮我调查一下这岛上那些奇怪的动物们吧，杀死50只后回来告诉我。", 0)
		ctx:AddContextNode(30, 255, "完成了", 3)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "的确很厉害！", 0)
		ctx:AddContextNode(30, 255, "完成了", 4)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "老兵:", 0)
		ctx:AddContextNode(0, 20, "你还没有完成杀50只怪物的任务！", 0)
		ctx:AddContextNode(30, 270, "的确", 1)]]

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
