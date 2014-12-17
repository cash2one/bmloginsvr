package client

import (
	"errors"
	"log"
	"net"
	//"time"
)

type Client struct {
	DestIPAddr string
	EvtHandler IClientHandler
}

func CreateClient() *Client {
	return &Client{}
}

func CreateClientWithDefaultHandler() *Client {
	return &Client{EvtHandler: CreateDefaultServerHandler(50)}
}

type IClientHandler interface {
	RunConnectionProcessLoop(conn *Connection)
	RunConnectionReadLoop(conn *Connection)
	RunConnectionWriteLoop(conn *Connection, stpch chan bool)

	createDisconnectEvt(conn *Connection) *ConnEvent
	createConnectEvt(conn *Connection) *ConnEvent
	createReadReadyEvt(conn *Connection, msg []byte) *ConnEvent

	GetEventQueue() chan *ConnEvent
}

func (this *Client) Connect(address string) (conn *Connection, err error) {
	if this.EvtHandler == nil {
		log.Fatalln("Must set the handler to handle the connection event...")
		return nil, errors.New("invalid evtHandler")
	}

	this.DestIPAddr = address

	cc, e := net.Dial("tcp", address)
	if e != nil {
		log.Println("connect to ", address, " failed :", e)
		err = e
		return
	}

	conn = CreateConnection(cc, 50)
	log.Println("connected to ", cc.RemoteAddr().String())

	go this.EvtHandler.RunConnectionProcessLoop(conn)
	return
}
