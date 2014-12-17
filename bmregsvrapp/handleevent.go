package main

import (
	"LSControlProto"
	"bytes"
	"client"
	"code.google.com/p/goprotobuf/proto"
	"encoding/binary"
	"log"
)

const (
	CHANEVENT_SENDMAIL = iota
)

type goChanEvent struct {
	command   int
	arguments []string
	retChan   chan bool
}

var (
	clientRegisted bool
)

func go_handleEvent() {
	log.Println("go_handleEvent activated.")
	defer log.Println("handleEvent dead..")

	clientRegisted = false

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
		}
	}
}

func processSocketEvent(evt *client.ConnEvent) bool {
	switch evt.Evtid {
	case client.CONNEVT_DISCONNECT:
		{
			log.Println("server disconnected.")
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
	case LSControlProto.Opcode_PKG_RegistAccountWithInfoAck:
		{
			registAccountAck := &LSControlProto.LGSRegistAccountAck{}
			err = proto.Unmarshal(msg[oft_body_start:], registAccountAck)
			if err != nil {
				return
			}
			onMsgRegistAccountAck(registAccountAck)
		}
	}
}

func onMsgRegistAccountAck(ack *LSControlProto.LGSRegistAccountAck) {
	if ack.GetResult() {
		//	success
	} else {
		//	failed
	}
}

func processMainChanEvent(evt *goChanEvent) bool {
	if evt.command == CHANEVENT_SENDMAIL {
		if len(evt.arguments) == 3 {
			err := SendMail(g_mailAccount, g_mailPassword, g_smtpAddress, evt.arguments[0], evt.arguments[1], evt.arguments[2], "text")
			if err != nil {
				evt.retChan <- false
			} else {
				evt.retChan <- true
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
