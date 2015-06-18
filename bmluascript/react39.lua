gNPCTable = {}

--	默认按钮 1:退出对话框  ContextNode属性:0:文字 function(ctx, questid, queststep)

--	10020秋言
gNPCTable[10020] =
function(ctx, questid, queststep)
	if questid == 0 and queststep == 0 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "曾经我驰骋江湖，却出现了不知名的恶魔。如今我选择保卫这里，可是我年事已高，还能守护多久呢？", 0)
		ctx:AddContextNode(30, 240, "伏魔", 3)
		ctx:AddContextNode(30, 255, "我会帮助你", 2)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 1 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "我只能帮助那些只有拥有伏魔令的英雄，请你在拥有了伏魔令后再来找我吧", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 2 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "请找队长来跟我交谈", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 3 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "这个伏魔令似乎损坏了", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 4 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "伏魔地图已经满员了，请稍等", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 5 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "您的队友们不在身边", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 5 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "无法使用伏魔令", 0)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 10 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "你可以挑战 伏魔令(赤) 地图", 0)
		ctx:AddContextNode(30, 255, "挑战", 501)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 11 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "你可以挑战 伏魔令(阎) 地图", 0)
		ctx:AddContextNode(30, 255, "挑战", 502)
		ctx:AddContextNode(30, 270, "关闭", 1)

	elseif questid == 0 and queststep == 999 then
		ctx:AddContextNode(0, 0, "司徒秋言:", 0)
		ctx:AddContextNode(0, 20, "曾经我驰骋江湖，却出现了不知名的恶魔。如今我选择保卫这里，可是我年事已高，还能守护多久呢？", 0)
		ctx:AddContextNode(30, 240, "伏魔", 3)
		ctx:AddContextNode(30, 255, "我会帮助你", 2)
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
