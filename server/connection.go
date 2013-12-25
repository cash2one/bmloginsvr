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
	CONNEVT_UNDEFINE   = 0
	CONNEVT_CONNECT    = 1
	CONNEVT_DISCONNECT = 2
)

type ConnEvent struct {
	Evtid uint32
	Conn  Connection
	Msg   []byte
}
