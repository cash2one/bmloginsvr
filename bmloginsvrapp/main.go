package main

import (
	"fmt"
	"log"
	"server"
)

var (
	g_ServerS    *server.Server
	g_ServerC    *server.Server
	g_UserList   *UserInfoList
	g_ServerList *UserInfoList
	g_CtrlCh     chan uint8
)

func main() {
	//	for server
	handler := server.CreateDefaultServerHandler(50)
	g_ServerS = server.CreateWithConfig(nil)
	g_ServerS.EvtHandler = handler
	g_ServerList = &UserInfoList{
		allusers: make(map[uint32]IUserInterface),
	}

	//	for client
	chandler := server.CreateDefaultServerHandler(50)
	g_ServerC = server.CreateWithConfig(nil)
	g_ServerC.EvtHandler = chandler
	g_UserList = &UserInfoList{
		allusers: make(map[uint32]IUserInterface),
	}

	g_CtrlCh = make(chan uint8, 10)

	ch := make(chan string, 10)
	go go_handleInput(ch)

	server.InitSeed(0)
	ipaddrclient := "127.0.0.1:8300"
	ipaddrserver := "127.0.0.1:8200"
	if g_ServerS.StartListen(ipaddrserver) && g_ServerC.StartListen(ipaddrclient) {
		log.Println("Start process event.listen server:", ipaddrserver, " listen client:", ipaddrclient)

		for {
			select {
			case evt := <-handler.GetEventQueue():
				{
					ProcessServerSEvent(evt)
				}
			case evt := <-chandler.GetEventQueue():
				{
					ProcessServerCEvent(evt)
				}
			case input := <-ch:
				{
					ProcessInput(input)
				}
			case ctrl := <-g_CtrlCh:
				{
					if ctrl == 0 {
						break
					}
				}
			}
		}
	}

	log.Println("Quit process event...")
	close(g_CtrlCh)
}

func ProcessServerCEvent(evt *server.ConnEvent) {
	switch evt.Evtid {
	case server.CONNEVT_CONNECT:
		{
			HandleCConnect(evt)
		}
	case server.CONNEVT_READREADY:
		{
			HandleCMsg(evt)
		}
	case server.CONNEVT_DISCONNECT:
		{
			HandleCDisconnect(evt)
		}
	default:
		{
			log.Println("Unsolved ConnEvent[evtid:", evt.Evtid, "]")
		}
	}
}

func ProcessServerSEvent(evt *server.ConnEvent) {
	switch evt.Evtid {
	case server.CONNEVT_CONNECT:
		{
			HandleSConnect(evt)
		}
	case server.CONNEVT_READREADY:
		{
			HandleSMsg(evt)
		}
	case server.CONNEVT_DISCONNECT:
		{
			HandleSDisconnect(evt)
		}
	default:
		{
			log.Println("Unsolved ConnEvent[evtid:", evt.Evtid, "]")
		}
	}
}

func ProcessInput(input string) {
	log.Println("rec ", input)
	var cmd, param string = "", ""
	_, err := fmt.Sscanf(input, "%s_%s", &cmd, &param)
	if err != nil {
		log.Println("Parse user input error!Error[", err, "]")
		return
	}
	switch cmd {
	case "quit":
		{
			g_CtrlCh <- uint8(0)
		}
	}
	log.Println("Command: ", cmd, " parameters: ", param)
}

func go_handleInput(ch chan string) {
	log.Println("Goroutine [go_handleInput] start...")

	var (
		cmd string
	)

	for {
		_, err := fmt.Scanln(&cmd)
		if err != nil {
			log.Println("Receive user input failed...Error[", err, "]")
			break
		}
		ch <- cmd
	}

	log.Println("Goroutine [go_handleInput] quit...")
}
