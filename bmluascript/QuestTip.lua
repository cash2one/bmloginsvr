--客户端任务提示脚本

MirLog("载入任务提示脚本成功")

function CreateQuestDlg(dlg, ctx, player)
	MirLog("创建任务窗体")

	--	Main quest 0
	local curstep = ctx:GetQuestStep(0)
	MirLog("Current step:"..curstep)

	if curstep >= 0 then
		--	Need tip
		--local questctx = dlg:PushQuestInfo()
		--local qx = questctx

		if curstep == 0 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "交给(21,14)处的新手指导员一份鸡肉"
			qx.stTip.xReward = "金创药(小量)*5"
		elseif curstep == 4 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "交给(21,14)处的新手指导员一份鹿肉"
			qx.stTip.xReward = "一把匕首"
		elseif curstep == 7 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "领取木料"
		elseif curstep == 9 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "将修复渔船的木料送给河边的渔夫"
			qx.stTip.xReward = "金币500"
		elseif curstep == 10 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "回去向新手指导员汇报任务完成"
			qx.stTip.xReward = "金创药(小量)*5"
		elseif curstep == 11 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			local counter = ctx:GetQuestCounter(0)
			if counter <= 9 then
				qx.stTip.xRequire = "消灭落霞村外的("..counter.."/10)只稻草人"
			else
				qx.stTip.xRequire = "回去寻找新手指导员"
			end
			qx.stTip.xReward = "经验500"
		elseif curstep == 13 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "和新手指导员对话"
		elseif curstep == 14 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			local counter = ctx:GetQuestCounter(0)
			if counter <= 9 then
				qx.stTip.xRequire = "消灭落霞村外的("..counter.."/10)只钉耙猫或多钩猫"
			else
				qx.stTip.xRequire = "回去寻找新手指导员"
			end
			qx.stTip.xReward = "经验1000 金币2000"
		elseif curstep == 15 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "升级到7级后向新手指导员领取7级书籍，记得背包留一格空哦！"
			MirLog("JOB"..player:GetHeroJob())
			if player:GetHeroJob() == 0 then
				qx.stTip.xReward = "基本剑术"
			elseif player:GetHeroJob() == 1 then
				qx.stTip.xReward = "小火球"
			elseif player:GetHeroJob() == 2 then
				qx.stTip.xReward = "治愈术"
			end
		elseif curstep == 17 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "和新手指导员对话"
		elseif curstep == 18 or curstep == 19 or curstep == 20 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "和铁匠对话"
		elseif curstep == 25 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "在落霞山谷附近的半兽人古墓中找到铁匠的孩子"
			--qx.stTip.xReward = "经验800 金币1000"
		elseif curstep == 26 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "告诉铁匠小宝的消息"
			qx.stTip.xReward = "经验800 金币1000"
		elseif curstep == 27 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "英雄成长"
			qx.stTip.xDescript = "欢迎进入梦回传奇的世界，为了让您更加熟悉游戏，帮助您成长，请寻找(21,14)处的新手指导员领取新手任务。"
			qx.stTip.xRequire = "再次寻找铁匠可以帮你发掘武器的潜力，寻找小贩激活下一个主线任务"
		end

	end

	--	Main Quest 1
	--	Activate when quest 0 step >= 27
	if curstep >= 27 then
		local curstep1 = ctx:GetQuestStep(1)

		if curstep1 == 0 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "寻找小贩与其对话"
			qx.stTip.xReward = "回城卷*1"
		elseif curstep1 == 1 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "让小贩传送到目的地点"
			--qx.stTip.xReward = "一个回城卷"
		elseif curstep1 == 2 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "找到被半兽战士夺取的一份龙胆草"
			qx.stTip.xReward = "金创药(中量)*6 回城卷*3"
		elseif curstep1 == 3 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "寻找小贩与其对话"
			--qx.stTip.xReward = "一个回城卷"
		elseif curstep1 == 4 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "在半兽人古墓中得到一个蝎子的尾巴，交给小贩"
			--qx.stTip.xReward = "回城卷*1"
		elseif curstep1 == 5 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "将小贩制作好的药粉交给落霞山谷的王老伯(54,52)"
			qx.stTip.xReward = "经验1500"
		elseif curstep1 == 6 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "回去寻找小贩对话"
			--qx.stTip.xReward = "经验1500"
		elseif curstep1 == 7 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "王老伯有些事情想麻烦你，去落霞山谷和他对话"
			--qx.stTip.xReward = "经验1500"
		elseif curstep1 == 8 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "杀死一个半兽人古墓深处的骷髅精灵"
			--qx.stTip.xReward = "经验1500"
		elseif curstep1 == 9 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "回去找王老伯汇报情况并领取奖励"
			if player:GetHeroJob() == 0 then
				qx.stTip.xReward = "经验1500 金币5000 轻型盔甲".." 八荒"
			elseif player:GetHeroJob() == 1 then
				qx.stTip.xReward = "经验1500 金币5000 轻型盔甲".." 海魂"
			elseif player:GetHeroJob() == 2 then
				qx.stTip.xReward = "经验1500 金币5000 轻型盔甲".." 半月"
			end
		elseif curstep1 == 10 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "请王老伯传送到矿窑，带回一本《古墓书籍》交给王老伯"
			qx.stTip.xReward = "神殿卷轴*1"
		elseif curstep1 == 11 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "使用神殿卷轴进入半兽人古墓深处的沃玛神殿，杀死沃玛护卫获得神秘的勋章并交给王老伯(神殿卷轴可在骷髅精灵身上取得)"
			qx.stTip.xReward = "经验50000 金币10000"
		elseif curstep1 == 12 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "带着神秘的勋章，踏上寻找封魔大陆的旅途，并与封魔城主对话"
			qx.stTip.xReward = "荣誉勋章"
		elseif curstep1 == 13 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			local counter = ctx:GetQuestCounter(1)
			if counter <= 49 then
				qx.stTip.xRequire = "进入地牢一层，杀死("..counter.."/50)只钳虫或者蜈蚣"
			else
				qx.stTip.xRequire = "回去向封魔城主汇报任务情况"
			end
			qx.stTip.xReward = "经验50000"
		elseif curstep1 == 14 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "通过封魔城主传送到祖玛大殿，从暗之祖玛弓箭手身上取得封魔密信"
			qx.stTip.xReward = "经验80000 金币10000"
		elseif curstep1 == 15 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "将封魔密信交给在矿窑的神秘学者"
			qx.stTip.xReward = "经验20000 金币2000"
		elseif curstep1 == 16 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "失态紧急，加急将恶魔密信交给封魔城主"
			qx.stTip.xReward = "经验20000"
		elseif curstep1 == 17 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "在矿窑深处的赤月峡谷中获得5个剧毒蛛丝，带回来交给封魔城主"
			qx.stTip.xReward = "经验80000 金币10000"
		elseif curstep1 == 18 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "在赤月峡谷深处从血僵尸身上得到潘多拉宝盒"
			qx.stTip.xReward = "经验30000 金币5000"
		elseif curstep1 == 19 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "从兽人古墓的骷髅精灵，矿窑的尸王，沃玛神殿的沃玛教主，连接通道的白野猪，祖玛大殿的祖玛教主身上取得神器碎片，通过潘多拉宝盒合成上古神器"
			qx.stTip.xReward = "经验150000 金币20000"
		elseif curstep1 == 20 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "杀死从恶魔封印水中召唤出来的恶魔，将掉落物品交给封魔城主"
			qx.stTip.xReward = "经验150000 金币20000"
		elseif curstep1 == 21 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "带着黑色羽毛去见海边小岛的老兵，查明羽毛的来源"
			qx.stTip.xReward = "经验20000"
		elseif curstep1 == 22 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			local counter = ctx:GetQuestCounter(1)
			if counter <= 49 then
				qx.stTip.xRequire = "杀死小岛上的("..counter.."/50)只怪物"
			else
				qx.stTip.xRequire = "向老兵报告情况"
			end
			qx.stTip.xReward = "经验1000000 金币100000"
		elseif curstep1 == 23 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "从熔岩地狱恶魔身上获得熔岩之心"
			qx.stTip.xReward = "经验30000 金币30000"
		elseif curstep1 == 24 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "将熔岩之心带给封印的英雄"
			qx.stTip.xReward = "经验10000"
		elseif curstep1 == 25 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "玛法探秘"
			qx.stTip.xDescript = "您已成长为一位有胆识的英雄，您接下来需要面对来自玛法大陆的各种恶魔，做好准备吧。"
			qx.stTip.xRequire = "任务待续"
			qx.stTip.xReward = "经验10000"
		end
	end

	-- Branch quest 0
	if player:GetHeroLevel() >= 15 then
		local bcurstep = ctx:GetQuestStep(50)
		if bcurstep == 0 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "寻找藏宝图"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "神秘学者丢失了一份矿窑藏宝图，据说是和他装扮一样的僵尸抢走的，请帮他找回这份藏宝图。"
			qx.stTip.xRequire = "与神秘学者对话"
			--qx.stTip.xReward = "经验20000"
		elseif bcurstep == 1 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "寻找藏宝图"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "神秘学者丢失了一份矿窑藏宝图，据说是和他装扮一样的僵尸抢走的，请帮他找回这份藏宝图。"
			qx.stTip.xRequire = "从僵尸身上找回藏宝图"
			qx.stTip.xReward = "经验3000 金币5000"
		elseif bcurstep == 2 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "寻找藏宝图"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "神秘学者丢失了一份矿窑藏宝图，据说是和他装扮一样的僵尸抢走的，请帮他找回这份藏宝图。"
			qx.stTip.xRequire = "与神秘学者对话"
			--qx.stTip.xReward = "经验3000 金币5000"
		elseif bcurstep == 3 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "寻找藏宝图"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "神秘学者丢失了一份矿窑藏宝图，据说是和他装扮一样的僵尸抢走的，请帮他找回这份藏宝图。"
			local counter = ctx:GetQuestCounter(50)
			qx.stTip.xRequire = "消灭("..counter.."/20)只僵尸"
			qx.stTip.xReward = "经验4000 金币5000"
		elseif bcurstep == 4 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "寻找藏宝图"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "神秘学者丢失了一份矿窑藏宝图，据说是和他装扮一样的僵尸抢走的，请帮他找回这份藏宝图。"
			qx.stTip.xRequire = "向神秘学者汇报情况"
			qx.stTip.xReward = "经验4000 金币5000"
		end
	end

	--	Branch quest 1
	if player:GetHeroLevel() >= 40 then
		local bcurstep1 = ctx:GetQuestStep(51)
		if bcurstep1 == 0 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "血魔之血"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "流传着这么一个传说，在三英雄与血魔战斗的时候，圣战拼尽最后一口气，将血魔引入了赤月峡谷深处，在即将消失的时候将此物抛给了法神。据传此物拥有无尽的魔力。"
			qx.stTip.xRequire = "从双头血魔身上取得血魔之血"
			--qx.stTip.xReward = "经验1000"
		elseif bcurstep1 == 1 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "血魔之血"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "流传着这么一个传说，在三英雄与血魔战斗的时候，圣战拼尽最后一口气，将血魔引入了赤月峡谷深处，在即将消失的时候将此物抛给了法神。据传此物拥有无尽的魔力。"
			qx.stTip.xRequire = "寻找天尊询问此事"
			qx.stTip.xReward = "经验1000"
		elseif bcurstep1 == 2 then
			local qx = dlg:PushQuestInfo()
			qx.stSummary.xQuestName = "血魔之血"
			qx.stSummary.nType = 1
			qx.stTip.xDescript = "流传着这么一个传说，在三英雄与血魔战斗的时候，圣战拼尽最后一口气，将血魔引入了赤月峡谷深处，在即将消失的时候将此物抛给了法神。据传此物拥有无尽的魔力。"
			qx.stTip.xRequire = "从赤月恶魔身上取得古老的火把，将其带回给天尊"
			qx.stTip.xReward = "经验250000 金币150000"
		end
	end

	--	Branch quest 2
	if player:GetHeroLevel() >= 55 then
		local mcurstep1 = ctx:GetQuestStep(1)
		local bcurstep2 = ctx:GetQuestStep(52)
		MirLog("Lv"..player:GetHeroLevel().."mstep"..mcurstep1.."bstep"..bcurstep2)
		if mcurstep1 >= 25 then
			if bcurstep2 == 0 then
				local qx = dlg:PushQuestInfo()
				qx.stSummary.xQuestName = "阎魔太子"
				qx.stSummary.nType = 1
				qx.stTip.xDescript = "在寒冰王座深处，隐藏着令人无比胆寒的恶魔，即使是寒冰魔王也要敬畏它三分。它有着深不可测的能力，传说想要接近它的人被它动几根手指就解决了。八卦符咒可以召唤出它身边的侍卫，而这些侍卫已经是这个玛法大陆最可怕的恶魔了。"
				qx.stTip.xRequire = "与异界之门对话获取更多信息"
				--qx.stTip.xReward = "八卦符咒"
			elseif bcurstep2 == 1 then
				local qx = dlg:PushQuestInfo()
				qx.stSummary.xQuestName = "阎魔太子"
				qx.stSummary.nType = 1
				qx.stTip.xDescript = "在寒冰王座深处，隐藏着令人无比胆寒的恶魔，即使是寒冰魔王也要敬畏它三分。它有着深不可测的能力，传说想要接近它的人被它动几根手指就解决了。八卦符咒可以召唤出它身边的侍卫，而这些侍卫已经是这个玛法大陆最可怕的恶魔了。"
				qx.stTip.xRequire = "收集七颗佛珠"
				qx.stTip.xReward = "八卦符咒*1 金条*1"
			end
		end
	end

--	Create End
end



--	游戏捐助
--	id:2首页 id:1关闭 其它id<10000可以快捷跳转到相应的id显示界面
function ShowDonate(ctx, id)
	if id == 2 then
		ctx:AddContextNode(0, 91, "本游戏为免费游戏，若您喜欢本游戏，支持本游戏开发，可以在菜单(按键E)中选择游戏捐助来捐助作者的开发和维护。若您捐赠后不希望出现在本列表中，请发邮件告知。若您需要补全贴吧ID信息或者QQ群名片信息，也请发邮件告知。", 0)
		ctx:AddContextNode(0, 234,  "BackMIR贴吧", 10001)
		ctx:AddContextNode(0, 247,  "捐助游戏作者", 9999)
		ctx:AddContextNode(0, 260,  "查看捐赠列表", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 3 then
		ctx:AddContextNode(0, 26,  "*培 blu***@163.com(神秘男子A)", 0)
		ctx:AddContextNode(0, 39,  "*海亮 831***@163.com", 0)
		ctx:AddContextNode(0, 52,  "*必文 che***@vip.qq.com", 0)
		ctx:AddContextNode(0, 65,  "*宝来 yes***@163.com(百废纸)", 0)
		ctx:AddContextNode(0, 78,  "*红 wh6***@163.com(星海LP)", 0)
		ctx:AddContextNode(0, 91,  "*懿 188****4777", 0)
		ctx:AddContextNode(0, 104,  "*健宇 181****0376(diahycts)", 0)
		ctx:AddContextNode(0, 117,  "*铃 182****5738", 0)
		ctx:AddContextNode(0, 130,  "*超峰 139****8935(1123小猪)", 0)
		ctx:AddContextNode(0, 143,  "*高林 181****6778(慎独)", 0)
		ctx:AddContextNode(0, 156,  "*海洋 che***@sina.com(夜奔Er)", 0)
		ctx:AddContextNode(0, 169,  "*国明 xyx***@163.com", 0)
		ctx:AddContextNode(0, 182,  "*超峰 jid***@163.com", 0)
		ctx:AddContextNode(0, 195,  "*蝶 lee***@sina.com", 0)
		ctx:AddContextNode(0, 208,  "*福君 hua***@126.com", 0)
		ctx:AddContextNode(0, 221,  "*光丰 984***@qq.com(聖游子)", 0)
		ctx:AddContextNode(0, 234,  "*永昌 lvj***@163.com", 0)
		ctx:AddContextNode(0, 247,  "下一页", 4)
		ctx:AddContextNode(0, 260,  "返回主页", 2)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 4 then
		ctx:AddContextNode(0, 26,  "*凯 ssu***@126.com(ssunkaii)", 0)
		ctx:AddContextNode(0, 39,  "*敏强 130****7488", 0)
		ctx:AddContextNode(0, 52,  "*宇阳 a67***@163.com(李魍魉)", 0)
		ctx:AddContextNode(0, 65,  "*金生 wuj***@163.com(莜莜)", 0)
		ctx:AddContextNode(0, 78,  "*绪文 187****7902(zxw75192)", 0)
		ctx:AddContextNode(0, 91,  "*永康 160***@qq.com", 0)
		ctx:AddContextNode(0, 104,  "*昊 wuh***@163.com", 0)
		ctx:AddContextNode(0, 117,  "*帆 370***@qq.com", 0)
		ctx:AddContextNode(0, 130,  "*轶昂 284***@qq.com", 0)
		ctx:AddContextNode(0, 143,  "*长勇 105***@qq.com", 0)
		ctx:AddContextNode(0, 156,  "*宇峰 i***@leafwind.me", 0)
		ctx:AddContextNode(0, 169,  "*海明 275***@qq.com", 0)
		ctx:AddContextNode(0, 182,  "*琉丹 152****1227", 0)
		ctx:AddContextNode(0, 195,  "*佳栋 186****2397(商憧)", 0)
		ctx:AddContextNode(0, 208,  "*仲强 chu***@qq.com(梦在飘逸)", 0)
		ctx:AddContextNode(0, 221,  "*光 139****3776(道士威武)", 0)
		ctx:AddContextNode(0, 234,  "*海鸿 312***@qq.com(Blaveam)", 0)
		ctx:AddContextNode(0, 247,  "下一页", 5)
		ctx:AddContextNode(0, 260,  "返回主页", 2)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 5 then
		ctx:AddContextNode(0, 26,  "*振华 now***@sina.com(冷涟漪)", 0)
		ctx:AddContextNode(0, 39,  "*雪峰 163***@163.com", 0)
		ctx:AddContextNode(0, 52,  "*建平 631***@qq.com", 0)
		ctx:AddContextNode(0, 65,  "*文达 159****1436(乱世狂豪)", 0)
		ctx:AddContextNode(0, 78,  "*航 159****6625", 0)
		ctx:AddContextNode(0, 91,  "*建平 ljp***@126.com(星空)", 0)
		ctx:AddContextNode(0, 104,  "*国栋 905***@qq.com", 0)
		ctx:AddContextNode(0, 117,  "*丽 372***@qq.com", 0)
		ctx:AddContextNode(0, 130,  "*大平 414***@qq.com", 0)
		ctx:AddContextNode(0, 143,  "*红 189****1224", 0)
		ctx:AddContextNode(0, 156,  "*硕 hiv***@163.com", 0)
		ctx:AddContextNode(0, 169,  "*志荣 liz***@sohu.com(闲人李1314)", 0)
		ctx:AddContextNode(0, 182,  "*毅 138****5350", 0)
		ctx:AddContextNode(0, 195,  "*华伟 159****9849", 0)
		ctx:AddContextNode(0, 208,  "*沙沙 104***@qq.com", 0)
		ctx:AddContextNode(0, 221,  "*小虎 134****7024", 0)
		ctx:AddContextNode(0, 234,  "*海东 187****1008", 0)
		ctx:AddContextNode(0, 260,  "返回主页", 2)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 6 then
		ctx:AddContextNode(0, 26,  "长空一风", 0)
		ctx:AddContextNode(0, 39,  "*领 151****7762", 0)
		ctx:AddContextNode(0, 52,  "*会聪 136****3961(浪漫撒加)", 0)
		ctx:AddContextNode(0, 65,  "*惠宝 189****8669", 0)
		ctx:AddContextNode(0, 78,  "*光丰 138****2212", 0)
		ctx:AddContextNode(0, 91,  "*丰藤 139****2609", 0)
		ctx:AddContextNode(0, 104,  "*刘永 152****5159", 0)
		ctx:AddContextNode(0, 117,  "*旬 139****7085", 0)
		ctx:AddContextNode(0, 130,  "*艳辉 147***@qq.com", 0)
		ctx:AddContextNode(0, 143,  "*兵 182****0219", 0)
		ctx:AddContextNode(0, 156,  "*喆 515***@qq.com", 0)
		ctx:AddContextNode(0, 169,  "*凤明 136****9638(淡漠流年)", 0)
		ctx:AddContextNode(0, 182,  "*博 372***@qq.com(神奇的豆子)", 0)
		ctx:AddContextNode(0, 195,  "*建平 631***@qq.com", 0)
		--[[
		ctx:AddContextNode(0, 26,  "*凯 ssu***@126.com(ssunkaii)", 0)
		ctx:AddContextNode(0, 39,  "*敏强 130****7488", 0)
		ctx:AddContextNode(0, 52,  "*宇阳 a67***@163.com(李魍魉)", 0)
		ctx:AddContextNode(0, 65,  "*金生 wuj***@163.com(莜莜)", 0)
		ctx:AddContextNode(0, 78,  "*绪文 187****7902(zxw75192)", 0)
		ctx:AddContextNode(0, 91,  "*永康 160***@qq.com", 0)
		ctx:AddContextNode(0, 104,  "*昊 wuh***@163.com", 0)
		ctx:AddContextNode(0, 117,  "*帆 370***@qq.com", 0)
		ctx:AddContextNode(0, 130,  "*轶昂 284***@qq.com", 0)
		ctx:AddContextNode(0, 143,  "*长勇 105***@qq.com", 0)
		ctx:AddContextNode(0, 156,  "*宇峰 i***@leafwind.me", 0)
		ctx:AddContextNode(0, 169,  "*海明 275***@qq.com", 0)
		ctx:AddContextNode(0, 182,  "*琉丹 152****1227", 0)
		ctx:AddContextNode(0, 195,  "*佳栋 186****2397(商憧)", 0)
		ctx:AddContextNode(0, 208,  "*仲强 chu***@qq.com(梦在飘逸)", 0)
		ctx:AddContextNode(0, 221,  "*光 139****3776(道士威武)", 0)
		ctx:AddContextNode(0, 234,  "*海鸿 312***@qq.com(Blaveam)", 0)
		ctx:AddContextNode(0, 247,  "下一页", 5)
		ctx:AddContextNode(0, 260,  "返回主页", 2)
		ctx:AddContextNode(0, 273,  "关闭", 1)
		]]
	elseif id == 9999 then
		ctx:AddContextNode(0, 0, "BackMIR为一款倾注了作者一年多心血的独立游戏，独立制作，采用了热血传奇的素材（素材版权为盛大所有）。本游戏为免费游戏，不存在任何购买道具，购买等级，购买VIP等等的盈利方式，本游戏为作者学习之心血，无任何盈利行为，任何人将此游戏用于盈利目的均与作者无关。若您支持本游戏的开发及维护，可使用下面方式进行捐助。", 0)
		ctx:AddContextNode(0, 247,  "返回主页", 2)
		ctx:AddContextNode(0, 260,  "支付宝捐助", 10000)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	end
end



--	游戏帮助窗口
--	id:2首页 id:1关闭 其它id<10000可以快捷跳转到相应的id显示界面
function ShowHelp(ctx, id)
	if id == 2 then
		ctx:AddContextNode(0, 0, " ", 0)
		ctx:AddContextNode(72, 39, "[基本操作快捷键]", 0)
		ctx:AddContextNode(0, 52, "背包[F9]", 0)
		ctx:AddContextNode(80, 52, "人物[F10]", 0)
		ctx:AddContextNode(160, 52, "技能[F11]", 0)
		ctx:AddContextNode(0, 65,  "大地图[TAB]", 0)
		ctx:AddContextNode(80, 65, "任务[Q]", 0)
		ctx:AddContextNode(160, 65, "锻造[M]", 0)
		ctx:AddContextNode(0, 91, "本游戏为免费游戏，若您喜欢本游戏，支持本游戏开发，可以在菜单中选择游戏捐助来捐助作者的开发和维护", 0)
		ctx:AddContextNode(0, 234,  "BackMIR贴吧", 10001)
		ctx:AddContextNode(0, 247,  "捐助游戏作者", 9999)
		ctx:AddContextNode(0, 260,  "游戏帮助索引", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id ==  3 then
		ctx:AddContextNode(78, 0, "[游戏帮助索引]", 0)
		ctx:AddContextNode(0, 26, "如何捡取物品", 4)
		ctx:AddContextNode(80, 26, "非绑定物品如何给小号", 5)
		ctx:AddContextNode(0, 39, "如何卖东西", 6)
		ctx:AddContextNode(80, 39, "如何升级技能", 7)
		ctx:AddContextNode(160, 39, "如何使用技能", 8)
		ctx:AddContextNode(0, 52, "如何隔位刺杀", 9)
		ctx:AddContextNode(80, 52, "如何切换全屏", 10)
		ctx:AddContextNode(160, 52, "如何联机游戏", 11)
		ctx:AddContextNode(0, 65, "如何组队", 12)
		ctx:AddContextNode(80, 65, "使用传送戒指", 13)
		ctx:AddContextNode(160, 65, "地面物品叠加", 14)
		ctx:AddContextNode(0, 78, "存档失败", 15)
		ctx:AddContextNode(80, 78, "分解合成装备", 16)
		ctx:AddContextNode(160, 78, "精华石乾坤石", 17)
		ctx:AddContextNode(0, 91, "精英怪首领怪", 18)
		ctx:AddContextNode(80, 91, "关于藏宝图", 19)
		ctx:AddContextNode(160, 91, "批量买消耗品", 20)
		ctx:AddContextNode(0, 104, "查看玩家装备", 21)
		ctx:AddContextNode(0, 247,  "下一页", 3)
		ctx:AddContextNode(0, 260,  "返回主页", 2)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 4 then
		ctx:AddContextNode(0, 0, "人物站在物品上方，按空格键进行捡取操作，或者按住Ctrl，然后点击地面物品即可捡取。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 5 then
		ctx:AddContextNode(0, 0, "将非绑定物品扔在地上，关闭客户端，不要关闭服务器，上另一个号，在相同地方进行捡取操作，注意3分钟物品将自动消失。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 6 then
		ctx:AddContextNode(0, 0, "选中背包物品，将背包物品拖入商店窗口即可，商店窗口价格显示即为物品卖出的价格。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 7 then
		ctx:AddContextNode(0, 0, "到达一定等级后，学习相同书籍即可升级相应技能，即3级技能需要特定书籍3本。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 8 then
		ctx:AddContextNode(0, 0, "学习技能后，打开技能窗口，点击技能图标，设置F1-F8技能使用按键，按相应按键即可使用。部分主动技能无需释放，也无法设置快捷键。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 9 then
		ctx:AddContextNode(0, 0, "开启刺杀剑法后，持续按住Shift按键，点击左键可强行攻击。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 10 then
		ctx:AddContextNode(0, 0, "进入游戏后，按住Alt，然后按Enter键即可切换全屏与窗口模式。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 11 then
		ctx:AddContextNode(0, 0, "若您需要加入其它玩家的游戏服务器，只需在登陆器中填写客户端IP，勾掉启动服务器和单机模式启动，启动游戏即可。若您需要自己开启游戏服务器，非路由器玩家直接填写外网IP即可，路由器用户需开启路由器的DMZ或端口映射，服务器IP填写局域网IP，勾选启动服务器，勾掉单机模式启动，在客户端IP中填写自己的局域网IP启动即可。将外网IP告诉朋友即可让好友加入自己的游戏。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 12 then
		ctx:AddContextNode(0, 0, "由于时间和素材限制，只开放了简易组队模型。输入@createteam即可开启新的队伍，要添加队员，则需要在同一张地图中选中另一个玩家，按Alt+A即可添加别的玩家进入队伍，要删除成员，则选中玩家后按Alt+K即可踢出某位玩家。假如想自己退出目前的队伍，输入@quitteam即可。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 13 then
		ctx:AddContextNode(0, 0, "得到传送戒指之后，将其戴在身上，输入@move x,y即可，冷却时间15秒，部分地图无法进行传送。传送戒指可以在各大教主BOSS身上取得，也可以通过藏宝图中的宝箱取得。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 14 then
		ctx:AddContextNode(0, 0, "当地面物品叠加后，在物品处按住Alt键即可显示该坐标所有叠加的物品。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 15 then
		ctx:AddContextNode(0, 0, "当提示数据为空存档失败时，请检查背包物品，有无法保存属性的，是无法存档的，要存档请将无法存档的物品扔在地上，然后再进行存档。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 16 then
		ctx:AddContextNode(0, 0, "当您打到一件附加属性为攻击、魔法、道术的装备时候，即可将此装备分解。打开分解窗口，将装备放到最下面一格中，将会提示该分解将得到的物品，分解物品为该装备等级向下一级的精华石。当您获得和精华石相同等级的装备时候，打开装备分解窗口，按右箭头切换到合成界面，将石头放入上方5个格子中的一个中，装备放入最下方格子，即可进行装备升级。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 17 then
		ctx:AddContextNode(0, 0, "分解极品装备将得到精华石，精华石可以升级和精华石级别相同或以下的装备。20个精华石可以在装备合成界面中向上合成为一个高级别精华石。乾坤石适用于极品装备，会将所加的极品点数随机再分配一次。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 18 then
		ctx:AddContextNode(0, 0, "各地图均有一定几率刷出精英怪和首领怪，该怪物掉率装备几率，掉落装备出现极品几率均比普通怪高很多。若需关闭精英怪物生成功能，在游戏目录中找到cfg.ini，打开它，编辑GENELITEMONS=0即可。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 19 then
		ctx:AddContextNode(0, 0, "右键使用藏宝图将可在特定地图，特定坐标开启隐藏地图，若无法开启，则会在屏幕左上角提示正确位置。藏宝图可在祖玛教主等BOSS身上取得。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 20 then
		ctx:AddContextNode(0, 0, "买消耗品时候，如金创药，魔法药及各种卷轴时，按住Ctrl再买即可一次购买一组(20个)消耗品。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 21 then
		ctx:AddContextNode(0, 0, "选中一个玩家，按住Alt的同时按一下V即可查看对方玩家装备。", 0)
		ctx:AddContextNode(0, 260,  "返回索引主页", 3)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 9998 then
		ctx:AddContextNode(0, 260,  "返回主页", 2)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	elseif id == 9999 then
		ctx:AddContextNode(0, 0, "BackMIR为一款倾注了作者一年多心血的独立游戏，独立制作，采用了热血传奇的素材（素材版权为盛大所有）。本游戏为免费游戏，不存在任何购买道具，购买等级，购买VIP等等的盈利方式，本游戏为作者学习之心血，无任何盈利行为，任何人将此游戏用于盈利目的均与作者无关。若您支持本游戏的开发及维护，可使用下面方式进行捐助。", 0)
		ctx:AddContextNode(0, 247,  "返回主页", 2)
		ctx:AddContextNode(0, 260,  "支付宝捐助", 10000)
		ctx:AddContextNode(0, 273,  "关闭", 1)
	end
end
