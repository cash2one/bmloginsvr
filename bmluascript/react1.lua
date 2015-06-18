gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字
--	王老伯
gNPCTable[10004] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "落霞山谷真是一个奇妙的地方，外面安静，却暗藏无数山洞，兽人古墓是已经被大家所熟知的，据传说还有携带着前人武功秘籍的僵尸出没，更有邪恶的宗教存在，其首领更是杀人无数，不过由于无法抵御阳光，我们这还算安全，谢天谢地啊！", 0)
		ctx:AddContextNode(30, 240, "传送", 111)
		ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "真想一睹那些稀世武林秘籍和首饰的尊荣啊！可惜人已老，年轻人你还能有机会去实现它。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "落霞村的小贩真是个好人，也谢谢你帮我送来。回去告诉小贩我已经收到了并表达我的谢意吧！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "听说兽人古墓深处有骷髅族的头目：骷髅精灵，而那深处也有一扇门，我想在那里做一种仪式，我老了，已经不能潜入那么危险的地方了，杀死一个骷髅精灵，完成这个后再来找我吧", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "年轻人，快去帮我完成这个仪式吧，我不会亏待你的！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 5 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "你完成了这个仪式！作为报酬我可以为你开启传送到山谷矿区的开关，对你不用惊讶，其实我本身也是一个迷。", 0)
		ctx:AddContextNode(30, 255, "接受奖励", 3)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 6 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "矿区是一个恐怖的地方，僵尸原先是一个矿山中的矿工。矿山在奇怪力量导致的地震中坍塌，他们被困其中。他们死去后重生为僵尸。它们身上携带着不少书籍，帮我带回一本《古墓书籍》回来吧！", 0)
		ctx:AddContextNode(30, 255, "我已经完成了", 4)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 7 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "留出足够的空间来领取奖励！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 8 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "我已经参透了古墓书籍的奥秘，所以兽人古墓的迷也差不多解开了，这是一张神奇的卷轴，你可以使用它来进入兽人古墓二层的神秘宫殿，仪式已经开始了，神秘的异教徒们也开始蠢蠢欲动了，听说骷髅精灵也会参与这个仪式，所以你可能能在它身上找到这份卷轴。请进入此神殿，杀死护卫给我带来一个邪教勋章吧!", 0)
		ctx:AddContextNode(30, 255, "带来了", 5)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 9 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "探索矿窑，带来一本古墓书籍，快去吧年轻人。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 10 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "神殿里面是相当的危险，还有可怕的教主存在，千万要当心啊。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 11 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "你很勇敢，这个勋章代表了一种荣誉，你带着它去封魔大陆见封魔城主，那里最近发生了一些异变，城主需要你的帮助。从此，你将踏上一个新的旅途。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 111 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "山谷矿区", 112)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 254 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "这可不是义务劳动！准备好足够的钱再来找我吧!", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 255 then
		ctx:AddContextNode(0, 0, "王老伯:", 0)
		ctx:AddContextNode(0, 20, "我还无法帮你为你进行传送，因为我还无法确定你的实力。", 0)
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
