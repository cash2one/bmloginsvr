gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字
--	10003 小宝
gNPCTable[10002] =
function(ctx, questid, queststep)

	MirLog("questid=="..questid.."step=="..queststep)

	if questid == 0 and queststep == 0 then
		ctx:AddContextNode(0, 0, "小宝:", 0)
		ctx:AddContextNode(0, 20, "本来想来找宝藏了，发现这些恐怖的骷髅太强大了，根本打不过，保命之际学会了打坐护身，现在正在修炼着呢，这些骷髅都不敢靠近我。", 0)
		ctx:AddContextNode(30, 240, "聊聊", 2)
		ctx:AddContextNode(120, 240, "没事", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:AddContextNode(0, 0, "小宝:", 0)
		ctx:AddContextNode(0, 20, "原来是我父亲拜托你来找我的啊，那请你回去帮我报个平安，我还想在这里修炼修炼。", 0)
		ctx:AddContextNode(30, 240, "我知道了", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:AddContextNode(0, 0, "小宝:", 0)
		ctx:AddContextNode(0, 20, "险恶的环境可以激发潜能。", 0)
		ctx:AddContextNode(30, 240, "是啊", 1)

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
