package main

import (
	"LSControlProto"
	"bytes"
	"client"
	"code.google.com/p/goprotobuf/proto"
	"encoding/binary"
	"log"
	"time"
)

const (
	CHANEVENT_SENDMAIL = iota
	CHANEVENT_DBOP_SELECTBYMAIL
	CHANEVENT_DBOP_INSERT
)

type goChanEvent struct {
	command   int
	arguments []string
	argument  interface{}
	retChan   chan bool
}

var (
	g_clientRegisted bool = false
	g_clientVerified bool = false
)

func go_handleEvent() {
	log.Println("go_handleEvent activated.")
	defer log.Println("handleEvent dead..")

	g_clientVerified = false
	g_clientRegisted = false

	registerToLS()

	for {
		select {
		case evt := <-g_Client.EvtHandler.GetEventQueue():
			{
				if !processSocketEvent(evt) {
					return
				}
			}
		case evt := <-g_ChanEvent:
			{
				if !processMainChanEvent(evt) {
					return
				}
			}
		case <-time.After(time.Second * 10):
			{
				sendHeartBeat()
			}
		}
	}
}

func sendHeartBeat() {
	if g_clientRegisted &&
		g_clientVerified {
		pkg := &LSControlProto.RSHeartBeat{}
		pkg.Seq = proto.Uint32(1)
		data, _ := proto.Marshal(pkg)
		SendProtoBuf(uint32(LSControlProto.Opcode_PKG_HeartBeat), data)
	}
}

func registerToLS() {
	//serverid 4bytes;verifycode 4bytes;addrlen 1byte;addr addrlen
	var serverID uint16 = 101
	var verifyCode uint32 = 1
	addr := "localhost"
	var addrlen uint8 = uint8(len(addr))
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.LittleEndian, &serverID)
	binary.Write(buf, binary.LittleEndian, &verifyCode)
	binary.Write(buf, binary.LittleEndian, &addrlen)
	binary.Write(buf, binary.LittleEndian, []byte(addr))
	client.WriteMsgLittleEndian(g_Client.Conn, 10001, buf.Bytes())

	//	verify
	pkg := &LSControlProto.LSCCtrlVerifyReq{}
	pkg.Verifycode = proto.String("admin_rs")
	data, _ := proto.Marshal(pkg)
	SendProtoBuf(uint32(LSControlProto.Opcode_PKG_CtrlVerifyReq), data)
}

func processSocketEvent(evt *client.ConnEvent) bool {
	switch evt.Evtid {
	case client.CONNEVT_DISCONNECT:
		{
			log.Println("server disconnected.")
			g_clientRegisted = false
			g_clientVerified = false
			return false
		}
	case client.CONNEVT_READREADY:
		{
			handleLSMsg(evt)
		}
	}

	return true
}

func handleLSMsg(evt *client.ConnEvent) {
	if !g_clientRegisted {
		//	第一个消息是注册成功or失败消息
		buf := bytes.NewBuffer(evt.Msg)
		var pkglen uint32 = 0
		binary.Read(buf, binary.LittleEndian, &pkglen)
		var op uint32 = 0
		binary.Read(buf, binary.LittleEndian, &op)

		log.Println("raw op ", op)

		if op == 10003 {
			var ret uint8 = 0
			binary.Read(buf, binary.LittleEndian, &ret)

			if ret != 0 {
				g_clientRegisted = true
				log.Println("rs register success!")
			} else {
				log.Println("rs register failed.")
			}
		}

		return
	}

	msg := evt.Msg
	buf := bytes.NewBuffer(msg[0:5])
	buf.Next(4)
	var headlen uint8
	binary.Read(buf, binary.LittleEndian, &headlen)
	//	Read head
	head := &LSControlProto.LSCHead{}
	err := proto.Unmarshal(msg[5:5+headlen], head)
	if err != nil {
		log.Println("Failed to unmarshal proto head")
		return
	}

	//	dispatch
	defer func() {
		except := recover()
		if except != nil {
			log.Println(except)
		}

		if err != nil {
			log.Println(err)
		}
	}()

	opcode := LSControlProto.Opcode(head.GetOpcode())
	var oft_body_start int = 5 + int(headlen)
	log.Println("Ctrl msg[", opcode, "]")

	switch opcode {
	case LSControlProto.Opcode_PKG_CtrlVerifyAck:
		{
			ctrlVerifyAck := &LSControlProto.LSCCtrlVerifyAck{}
			err = proto.Unmarshal(msg[oft_body_start:], ctrlVerifyAck)
			if err != nil {
				return
			}
			onMsgCtrlVerifyAck(ctrlVerifyAck)
		}
	case LSControlProto.Opcode_PKG_RegistAccountWithInfoAck:
		{
			registAccountAck := &LSControlProto.RSRegistAccountAck{}
			err = proto.Unmarshal(msg[oft_body_start:], registAccountAck)
			if err != nil {
				return
			}
			onMsgRegistAccountAck(registAccountAck)
		}
	}
}

func onMsgCtrlVerifyAck(ack *LSControlProto.LSCCtrlVerifyAck) {
	if ack.GetResult() {
		g_clientVerified = true
		log.Println("rs verify success!")
	} else {
		log.Println("rs verify failed.")
	}
}

func onMsgRegistAccountAck(ack *LSControlProto.RSRegistAccountAck) {
	//	mailTitle := "您好 " + ack.GetAccount() + ":您的BackMIR账户注册结果"
	if ack.GetResult() {
		//	success
		retStr := "您好，您的BackMIR账户已经注册成功\r\n"
		retStr += "您的登录账户为:" + ack.GetAccount() + "\r\n\r\n"
		retStr += "BackMIR服务器地址为:" + g_userLsAddress + "，祝您游戏愉快\r\n\r\n"
		retStr += "假设本游戏给您带来的乐趣，您可以选择捐助作者来支持作者的开发和维护，捐赠信息:支付宝sryan@qq.com\r\n"
		retStr += "官方QQ群: 238813170"
		//SendMail(g_mailAccount, g_mailPassword, g_smtpAddress, ack.GetMail(), mailTitle, retStr, "text")

		if !dbUpdateAccountByMail(g_DBUser, ack.GetMail(), ack.GetAccount()) {
			log.Println("can't update account by mail!!")
		}
	} else {
		//	failed
		//SendMail(g_mailAccount, g_mailPassword, g_smtpAddress, ack.GetMail(), mailTitle, "您的账户注册失败，可能的原因：\r\n1.该账户名已被注册\r\n2.账户密码只能包含数字和字母\r\n3.用户名和密码需在20个字符长度内\r\n\r\n请更换账户再次尝试", "text")
	}
}

func processMainChanEvent(evt *goChanEvent) bool {
	if evt.command == CHANEVENT_SENDMAIL {
		if len(evt.arguments) == 3 {
			err := SendMail(g_mailAccount, g_mailPassword, g_smtpAddress, evt.arguments[0], evt.arguments[1], evt.arguments[2], "text")
			if evt.retChan != nil {
				if err != nil {
					evt.retChan <- false
				} else {
					evt.retChan <- true
				}
			}
		}
	} else if evt.command == CHANEVENT_DBOP_SELECTBYMAIL {
		ret := false

		var userInfo *UserRegKeyInfo = nil

		if len(evt.arguments) == 1 &&
			len(evt.arguments[0]) != 0 &&
			nil != evt.argument {
			castOk := false
			userInfo, castOk = evt.argument.(*UserRegKeyInfo)
			if castOk &&
				nil != userInfo {
				ok, _ := dbGetUserRegKeyInfoByMail(g_DBUser, evt.arguments[0], userInfo)
				if ok {
					ret = true
				}
			}
		}

		if evt.retChan != nil {
			evt.retChan <- ret
		}
	} else if evt.command == CHANEVENT_DBOP_INSERT {
		ret := false

		if nil != evt.argument {
			userInfo, castOk := evt.argument.(*UserRegKeyInfo)
			if userInfo != nil &&
				castOk {
				ok := dbInsertUserRegKey(g_DBUser, userInfo)
				if ok {
					ret = true
				}
			}

			if evt.retChan != nil {
				evt.retChan <- ret
			}
		}
	}
	return true
}

func SendProtoBuf(opcode uint32, msg []byte) bool {
	//	get protobuf head data
	buf := new(bytes.Buffer)
	head := &LSControlProto.LSCHead{}
	head.Opcode = proto.Uint32(opcode)
	headbuf, err := proto.Marshal(head)
	if err != nil {
		log.Println(err)
		return false
	}

	//	calc size
	var headlength uint8
	headlength = uint8(len(headbuf))
	var msglength uint32
	msglength = uint32(4 + 1 + len(headbuf) + len(msg))

	//	send message
	binary.Write(buf, binary.BigEndian, &msglength)
	binary.Write(buf, binary.LittleEndian, &headlength)
	buf.Write(headbuf)
	buf.Write(msg)
	g_Client.Conn.WriteMsg(buf.Bytes())

	return true
}
