package server

import (
	"bytes"
	"encoding/binary"
	"log"
	"net"
)

type Connection struct {
	conn  net.Conn
	tag   uint32
	wrtch chan []byte
}

func WriteMsgLittleEndian(conn Connection, opcode uint32, body []byte) uint32 {
	buf := new(bytes.Buffer)
	length := 4 + 4 + 1
	if body != nil {
		length -= 1
		length += len(body)
	}
	binary.Write(buf, binary.BigEndian, length)
	binary.Write(buf, binary.LittleEndian, opcode)
	binary.Write(buf, binary.LittleEndian, body)
	data := buf.Bytes()
	return conn.WriteMsg(data)
}

func CreateConnection(nconn net.Conn, outputqueuesize uint32) *Connection {
	newconn := &Connection{
		conn:  nconn,
		wrtch: make(chan []byte, outputqueuesize),
		tag:   0,
	}
	return newconn
}

func (this *Connection) GetInternalConn() net.Conn {
	return this.conn
}

func (this *Connection) GetConnTag() uint32 {
	return this.tag
}

func (this *Connection) SetConnTag(tag uint32) {
	this.tag = tag
}

func (this *Connection) WriteMsg(msg []byte) uint32 {
	this.wrtch <- msg
	return 0
}

const (
	CONNEVT_UNDEFINE   = 0x00
	CONNEVT_CONNECT    = 0x01
	CONNEVT_DISCONNECT = 0x02
	CONNEVT_READREADY  = 0x03
	CONNEVT_CUSTOM     = 0x10
)

const (
	CONS_MSGHEAD_LENGTH        = 4
	CONS_MSGHEADBODY_MAXLENGTH = 16 * 1024
)

type ConnEvent struct {
	Evtid uint32
	Conn  Connection
	Msg   []byte
}

type DefaultServerHandler struct {
	connEventsCh chan *ConnEvent
}

func CreateDefaultServerHandler(queuesize uint32) *DefaultServerHandler {
	handler := &DefaultServerHandler{
		connEventsCh: make(chan *ConnEvent),
	}

	return handler
}

func (this *DefaultServerHandler) checkError(err error, info string) bool {
	if err != nil {
		log.Println(info, " Error[", err, "]")
		return false
	}
	return true
}

func (this *DefaultServerHandler) createDisconnectEvt(conn Connection) *ConnEvent {
	evt := &ConnEvent{
		Evtid: CONNEVT_DISCONNECT,
		Conn:  conn,
		Msg:   nil,
	}
	return evt
}

func (this *DefaultServerHandler) createConnectEvt(conn Connection) *ConnEvent {
	evt := &ConnEvent{
		Evtid: CONNEVT_CONNECT,
		Conn:  conn,
		Msg:   nil,
	}
	return evt
}

func (this *DefaultServerHandler) createReadReadyEvt(conn Connection, msg []byte) *ConnEvent {
	evt := &ConnEvent{
		Evtid: CONNEVT_READREADY,
		Conn:  conn,
		Msg:   nil,
	}
	evt.Msg = make([]byte, len(msg))
	copy(evt.Msg, msg)
	return evt
}

func (this *DefaultServerHandler) RunConnectionProcessLoop(conn Connection) {
	defer func() {
		err := recover()
		if err != nil {
			log.Println("Error occurs,Error[", err, "]")
		}
		conn.conn.Close()
		this.createDisconnectEvt(conn)
	}()

	go this.RunConnectionWriteLoop(conn)
	this.RunConnectionReadLoop(conn)
}

func (this *DefaultServerHandler) RunConnectionReadLoop(conn Connection) {
	buf := make([]byte, CONS_MSGHEADBODY_MAXLENGTH)

	defer log.Println("Connection readloop of[", conn.GetConnTag(), "] quit...")

	for {
		//	First length
		length, err := conn.conn.Read(buf[0:CONS_MSGHEAD_LENGTH])

		if err != nil {
			this.checkError(err, "Connect read failed...")
			this.connEventsCh <- this.createDisconnectEvt(conn)
			return
		}

		//	check head
		for length < CONS_MSGHEAD_LENGTH {
			//	Keep reading
			recl, err := conn.conn.Read(buf[length:CONS_MSGHEAD_LENGTH])
			length += recl
			if err != nil {
				this.checkError(err, "Connect read failed...")
				this.connEventsCh <- this.createDisconnectEvt(conn)
				return
			}
		}

		//	read content, head totallength and opcode
		var msglength uint32 = 0
		err = binary.Read(bytes.NewBuffer(buf[0:CONS_MSGHEAD_LENGTH]), binary.BigEndian, &msglength)
		if err != nil {
			this.checkError(err, "Get message length failed...")
			this.connEventsCh <- this.createDisconnectEvt(conn)
			return
		}
		if msglength >= CONS_MSGHEADBODY_MAXLENGTH {
			log.Println("invalid msg,msg too long...")
			break
		}
		log.Println("head read ,[head length: ", length, " msg length", msglength)

		//	Get the full message
		leftlength := int(msglength - CONS_MSGHEAD_LENGTH)
		for leftlength > 0 {
			recl, err := conn.conn.Read(buf[CONS_MSGHEAD_LENGTH:msglength])
			if err != nil {
				this.checkError(err, "Read message body failed...")
				this.connEventsCh <- this.createDisconnectEvt(conn)
				return
			}
			leftlength -= recl
			log.Printf("rec %d left %d", recl, leftlength)
		}
		log.Println("get the full msg")

		//	OK
		if msglength < 8 {
			log.Println("Discard a msg because the length of the msg is less than 8")
			continue
		}
		this.connEventsCh <- this.createReadReadyEvt(conn, buf[0:msglength])
	}
}

func (this *DefaultServerHandler) RunConnectionWriteLoop(conn Connection) {
	defer func() {
		err := recover()
		if err != nil {
			log.Println("Exception.Error occurs,Error[", err, "]")
		}
		conn.conn.Close()
		this.createDisconnectEvt(conn)
	}()

	for {
		select {
		case msg := <-conn.wrtch:
			{
				_, err := conn.conn.Write(msg)
				if err != nil {
					this.checkError(err, "Write msg error...")
					break
				}
			}
		}
	}

	log.Println("Connection writeloop of[", conn.GetConnTag(), "] quit...")
}

func (this *DefaultServerHandler) GetEventQueue() chan *ConnEvent {
	return this.connEventsCh
}
