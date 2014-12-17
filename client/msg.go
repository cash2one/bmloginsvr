package client

import (
	"bytes"
	"encoding/binary"
	"log"
)

type IMsgReader interface {
	IsMsgValid() bool
	SetDataSource([]byte)
	ReadMsgLength() uint32
	ReadMsgOpCode() uint32
}

type DefaultMsgReader struct {
	msg []byte
}

func (this *DefaultMsgReader) SetDataSource(src []byte) {
	this.msg = src
}

func (this *DefaultMsgReader) ReadMsgLength() uint32 {
	if nil == this.msg {
		return 0
	}
	var length uint32 = 0
	err := binary.Read(bytes.NewBuffer(this.msg[0:CONS_MSGHEAD_LENGTH]), binary.BigEndian, &length)
	if err != nil {
		log.Println("Read msg length err...Error[", err, "]")
	}
	return length
}

func (this *DefaultMsgReader) ReadMsgOpCode() uint32 {
	if nil == this.msg {
		return 0
	}
	if len(this.msg) < 8 {
		log.Println("Err!!A msg format invalid...")
		return 0
	}
	var opcode uint32 = 0
	err := binary.Read(bytes.NewBuffer(this.msg[4:8]), binary.LittleEndian, &opcode)
	if err != nil {
		log.Println("Read msg opcode err...Error[", err, "]")
	}
	return opcode
}

func (this *DefaultMsgReader) IsMsgValid() bool {
	if nil == this.msg {
		return false
	}
	if len(this.msg) < 8 {
		return false
	}

	return true
}
