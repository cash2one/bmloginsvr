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
	switch opcode {
	case LSControlProto.Opcode_PKG_RegistAccountReq:
		{
			registAccountReq := &LSControlProto.LSCRegistAccountReq{}
			err = proto.Unmarshal(msg[5+headlen:], registAccountReq)
			if err != nil {
				return
			}
			this.OnRegistAccountReq(registAccountReq)
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

func (this *ServerUser) OnRegistAccountReq(req *LSControlProto.LSCRegistAccountReq) {
	account := req.GetAccount()
	password := req.GetPassword()

	//	check
	reg, _ := regexp.Compile("^[A-Za-z0-9]+$")
	var ret = false

	if reg.MatchString(account) && reg.MatchString(password) {
		ret = true
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
