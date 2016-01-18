package websocketserver

import (
	"errors"
	"github.com/gorilla/websocket"
	"gitlab.uc888.cn/mobile/tryutils"
	"runtime/debug"
	"time"
)

type WebSocketSendMessage struct {
	MsgType int
	MsgData []byte
}

type WebSocketConn struct {
	Conn          *websocket.Conn
	Tag           uint32
	EventReceiver WebSocketEventReceiver
	closed        bool
	connected     bool
	WriteQueue    chan *WebSocketSendMessage
	cfg           *WebSocketConfig
}

func NewWebSocketConn() *WebSocketConn {
	ins := &WebSocketConn{}
	ins.WriteQueue = make(chan *WebSocketSendMessage, 100)
	return ins
}

func (this *WebSocketConn) DispatchConnect() {
	if nil == this.EventReceiver {
		return
	}
	this.EventReceiver.OnEventConnect(this)
}

func (this *WebSocketConn) DispatchDisconnect() {
	if nil == this.EventReceiver {
		return
	}
	this.EventReceiver.OnEventDisconnect(this)
}

func (this *WebSocketConn) DispatchRead(msgType int, msg []byte) {
	if nil == this.EventReceiver {
		return
	}
	this.EventReceiver.OnEventRead(this, msgType, msg)
}

func (this *WebSocketConn) DispatchWrite() {
	if nil == this.EventReceiver {
		return
	}
	this.EventReceiver.OnEventWrite(this)
}

func (this *WebSocketConn) WriteMessage(msgType int, msg []byte) {
	if this.cfg.WriteTimeoutSec == 0 {
		//	直接发送
		this.WriteQueue <- &WebSocketSendMessage{MsgType: msgType, MsgData: msg}
	} else {
		//	需要判断是否超时
		select {
		case <-time.After(time.Second * time.Duration(this.cfg.WriteTimeoutSec)):
			{
				this.Conn.Close()
			}
		case this.WriteQueue <- &WebSocketSendMessage{MsgType: msgType, MsgData: msg}:
			{
				//	nothing
			}
		}
	}
}

func (this *WebSocketConn) Close() {
	if !this.closed &&
		this.connected {
		this.Conn.Close()
		this.closed = true
		this.connected = false
	}
}

func (this *WebSocketConn) ReadAndWrite(cfg *WebSocketConfig) error {
	//	one goroutine write, one goroutine read
	this.DispatchConnect()
	this.connected = true
	this.cfg = cfg

	go this.write()
	err := this.read()

	//	close the connection when return
	this.Conn.Close()
	this.closed = true
	close(this.WriteQueue)
	this.DispatchDisconnect()
	return err
}

/*func (this *WebSocketConn) readPacket(buf []byte) (int, int, error) {
	var messageType int
	var r io.Reader
	var err error

	messageType, r, err = this.Conn.NextReader()
	if err != nil {
		return 0, 0, err
	}

	var readSize int
	readSize, err = io.ReadFull(r, buf)

	if err != nil {
		//	process errors
		if err == io.ErrUnexpectedEOF {
			err = nil
		}
	}

	return messageType, readSize, err
}*/

func (this *WebSocketConn) readPacket() (int, []byte, error) {
	messageType, data, err := this.Conn.ReadMessage()
	if err != nil {
		return messageType, nil, err
	}

	return messageType, data, err
}

func (this *WebSocketConn) read() error {
	defer func() {
		tryutils.LogInfoln("WebSocketConn read quit.")

		//	单独的一个goroutine，打印错误
		if err := recover(); err != nil {
			tryutils.LogErrorln("Panic:", err)
			tryutils.LogErrorln("Stack:", string(debug.Stack()))
		}
	}()

	tryutils.LogInfoln("WebSocketConn read begin.")

	for {
		if 0 != this.cfg.ReadTimeoutSec {
			this.Conn.SetReadDeadline(time.Now().Add(time.Second * time.Duration(this.cfg.ReadTimeoutSec)))
		}
		messageType, buf, err := this.readPacket()
		if err != nil {
			return err
		}
		this.Conn.SetReadDeadline(time.Time{})

		this.DispatchRead(messageType, buf)
	}

	return nil
}

//	Must close the connection when error occurs
func (this *WebSocketConn) write() error {
	defer func() {
		tryutils.LogInfoln("WebSocketConn write quit.")

		//	单独的一个goroutine，打印错误
		if err := recover(); err != nil {
			tryutils.LogErrorln("Panic:", err)
			tryutils.LogErrorln("Stack:", string(debug.Stack()))
		}
	}()

	tryutils.LogInfoln("WebSocketConn write begin.")
	for {
		select {
		case data, ok := <-this.WriteQueue:
			{
				if !ok {
					this.Close()
					return errors.New("Get data from write channel failed.")
				}

				w, err := this.Conn.NextWriter(data.MsgType)
				if err != nil {
					this.Close()
					return err
				}
				_, err = w.Write(data.MsgData)
				if err != nil {
					this.Close()
					return err
				}
				if err = w.Close(); err != nil {
					this.Close()
					return err
				}

				this.DispatchWrite()
			}
		}
	}

	return nil
}

func (this *WebSocketConn) WriteBinary(buf *tryutils.MessageBuffer) {
	this.WriteMessage(websocket.BinaryMessage, buf.GetData())
}

func (this *WebSocketConn) WriteText(text string) {
	this.WriteMessage(websocket.TextMessage, []byte(text))
}

func (this *WebSocketConn) WriteWithHead(head tryutils.IMessageHeadWriter, msgData ...interface{}) bool {
	buf := &tryutils.MessageBuffer{}

	head.WriteHeadToBuffer(buf)

	for i := range msgData {
		if !buf.Write(msgData[i]) {
			return false
		}
	}
	if !head.UpdateHeadContentLength(buf) {
		return false
	}

	this.WriteBinary(buf)
	return true
}
