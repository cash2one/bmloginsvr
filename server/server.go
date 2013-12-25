package server

import (
	"log"
	"net"
	"time"
)

type Server struct {
	SConfig    ServerConfig
	ListenAddr string
	EvtHandler IServerHandler
}

type IServerHandler interface {
	RunConnectionProcessLoop(conn Connection)
	RunConnectionReadLoop(conn Connection)
	RunConnectionWriteLoop(conn Connection)

	GetEventQueue() chan *ConnEvent
}

type ServerConfig struct {
	outputEvtSum uint32
}

func CreateWithConfig(cfg *ServerConfig) *Server {
	svr := &Server{SConfig: *cfg}
	return svr
}

func (this *Server) StartListen(listenaddr string) bool {
	if this.EvtHandler == nil {
		log.Fatalln("Must set the handler to handle the connection event...")
		return false
	}

	listener, err := net.Listen("tcp", listenaddr)
	if err != nil {
		log.Fatalln("Can listen on ", listenaddr, " ,following are error info[", err, "]")
		return false
	}

	log.Println("Start listen on ", listenaddr)
	this.ListenAddr = listenaddr

	//	Create a goroutine to handle the accept event
	ctrlSignal := make(chan int, 10)
	go this.go_handleAccept(listener, ctrlSignal)

	return true
}

func (this *Server) go_handleAccept(listener net.Listener, ctrlSignal <-chan int) {
	log.Println("Goroutine [go_handleAccept] begin...")

	for {
		select {
		case variable := <-ctrlSignal:
			{
				if variable == 1 {
					break
				}
			}
		case <-time.After(10 * time.Millisecond):
			{
				break
			}
		}
	}

	log.Println("Goroutine [go_handleAccept quit...]")
}
