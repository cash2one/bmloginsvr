package server

import (
	"testing"
)

func TestServer(t *testing.T) {
	evthandler := CreateDefaultServerHandler(50)
	cfg := &ServerConfig{outputEvtSum: 50}
	svr := CreateWithConfig(cfg)
	svr.EvtHandler = evthandler

	lsaddr := "127.0.0.1:8300"
	if !svr.StartListen(lsaddr) {
		t.Error("Can listen on ip port ", lsaddr)
	}
}
