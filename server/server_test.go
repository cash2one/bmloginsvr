package server

import (
	"testing"
)

type ConnHandler struct {
	EvtQueue chan *ConnEvent
}

func (this *ConnHandler) RunConnectionProcessLoop(conn Connection) {
	go this.RunConnectionWriteLoop(conn)
	this.RunConnectionReadLoop(conn)
}

func (this *ConnHandler) RunConnectionReadLoop(conn Connection) {

}

func (this *ConnHandler) RunConnectionWriteLoop(conn Connection) {

}

func (this *ConnHandler) GetEventQueue() chan *ConnEvent {
	return this.EvtQueue
}

func TestServer(t *testing.T) {
	evthandler := &ConnHandler{
		EvtQueue: make(chan *ConnEvent, 50),
	}
	cfg := &ServerConfig{outputEvtSum: 50}
	svr := CreateWithConfig(cfg)
	svr.EvtHandler = evthandler

	lsaddr := "127.0.0.1:8300"
	if !svr.StartListen(lsaddr) {
		t.Error("Can listen on ip port ", lsaddr)
	}
}
