package websocketserver

import (
	"github.com/gorilla/websocket"
	"gitlab.uc888.cn/mobile/tryutils"
	"net/http"
)

type WebSocketServer struct {
	conf        WebSocketConfig
	evtsChannel chan *tryutils.ConnEvent
	handleUrls  string
}

func NewWebSocketServer(cfg *WebSocketConfig) *WebSocketServer {
	ins := &WebSocketServer{}
	ins.Create(cfg)
	return ins
}

func (this *WebSocketServer) Create(cfg *WebSocketConfig) {
	if nil == cfg {
		this.conf = defaultWebSocketConn
	} else {
		this.conf = *cfg
	}

	this.evtsChannel = make(chan *tryutils.ConnEvent, this.conf.EventQueueBufferSize)
}

//	interface implement
func (this *WebSocketServer) OnEventConnect(conn *WebSocketConn) {
	evt := &tryutils.ConnEvent{}
	evt.Type = tryutils.EVENT_CONNECTED
	evt.UserData = conn
	this.evtsChannel <- evt
}

func (this *WebSocketServer) OnEventDisconnect(conn *WebSocketConn) {
	evt := &tryutils.ConnEvent{}
	evt.Type = tryutils.EVENT_DISCONNECTED
	evt.UserData = conn
	this.evtsChannel <- evt
}

func (this *WebSocketServer) OnEventRead(conn *WebSocketConn, msgType int, msg []byte) {
	evt := &tryutils.ConnEvent{}
	evt.Type = tryutils.EVENT_MESSAGE
	evt.UserData = conn
	evt.Data = &tryutils.MessageBuffer{}
	evt.Data.WriteBuffer(msg)
	evt.Data.Rewind()
	this.evtsChannel <- evt
}

func (this *WebSocketServer) OnEventWrite(conn *WebSocketConn) {

}

//	export
func (this *WebSocketServer) Listen(url string) {
	http.HandleFunc(url, this.webSocketHandler)
	this.handleUrls += url
	this.handleUrls += " "
}

func (this *WebSocketServer) Run(addr string) {
	tryutils.LogInfoln("Web socket server listen on address:", addr, "handle url:", this.handleUrls)
	tryutils.LogInfoln("Websocket server config:", &this.conf)
	go http.ListenAndServe(addr, nil)
}

func (this *WebSocketServer) GetEventQueue() chan *tryutils.ConnEvent {
	return this.evtsChannel
}

//	private
func (this *WebSocketServer) webSocketHandler(w http.ResponseWriter, r *http.Request) {
	var upgrader = websocket.Upgrader{
		ReadBufferSize:  this.conf.ReadBufferSize,
		WriteBufferSize: this.conf.WriteBufferSize,
	}

	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		tryutils.LogErrorln("Failed to upgrade http connection.")
		return
	}

	wsc := NewWebSocketConn()
	wsc.Conn = conn
	wsc.EventReceiver = this

	err = wsc.ReadAndWrite(&this.conf)
	if err != nil {
		tryutils.LogErrorln("Connection read and write failed.error:", err)
	}
}
