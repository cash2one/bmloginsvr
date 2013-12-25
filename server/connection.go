package server

import (
	//"log"
	"net"
)

type Connection struct {
	conn net.Conn
	tag  uint32
}
