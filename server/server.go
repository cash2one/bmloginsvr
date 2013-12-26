package server

import (
	"log"
	"net"
	//"time"
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

	createDisconnectEvt(conn Connection) *ConnEvent
	createConnectEvt(conn Connection) *ConnEvent
	createReadReadyEvt(conn Connection, msg []byte) *ConnEvent

	GetEventQueue() chan *ConnEvent
}

type ServerConfig struct {
	outputEvtSum uint32
}

func CreateWithConfig(cfg *ServerConfig) *Server {
	svr := &Server{}
	return svr
}

func (this *Server) SetOutputEvtSum(sum uint32) {
	if this.EvtHandler != nil {
	}
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
	go this.go_handleAccept(listener)

	return true
}

func (this *Server) go_handleAccept(listener net.Listener) {
	log.Println("Goroutine [go_handleAccept] start...")

	for {
		aconn, err := listener.Accept()
		if err != nil {
			log.Fatalln("Server listen failed...Server force to quit... Error info[", err, "]")
			break
		}

		//	Process the new connection
		/*newconn := Connection{
			conn: aconn,
			tag:  0,
			wrtch: make(chan []byte, 50)
		}*/
		newconn := CreateConnection(aconn, 50)

		log.Println("New connection [", newconn.conn.RemoteAddr(), "]")
		go this.EvtHandler.RunConnectionProcessLoop(*newconn)
		this.EvtHandler.GetEventQueue() <- this.EvtHandler.createConnectEvt(*newconn)
	}

	log.Println("Goroutine [go_handleAccept quit...]")
}
