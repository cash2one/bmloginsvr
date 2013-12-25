package server

import (
//"log"
)

type Server struct {
	sconfig ServerConfig
}

type IServerHandler interface {
	RunConnectionProcessLoop(conn Connection)
	RunConnectionReadLoop(conn Connection)
	RunConnectionWriteLoop(conn Connection)
}

type ServerConfig struct {
	outputEvtSum uint32
}

func CreateWithConfig(cfg *ServerConfig) *Server {
	svr := &Server{sconfig: *cfg}
	return svr
}

func (this *Server) StartListen(listenaddr string) bool {
	return true
}
