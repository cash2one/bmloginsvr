package server

import (
	"log"
	"net"
)

type Connection struct {
	conn net.Conn
	tag  uint32
}

const (
	CONNEVT_UNDEFINE   = 0x0
	CONNEVT_CONNECT    = 0x1
	CONNEVT_DISCONNECT = 0x2
	CONNEVT_CUSTOM     = 0x10
)

const (
	CONS_MSGHEAD_LENGTH = 4
)

type ConnEvent struct {
	Evtid uint32
	Conn  Connection
	Msg   []byte
}

type DefaultServerHandler struct {
	connEventsCh chan *ConnEvent
}

func CreateDefaultServerHandler(queuesize uint32) *DefaultServerHandler {
	handler := &DefaultServerHandler{
		connEventsCh: make(chan *ConnEvent),
	}

	return handler
}

func (this *DefaultServerHandler) RunConnectionProcessLoop(conn Connection) {
	go this.RunConnectionWriteLoop(conn)
	this.RunConnectionReadLoop(conn)
}

func (this *DefaultServerHandler) RunConnectionReadLoop(conn Connection) {
	buf := make([]byte, 1024)
	totalread := 0

	for {
		//	First length
		totalread = 0
		length, err := conn.conn.Read(buf)

		if err != nil {
			evt := &ConnEvent{
				Evtid: CONNEVT_DISCONNECT,
			}
			this.GetEventQueue() <- evt
			log.Println("Connection read error,connection cut...Error info[", err, "]")
			break
		}

		if length < CONS_MSGHEAD_LENGTH {
			//	go on reading
			totalread += length
			continue
		}
	}
}

func (this *DefaultServerHandler) RunConnectionWriteLoop(conn Connection) {

}

func (this *DefaultServerHandler) GetEventQueue() chan *ConnEvent {
	return this.connEventsCh
}
