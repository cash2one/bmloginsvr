gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)

--	10008 封魔商人
gNPCTable[10008] =
function(ctx, questid, queststep)
	if questid == 0 and queststep == 0 then
		ctx:AddContextNode(0, 0, "恶魔商人:", 0)
		ctx:AddContextNode(0, 20, "我这里出售各种高级消耗品！", 0)
		ctx:AddContextNode(30, 255, "我要买", 2)
		ctx:AddContextNode(30, 270, "关闭", 1)

	end
end

--	10009 封魔城主
gNPCTable[10009] =
function(ctx, questid, queststep)
	if questid == 0 and queststep == 0 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "我是这座城池的主人，找我有何贵干？", 0)
		--ctx:AddContextNode(30, 227, "挑战副本", 50)
		ctx:AddContextNode(30, 240, "传送", 5)
		ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "这原本是个多么美好祥和的城市，人们安居乐业，然而当祖玛教主和虹膜教主复活的那一刻，整个城市都变了，所有子民们都变成了恶魔，而我保持了人形也只是一个傀儡，多么希望有英雄可以帮帮我们。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "你居然携带着神秘的勋章！证明了你有足够的实力，你能帮我完成几件事么？", 0)
		ctx:AddContextNode(30, 255, "接受", 3)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "你的勋章呢？", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "我把神秘的勋章转化为了荣誉勋章，英雄你配得上这个勋章！为了使邪恶的教主们再度陷入沉睡，必须完成下面几件事。可是现在消息不全，你去地牢一层或二层杀50只蜈蚣或者钳虫来打探打探教主们的反应。", 0)
		ctx:AddContextNode(30, 255, "完成了", 4)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 5 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "你还没有杀死50只蜈蚣或钳虫！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 6 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "有点眉目了，查出了一个教主的所在地，我现在随时可以把你传送过去，我需要获得一封 封魔密信 来获取更多的情报，这封信可以在祖玛大殿里杀死 暗之祖玛弓箭手 得到。", 0)
		ctx:AddContextNode(30, 255, "找到了", 6)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 7 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "这是一封古老的信，由法玛文字写成，世上很少有人能懂，但是我知道有一个人懂这文字，就是在矿窑研习秘籍的神秘学者，麻烦你把这封信给他看看吧!", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 8 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, " 封魔密信 可以在祖玛大殿里杀死 暗之祖玛弓箭手 得到。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 9 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "神秘学者不愧是学者，轻而易举的解读了这封信。原来恶魔的阴影不止覆盖到了我的封魔大陆，还影响到了落霞山谷，这些恶魔栖息在矿窑的深处，似乎是一些可怕的蜘蛛，在赤月峡谷带回5个它们的剧毒蜘丝来看看它们苏醒到什么程度了吧！", 0)
		ctx:AddContextNode(30, 255, "找到了", 8)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 10 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "要让这些恶魔们沉睡，必须得有一把上古神器，它的碎片分散在珐玛大陆，据确切消息各大教主们身上携带着它们，然而要能重现这把神器，必须得有 潘多拉魔盒，在血僵尸身上获取它吧!", 0)
		ctx:AddContextNode(30, 255, "找到了", 9)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 11 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "我需要5个剧毒蛛丝，加油吧。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 12 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "得到了魔盒，剩下只要收集神器的碎片即可，去杀死兽人古墓的骷髅精灵，矿窑的尸王，沃玛神殿的沃玛教主，连接通道的白野猪，祖玛大殿的祖玛教主，收集好碎片通过潘多拉魔盒合成上古神器来交给我吧！", 0)
		ctx:AddContextNode(30, 255, "合成了", 10)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 13 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "我用上古神器的力量暂时封印了影响这里的恶魔们，封印之后掉下了一样奇怪的东西，可能和最近某座小岛的情况有关。这瓶水可能可以召唤出恶魔来，帮我消灭它，看看它身上携带着什么东西吧。", 0)
		ctx:AddContextNode(30, 255, "消灭了", 11)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 14 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "合成好上古神器，然后交给我。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 15 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "这根羽毛十分稀有，我印象中只有某处的一个小岛上才存在着这种鸟类，这些恶魔必定是从那里出现的！带着这根羽毛去小岛见小村里的老兵吧！我和他很熟悉了，顺便替我向他问个好。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 16 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "杀死了该封印水中的怪物没？带上它的掉落物品来见我吧。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)


	elseif questid == 0 and queststep == 100 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "我现在可以把你传送到祖玛大殿。", 0)
		ctx:AddContextNode(0, 50, "祖玛大殿", 7)
		ctx:AddContextNode(0, 65, "关闭", 1)

	elseif questid == 0 and queststep == 101 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "没有足够的情报，我还无法传送。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 1 and queststep == 0 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "假如你拥有各大BOSS身上携带的副本通行证，我就可以把你送到对应的副本中去。只有队长才能让我传送哦！副本的难度随着组队人数的增加而增加，当然其中的收获也是很多的！", 0)
		ctx:AddContextNode(30, 140, "副本一(等级20以上)", 51)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 1 and queststep == 1 then
		ctx:AddContextNode(0, 0, "封魔城主:", 0)
		ctx:AddContextNode(0, 20, "你不是队长，请让你们的队长来和我说话吧。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	end
end


--	仓库老板
gNPCTable[10011] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "仓库老板:", 0)
		ctx:AddContextNode(0, 20, "背包太重背不动放不下了怎么办？当然是找我呗！我这提供方便快捷的服务，当然要给我一点点报酬啦！当然是小钱而已，每次打开仓库需要消耗500金币。", 0)
		ctx:AddContextNode(30, 240, "打开仓库", 2)
		--ctx:AddContextNode(30, 255, "打听打听兽人古墓", 3)
		ctx:AddContextNode(30, 255, "没什么事", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "仓库老板:", 0)
		ctx:AddContextNode(0, 20, "这是生意，没钱还来找我？准备好500金币再来找我吧！", 0)
		ctx:AddContextNode(30, 255, "努力赚钱", 1)

	end

end

--	冰封的英雄
gNPCTable[10017] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "冰封的英雄:", 0)
		ctx:AddContextNode(0, 20, "在过去的岁月里，我作为寻找玛法家园的使者，孤身一人闯入了雪域深处。在那里我与强大的魔王进行了三天三夜是殊死搏斗，终于将魔王打败，然而自己却也不幸被黑暗魔法所伤，身体上产生了一些可怕的异变，最终成为了一座冰冷的雕像。", 0)
		ctx:AddContextNode(30, 240, "打听关于雪域的事情", 3)
		ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "没什么事", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "冰封的英雄:", 0)
		ctx:AddContextNode(0, 20, "由于我的牺牲，揭开了雪域的古老封印，如今人们才能一窥雪域的无限奥秘。雪域是非常凶险的地方，当然也有有着未知魔力的稀世装备。", 0)
		--ctx:AddContextNode(30, 240, "打听关于雪域的事情", 3)
		--ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "明白了", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "冰封的英雄:", 0)
		ctx:AddContextNode(0, 20, "只有实力超群的英雄才能进入那里。", 0)
		--ctx:AddContextNode(30, 240, "打听关于雪域的事情", 3)
		--ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "明白了", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "冰封的英雄:", 0)
		ctx:AddContextNode(0, 20, "很遗憾，黑暗魔法持续的时间太长，即使有用，我已经无法恢复当年的身手与体力了。我是即将落下的夕阳，而英雄你确是冉冉升起的朝日。我很确信你可以进入雪域与恶魔进行一番较量，我会把你送过去。", 0)
		--ctx:AddContextNode(30, 240, "打听关于雪域的事情", 3)
		--ctx:AddContextNode(30, 255, "聊聊", 2)
		ctx:AddContextNode(30, 270, "明白了", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "冰封的英雄:", 0)
		ctx:AddContextNode(0, 20, "我可以将你传送到静之冰谷中", 0)
		--ctx:AddContextNode(30, 240, "打听关于雪域的事情", 3)
		ctx:AddContextNode(30, 255, "需要去", 4)
		ctx:AddContextNode(30, 270, "明白了", 1)

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
