gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字
--	10010 新手指引
gNPCTable[10010] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then

		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "你好，欢迎进入梦回传奇的世界。下面我将指引你熟悉游戏，伴随你度过初期的成长过程并熟悉基本的地图。那么在落霞村内取1块鸡肉来吧！（杀死鸡一定几率获得）", 0)
		ctx:AddContextNode(30, 240, "我带来了", 2)
		ctx:AddContextNode(120, 240, "我这就去取", 1)

	elseif questid == 0 and queststep == 1 then

		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "您还没有完成任务哦！去取1块鸡肉来吧！", 0)
		ctx:AddContextNode(120, 240, "我明白了", 1)

	elseif questid == 0 and queststep == 2 then

		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "很好，作为报酬，送你5个红药", 0)
		ctx:AddContextNode(120, 240, "谢谢", 3)
		--ButtonID 3 给5红药

	elseif questid == 0 and queststep == 3 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "呀！你的背包东西太多了！请清理了空间再来拿奖励！", 0)
		ctx:AddContextNode(120, 240, "马上整理", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "送你5瓶小红药，接下来请你取1块鹿肉来，会送你一把匕首哦！", 0)
		ctx:AddContextNode(30, 240, "我带来了", 4)
		ctx:AddContextNode(120, 240, "我这就去取", 1)
		--ButtonID 4 收走鹿肉

	elseif questid == 0 and queststep == 5 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "你的背包里面没有鹿肉哦！请取回1块鹿肉！", 0)
		ctx:AddContextNode(120, 240, "马上去取", 1)

	elseif questid == 0 and queststep == 6 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "请留下1格背包空余，放不下了。", 0)
		ctx:AddContextNode(120, 240, "马上整理", 1)

	elseif questid == 0 and queststep == 7 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "这是一把锋利的匕首！接下来帮我跑一下腿吧！河边的渔夫托我带一块修船用的木料，帮我送过去吧!", 0)
		ctx:AddContextNode(30, 240, "给我吧", 5)
		ctx:AddContextNode(120, 240, "我不高兴送", 1)
		--ButtonID 5 送信

	elseif questid == 0 and queststep == 8 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "请留下1格背包空余，放不下了。", 0)
		ctx:AddContextNode(120, 240, "马上整理", 1)

	elseif questid == 0 and queststep == 9 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "船夫在坐标(59,28)处，地图右下角，很容易就找到了", 0)
		ctx:AddContextNode(120, 240, "我明白了", 1)

	elseif questid == 0 and queststep == 10 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "辛苦你帮我送到了,在落霞村左上角可以通往落霞谷，这是一个神秘的地方，虽然景色很美，但暗藏险恶之处。在村外有几只多钩猫钉耙猫和稻草人，它们原先很善良，可惜被半兽人污染了，为了锻炼你，请杀死10只那儿的稻草人。", 0)
		ctx:AddContextNode(30, 240, "好的", 6)
		ctx:AddContextNode(120, 240, "没兴趣", 1)

	elseif questid == 0 and queststep == 11 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "这些稻草人对于现在的你来说还是很危险的，所以这5瓶金创药给你保命吧！杀完了后再来找我吧！", 0)
		ctx:AddContextNode(30, 240, "好的", 1)
		ctx:AddContextNode(120, 240, "我已经完成了", 7)

	elseif questid == 0 and queststep == 12 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "加油吧年轻人!", 0)
		ctx:AddContextNode(30, 240, "我会的", 1)

	elseif questid == 0 and queststep == 13 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "辛苦你了!你应该变得更强壮了!落霞谷(117,247)附近有杀死10只多钩猫或者钉耙猫吧!", 0)
		ctx:AddContextNode(30, 240, "我会的", 8)
		ctx:AddContextNode(120, 240, "算了我不想冒险", 1)

	elseif questid == 0 and queststep == 14 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "完成任务了么?不可小看钉耙猫和多钩猫!", 0)
		ctx:AddContextNode(30, 240, "我完成了", 9)

	elseif questid == 0 and queststep == 15 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "很好，你又成长了，到了7级后可以学习职业的第一个技能，于是努力升级到7级吧，我会送你第一本技能书的。", 0)
		ctx:AddContextNode(30, 240, "我到7级了", 10)

	elseif questid == 0 and queststep == 16 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "努力打怪吧!", 0)
		ctx:AddContextNode(30, 240, "我会的", 1)

	elseif questid == 0 and queststep == 17 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "恭喜你到了7级！以后每7级人物属性都会得到一定的提升，我送了你本技能书，右键可以学习。主动技能可以按F11打开技能窗口，并把鼠标移到相应的技能方框内，同时按下F1 - F8来设置技能使用的快捷键。普通技能最高等级为三级，高级技能最高等级为7级，一级技能手册可以用来提升1-3级的技能，二级可以用来提升4-6级的技能，三级可以提升7级技能，技能书可以打各大BOSS或者僵尸来得到。", 0)
		ctx:AddContextNode(30, 240, "我已经会了，让我更加强大吧", 11)

	elseif questid == 0 and queststep == 18 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "你已经成为了一名合格的战士，基础剑术将会提升你的命中率。为了能够进一步锻炼你，请找这儿的铁匠聊聊吧!", 0)
		ctx:AddContextNode(30, 240, "好", 1)

	elseif questid == 0 and queststep == 19 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "你已经成为了一名合格的法师，小火球将赋予你远程攻击怪物的能力。为了能够进一步锻炼你，请找这儿的铁匠聊聊吧!", 0)
		ctx:AddContextNode(30, 240, "好", 1)

	elseif questid == 0 and queststep == 20 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "你已经成为了一名合格的道士，治愈术将赋予你自我疗伤的能力。为了能够进一步锻炼你，请找这儿的铁匠聊聊吧!", 0)
		ctx:AddContextNode(30, 240, "好", 1)

	elseif questid == 0 and queststep == 21 then
		ctx:AddContextNode(0, 0, "新手指引:", 0)
		ctx:AddContextNode(0, 20, "...", 0)
		ctx:AddContextNode(30, 240, "好", 1)

	end

end

--	铁匠
gNPCTable[10000] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "今天天气不错。听说城外的兽人古墓里隐藏着一个全身透着血红色的可怕的骷髅，传言身上携带者珍贵的宝物。前几个月我的一个孩子去寻宝，可惜再也没有回来，太可怕了", 0)
		ctx:AddContextNode(30, 240, "我想买点东西", 2)
		ctx:AddContextNode(30, 255, "打听打听兽人古墓", 3)
		ctx:AddContextNode(30, 270, "没什么事", 1)
	elseif questid == 0 and queststep == 21 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "在两千年以前，半兽人部落开始出现了，而人类的历史远远的长于它。半兽人因为残暴，天生怪力，所以暂时的压过人类。后来人族崛起，半兽人被驱赶到了荒原上。两千年的时间内死去的半兽人都变成了骷髅亡灵，而怨念也使得亡灵产生了变异，出现了透着血红色的亡灵，也携带着两千多年内前人丢失的珍宝。", 0)
		ctx:AddContextNode(30, 255, "有什么我可以帮忙的", 4)
		ctx:AddContextNode(30, 270, "就这样吧", 1)
	elseif questid == 0 and queststep == 22 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "你还太年轻，多锻炼锻炼吧！", 0)
		ctx:AddContextNode(30, 270, "我会的", 1)
	elseif questid == 0 and queststep == 23 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "看你身手不错，你能帮我到兽人古墓里找找我的孩子吗？尸骨也行...！", 0)
		ctx:AddContextNode(30, 255, "义不容辞", 5)
		ctx:AddContextNode(30, 270, "太危险了，我不干", 1)
	elseif questid == 0 and queststep == 24 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "太感谢你了", 0)
		ctx:AddContextNode(30, 255, "不客气", 1)
	elseif questid == 0 and queststep == 25 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "估计找到的希望不大了...", 0)
		ctx:AddContextNode(30, 270, "节哀顺变", 1)
	elseif questid == 0 and queststep == 26 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "太感谢你帮我找到了我的孩子，这点小东西你就收下吧！下次找我谈话，我可以帮你提升手中武器的潜力。你的成长已经很快了，村里的小贩好像有事请你帮忙。", 0)
		ctx:AddContextNode(30, 270, "十分感谢", 1)
	elseif questid == 0 and queststep == 27 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "这些年我研究半兽人手中锋利的武器悟出了提升武器属性的奥秘，作为奖励我可以帮你提升一次武器的属性，随机的哦", 0)
		ctx:AddContextNode(30, 255, "帮我提升吧", 6)
		ctx:AddContextNode(30, 270, "暂时不需要", 1)
	elseif questid == 0 and queststep == 28 then
		ctx:AddContextNode(0, 0, "铁匠:", 0)
		ctx:AddContextNode(0, 20, "多谢你帮我找到我的孩子。", 0)
		ctx:AddContextNode(30, 255, "不客气", 1)

	end

end

--	渔夫
gNPCTable[10001] =
function(ctx, questid, queststep)

	if questid == 0 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "渔夫:", 0)
		ctx:AddContextNode(0, 20, "我在航海的时候遇到一座美丽的小岛，近距离一看，居然有各种奇怪的动物!居然还把我的船弄坏了，幸好小秀答应我帮我找一块上等木料。", 0)
		ctx:AddContextNode(30, 255, "聊聊小岛", 2)
		ctx:AddContextNode(30, 270, "的确", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "渔夫:", 0)
		ctx:AddContextNode(0, 20, "木料你带来了啊!太感谢你了，这点小费作为小小的奖赏吧!替我向新手指导员道声谢谢。", 0)
		ctx:AddContextNode(30, 270, "谢谢", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "渔夫:", 0)
		ctx:AddContextNode(0, 20, "这的确是一座很美丽的小岛，四面环海，当时我就被迷住了，于是情不自禁的停了船上了小岛，突然发现有各种未曾见到的怪物，还会攻击人！于是我急忙上了船离开了。我还认得那里的路，我依旧对那里耿耿于怀，你能帮我一探究竟么？行的话我可以送你到那里。", 0)
		ctx:AddContextNode(30, 255, "我想试试", 3)
		ctx:AddContextNode(30, 270, "算了", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "渔夫:", 0)
		ctx:AddContextNode(0, 20, "英雄，你还没有达到能探索那座小岛的境界，等到50级以后再来找我吧。", 0)
		ctx:AddContextNode(30, 270, "明白", 1)

	end

end

--	仓库老板
gNPCTable[10006] =
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

--	小贩
gNPCTable[10003] =
function(ctx, questid, queststep)

	if questid == 1 and queststep == 0 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "我这里出售各种药品，杂货，半价收购各种商品，很合算哦！", 0)
		ctx:AddContextNode(30, 240, "我想买点东西", 2)
		ctx:AddContextNode(30, 255, "聊聊", 3)
		ctx:AddContextNode(30, 270, "没什么事", 1)

	elseif questid == 1 and queststep == 1 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "前几天我在收集药材的时候被半兽战士偷袭，逃命之余丢了一份龙胆草，你能帮我找回来吗?", 0)
		ctx:AddContextNode(30, 255, "当然可以", 4)
		ctx:AddContextNode(30, 270, "没空", 1)

	elseif questid == 1 and queststep == 2 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "你还没有能够帮我忙的实力。", 0)
		ctx:AddContextNode(30, 270, "好的", 1)

	elseif questid == 1 and queststep == 3 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "十分感谢！我可以把你送过去，同时会送你一个回城卷轴，回城卷轴可以帮助你回到最近到达的城市。", 0)
		ctx:AddContextNode(30, 255, "现在就过去", 5)
		ctx:AddContextNode(30, 270, "再等等", 1)

	elseif questid == 1 and queststep == 4 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "怎么样，帮我找回龙胆草了没？", 0)
		ctx:AddContextNode(30, 255, "找到了！", 6)
		ctx:AddContextNode(30, 270, "没呢", 1)

	elseif questid == 1 and queststep == 5 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "留出3个空格子再和我说话吧！", 0)
		ctx:AddContextNode(30, 270, "好的", 1)

	elseif questid == 1 and queststep == 6 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "龙胆草是一种珍贵的药材，为了治好落霞山谷（54， 52）王老伯的病，还必须有蝎子的尾巴1个，帮我去兽人古墓搜集一下吧！", 0)
		ctx:AddContextNode(30, 255, "可以", 7)
		ctx:AddContextNode(30, 270, "不干", 1)

	elseif questid == 1 and queststep == 7 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "我等你的好消息！", 0)
		ctx:AddContextNode(30, 255, "我搜集齐了", 8)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 1 and queststep == 8 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "别骗人了，赶紧帮我取1只蝎子尾巴！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 1 and queststep == 9 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "好了，这样王老伯的病可以治好了，我给你我做好的药材，你帮我送过去吧,王老伯在落霞山谷(54, 52)处。", 0)
		ctx:AddContextNode(30, 255, "我已经送过去了", 9)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 1 and queststep == 10 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "王老伯在落霞山谷(54, 52)处,快点送过去吧。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 1 and queststep == 11 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "王老伯很看重你，听说有事拜托你，请你去落霞山谷找找他吧！", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 1 and queststep == 12 then
		ctx:Clear()
		ctx:AddContextNode(0, 0, "小贩:", 0)
		ctx:AddContextNode(0, 20, "去采药的时候听见了野兽的怒吼，绝不是半兽人种类，真可怕。", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	end

end

function ShowHelp(ctx, id)
	if id == 0 then
		ctx:AddContextNode(0, 0, "游戏基本操作", 0)
		ctx:AddContextNode(0, 15, "空格:捡取物品", 0)
		ctx:AddContextNode(0, 30, "F9 :显示背包", 0)
		ctx:AddContextNode(0, 45, "F10:显示人物", 0)
		ctx:AddContextNode(0, 60, "M  :显示大地图,大地图窗口右键按住可以移动", 0)
		ctx:AddContextNode(0, 90, "地图传送点: 走进传送点可以传送，跑进去无效", 0)
		ctx:AddContextNode(0, 120, "玩家接第一个任务，请找树边上的 新手指导员", 0)
	end
end


------------------------------------------
--	分配函数
function OnSetDialogContext(ctx, npcid, questid, queststep)
	local func = gNPCTable[npcid]

	if func ~= nil then
		ctx:Clear()
		func(ctx, questid, queststep)
	else
		MirLog("In react.lua:无法找到NPC["..npcid.."]")
	end
end
