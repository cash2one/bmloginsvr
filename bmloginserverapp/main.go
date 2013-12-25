package main

import (
	"log"
	"server"
)

var (
	g_ServerS       *server.Server
	g_ServerSConfig *server.ServerConfig
)

func main() {
	handler := server.CreateDefaultServerHandler(50)
	g_ServerSConfig = &server.ServerConfig{
		outputEvtSum: 50,
	}
	g_ServerS = server.CreateWithConfig(g_ServerSConfig)
	g_ServerS.EvtHandler = handler

	if g_ServerS.StartListen("127.0.0.1:8300") {
		log.Println("Start process connection event...")

		for {
			select {
			case evt := <-handler.GetEventQueue():
				ProcessServerSEvent(evt)
			}
		}
	}
}

func ProcessServerSEvent(evt *server.ConnEvent) {

}
