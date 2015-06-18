gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)
gNPCTable[10005] =
function(ctx, questid, queststep)
	if questid == 0 and queststep == 0 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "我看起来很像僵尸吧？为了学习这里的武功秘籍，我特意学了易容术，我容易么我。不过还是很值得的，这儿的武功秘籍相当之多，那些死去的矿工们虽然变成了僵尸，但是智力也低下了，已经分不清我是僵尸还是人类了。", 0)
		ctx:AddContextNode(30, 240, "我想出去", 2)
		ctx:AddContextNode(30, 255, "聊聊", 3)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "这些僵尸很可恶，虽说我看上去也是僵尸了，可是还是抢走了我的矿窑藏宝图，可以的话帮我找回来吧。我依稀记得是被跟我装束一样的僵尸给抢走了，太可恶了！", 0)
		ctx:AddContextNode(30, 240, "当然没问题", 4)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "十分感谢你！我等你的好消息", 0)
		ctx:AddContextNode(30, 240, "我找到了", 5)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "我的确有事想要你帮忙，不过你还太年轻，修炼到15级再来找我吧！", 0)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "还没找到我的矿窑藏宝图呀！", 0)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 5 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "这的确是我的藏宝图！太感谢你了，这点东西就送给你了。", 0)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 6 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "矿窑藏宝图是找到了，可是可恶的僵尸在我的藏宝图上打了个洞，真是的，越想越生气，你能帮我去消灭20只僵尸解恨吗？", 0)
		ctx:AddContextNode(30, 240, "可以", 6)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 7 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "自己小心！", 0)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 8 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "很好，这是给你的奖赏。", 0)
		ctx:AddContextNode(30, 240, "领取", 7)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 9 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "年轻人，你很有前途，我相信我们会再见面的。", 0)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 10 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "这封信太可怕了，怪不得我这里的矿窑也出现了一些异象，在僵尸出没的地方居然有时候能听到蜘蛛的声音。赶紧把这封信给城主送过去吧，我们得破坏恶魔们的阴谋。", 0)
		ctx:AddContextNode(30, 255, "关闭", 1)

	elseif questid == 0 and queststep == 100 then
		ctx:AddContextNode(0, 0, "神秘学者:", 0)
		ctx:AddContextNode(0, 20, "这地方真臭。", 0)
		ctx:AddContextNode(30, 255, "关闭", 1)

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
