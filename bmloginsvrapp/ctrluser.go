package main

import (
	"LSControlProto"
	"bytes"
	"code.google.com/p/goprotobuf/proto"
	"encoding/binary"
	"log"
	"regexp"
)

func (this *ServerUser) OnCtrlMsg(msg []byte) {
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
	if opcode != LSControlProto.Opcode_PKG_HeartBeat {
		log.Println("Ctrl msg[", opcode, "]")
	}

	if !this.ctrlverify {
		//	verify
		if opcode == LSControlProto.Opcode_PKG_CtrlVerifyReq {
			ctrlVerifyReq := &LSControlProto.LSCCtrlVerifyReq{}
			err = proto.Unmarshal(msg[oft_body_start:], ctrlVerifyReq)
			if err != nil {
				log.Println("proto unmarshal error.", err)
				return
			}

			log.Println("Verify ctrl terminal[", ctrlVerifyReq.GetVerifycode(), "]")

			ret := &LSControlProto.LSCCtrlVerifyAck{}
			ret.Result = proto.Bool(true)

			if ControlValid(ctrlVerifyReq.GetVerifycode()) {
				this.ctrlverify = true
				log.Println("pass[", ctrlVerifyReq.GetVerifycode(), "]")
			} else {
				ret.Result = proto.Bool(false)
				log.Println("invalid terminal[", ctrlVerifyReq.GetVerifycode(), "]")
				//this.conn.GetInternalConn().Close()
			}

			data, _ := proto.Marshal(ret)
			this.SendProtoBuf(uint32(LSControlProto.Opcode_PKG_CtrlVerifyAck), data)
		}
		return
	}

	switch opcode {
	case LSControlProto.Opcode_PKG_RegistAccountReq:
		{
			registAccountReq := &LSControlProto.LSCRegistAccountReq{}
			err = proto.Unmarshal(msg[oft_body_start:], registAccountReq)
			if err != nil {
				return
			}
			this.OnRegistAccountReq(registAccountReq)
		}
	case LSControlProto.Opcode_PKG_RegistAccountWithInfoReq:
		{
			registAccountWithInfoReq := &LSControlProto.RSRegistAccountReq{}
			err = proto.Unmarshal(msg[oft_body_start:], registAccountWithInfoReq)
			if err != nil {
				return
			}
			this.OnRsRegistAccountReq(registAccountWithInfoReq)
		}
	case LSControlProto.Opcode_PKG_ModifyPasswordReq:
		{
			modifyPasswordReq := &LSControlProto.RSModifyPasswordReq{}
			err = proto.Unmarshal(msg[oft_body_start:], modifyPasswordReq)
			if err != nil {
				return
			}
			this.OnRsMofifyPassword(modifyPasswordReq)
		}
	}
}

func (this *ServerUser) SendProtoBuf(opcode uint32, msg []byte) bool {
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
	this.conn.WriteMsg(buf.Bytes())

	return true
}

func (this *ServerUser) OnRsRegistAccountReq(req *LSControlProto.RSRegistAccountReq) {
	account := req.GetAccount()
	password := req.GetPassword()

	//	check
	reg, _ := regexp.Compile("^[A-Za-z0-9]+$")
	var ret = false

	if reg.MatchString(account) && reg.MatchString(password) {
		ret = true
	}

	if ret {
		//	regist
		if len(account) > 15 || len(password) > 15 {
			ret = false
		} else {
			users := make([]UserAccountInfo, 1)
			users[0].account = account
			users[0].password = password

			if !dbUserAccountExist(g_DBUser, account) {
				if !dbInsertUserAccountInfo(g_DBUser, users) {
					ret = false
				}
			} else {
				ret = false
			}
		}
	}

	ack := &LSControlProto.RSRegistAccountAck{}
	ack.Result = proto.Bool(ret)
	ack.Account = proto.String(req.GetAccount())
	ack.Mail = proto.String(req.GetMail())
	data, err := proto.Marshal(ack)

	if err != nil {
		log.Println(err)
		return
	}

	this.SendProtoBuf(uint32(LSControlProto.Opcode_PKG_RegistAccountWithInfoAck),
		data)
}

func (this *ServerUser) OnRegistAccountReq(req *LSControlProto.LSCRegistAccountReq) {
	account := req.GetAccount()
	password := req.GetPassword()

	//	check
	reg, _ := regexp.Compile("^[A-Za-z0-9]+$")
	var ret = false

	if reg.MatchString(account) && reg.MatchString(password) {
		ret = true
	}

	if ret {
		//	regist
		if len(account) > 15 || len(password) > 15 {
			ret = false
		} else {
			users := make([]UserAccountInfo, 1)
			users[0].account = account
			users[0].password = password

			if !dbUserAccountExist(g_DBUser, account) {
				if !dbInsertUserAccountInfo(g_DBUser, users) {
					ret = false
				}
			} else {
				ret = false
			}
		}
	}

	ack := &LSControlProto.LSCRegistAccountAck{}
	ack.Result = proto.Bool(ret)
	data, err := proto.Marshal(ack)

	if err != nil {
		log.Println(err)
		return
	}

	this.SendProtoBuf(uint32(LSControlProto.Opcode_PKG_RegistAccountAck),
		data)
}

func (this *ServerUser) OnRsMofifyPassword(req *LSControlProto.RSModifyPasswordReq) {
	account := req.GetAccount()
	password := req.GetPassword()

	rsp := &LSControlProto.RSModifyPasswordRsp{}
	rsp.Account = proto.String(account)

	if !dbUserAccountExist(g_DBUser, account) {
		rsp.Result = proto.Bool(false)
		data, err := proto.Marshal(rsp)
		if err != nil {
			log.Println(err)
			return
		}

		this.SendProtoBuf(uint32(LSControlProto.Opcode_PKG_ModifyPasswordRsp), data)
		return
	}

	if !dbUpdateUserAccountPassword(g_DBUser, account, password) {
		rsp.Result = proto.Bool(false)
		data, err := proto.Marshal(rsp)
		if err != nil {
			log.Println(err)
			return
		}

		this.SendProtoBuf(uint32(LSControlProto.Opcode_PKG_ModifyPasswordRsp), data)
		return
	}

	rsp.Result = proto.Bool(true)
	data, err := proto.Marshal(rsp)
	if err != nil {
		log.Println(err)
		return
	}

	this.SendProtoBuf(uint32(LSControlProto.Opcode_PKG_ModifyPasswordRsp), data)
	return
}
