package main

import (
	"fmt"
	"log"
	"server"
)

var (
	g_ServerS  *server.Server
	g_UserList *UserInfoList
	g_CtrlCh   chan uint8
)

func main() {
	handler := server.CreateDefaultServerHandler(50)
	g_ServerS = server.CreateWithConfig(nil)
	g_ServerS.EvtHandler = handler

	g_UserList = &UserInfoList{
		allusers: make(map[uint32]*User),
	}

	g_CtrlCh = make(chan uint8, 10)

	ch := make(chan string, 10)
	go go_handleInput(ch)

	server.InitSeed(0)
	if g_ServerS.StartListen("127.0.0.1:8300") {
		log.Println("Start process connection event...")

		for {
			select {
			case evt := <-handler.GetEventQueue():
				{
					ProcessServerSEvent(evt)
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

	close(g_CtrlCh)
}

func ProcessServerSEvent(evt *server.ConnEvent) {
	switch evt.Evtid {
	case server.CONNEVT_CONNECT:
		{
			HandleConnect(evt)
		}
	case server.CONNEVT_READREADY:
		{
			HandleMsg(evt)
		}
	case server.CONNEVT_DISCONNECT:
		{
			HandleDisconnect(evt)
		}
	default:
		{
			log.Println("Unsolved ConnEvent[evtid:", evt.Evtid, "]")
		}
	}
}

func ProcessInput(input string) {
	var cmd, param string = "", ""
	_, err := fmt.Scanf("-%s %s", &cmd, &param)
	if err != nil {
		log.Println("Parse user input error!")
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
		cmd   string
		param string
	)

	for {
		_, err := fmt.Scanf("-%s %s", &cmd, &param)
		if err != nil {
			log.Println("Receive user input failed...Error[", err, "]")
			break
		}
		ch <- cmd
	}

	log.Println("Goroutine [go_handleInput] quit...")
}
