package server

import (
	//"log"
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

}

func (this *DefaultServerHandler) RunConnectionReadLoop(conn Connection) {

}

func (this *DefaultServerHandler) RunConnectionWriteLoop(conn Connection) {

}

func (this *DefaultServerHandler) GetEventQueue() chan *ConnEvent {
	return this.connEventsCh
}
