package server

import (
	"bytes"
	"encoding/binary"
	"errors"
	"net"
	"shareutils"
	"time"
)

type Connection struct {
	conn  net.Conn
	tag   uint32
	wrtch chan []byte
}

func WriteMsgLittleEndian(conn *Connection, opcode uint32, body []byte) uint32 {
	buf := new(bytes.Buffer)
	var length uint32 = 4 + 4
	if body != nil {
		length += uint32(len(body))
	}
	binary.Write(buf, binary.BigEndian, &length)
	binary.Write(buf, binary.LittleEndian, &opcode)
	if body != nil {
		binary.Write(buf, binary.LittleEndian, body)
	}
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
	Conn  *Connection
	Msg   []byte
}

type DefaultServerHandler struct {
	connEventsCh chan *ConnEvent
	timeoutSec   uint64
}

func CreateDefaultServerHandler(queuesize uint32) *DefaultServerHandler {
	handler := &DefaultServerHandler{
		connEventsCh: make(chan *ConnEvent),
	}

	return handler
}

func (this *DefaultServerHandler) SetTimeoutSec(sec uint64) {
	this.timeoutSec = sec
}

func (this *DefaultServerHandler) checkError(err error, info string) bool {
	if err != nil {
		shareutils.LogErrorln(info, " Error[", err, "]")
		return false
	}
	return true
}

func (this *DefaultServerHandler) createDisconnectEvt(conn *Connection) *ConnEvent {
	evt := &ConnEvent{
		Evtid: CONNEVT_DISCONNECT,
		Conn:  conn,
		Msg:   nil,
	}
	return evt
}

func (this *DefaultServerHandler) createConnectEvt(conn *Connection) *ConnEvent {
	evt := &ConnEvent{
		Evtid: CONNEVT_CONNECT,
		Conn:  conn,
		Msg:   nil,
	}
	return evt
}

func (this *DefaultServerHandler) createReadReadyEvt(conn *Connection, msg []byte) *ConnEvent {
	evt := &ConnEvent{
		Evtid: CONNEVT_READREADY,
		Conn:  conn,
		Msg:   nil,
	}
	evt.Msg = make([]byte, len(msg))
	copy(evt.Msg, msg)
	return evt
}

func (this *DefaultServerHandler) RunConnectionProcessLoop(conn *Connection) {
	var stpch chan bool

	defer func() {
		err := recover()
		if err != nil {
			shareutils.LogErrorln("Error occurs,Error[", err, "]")
		}
		if stpch != nil {
			close(stpch)
		}
		conn.conn.Close()
		disConnEvt := this.createDisconnectEvt(conn)
		this.connEventsCh <- disConnEvt
		shareutils.LogInfoln("Goroutine [RunConnectionProcessLoop] on ", conn.GetConnTag(), " quit...")
	}()

	shareutils.LogInfoln("Goroutine [RunConnectionProcessLoop] on", conn.GetConnTag(), "is running.Timeout sec:", this.timeoutSec)

	stpch = make(chan bool)
	go this.RunConnectionWriteLoop(conn, stpch)
	this.RunConnectionReadLoop(conn)

	//	note:不需要为stpch写入数据 当channel close掉后 所有监听该channel处都会立即返回
}

func (this *DefaultServerHandler) ReadPacketFromConn(buf []byte, conn *Connection) (uint32, error) {
	//	First length
	length, err := conn.conn.Read(buf[0:CONS_MSGHEAD_LENGTH])

	if err != nil {
		this.checkError(err, "Connect read failed...")
		return 0, err
	}

	//	check head
	for length < CONS_MSGHEAD_LENGTH {
		//	Keep reading
		recl, err := conn.conn.Read(buf[length:CONS_MSGHEAD_LENGTH])
		length += recl
		if err != nil {
			this.checkError(err, "Connect read failed...")
			return 0, err
		}
	}

	//	read content, head totallength and opcode
	var msglength uint32 = 0
	err = binary.Read(bytes.NewBuffer(buf[0:CONS_MSGHEAD_LENGTH]), binary.BigEndian, &msglength)
	if err != nil {
		this.checkError(err, "Get message length failed...")
		return 0, err
	}
	if msglength >= CONS_MSGHEADBODY_MAXLENGTH {
		shareutils.LogErrorln("Invalid msg,msg too long...")
		return 0, errors.New("Msg length is too long")
	}

	//	Get the full message
	leftlength := int(msglength - CONS_MSGHEAD_LENGTH)
	for leftlength > 0 {
		recl, err := conn.conn.Read(buf[CONS_MSGHEAD_LENGTH:msglength])
		if err != nil {
			this.checkError(err, "Read message body failed...")
			return 0, err
		}
		leftlength -= recl
	}

	//	OK
	if msglength < 8 {
		shareutils.LogErrorln("Discard a msg because the length of the msg is less than 8")
		return 0, errors.New("Invalid msg head length")
	}

	return msglength, nil
}

func (this *DefaultServerHandler) RunConnectionReadLoop(conn *Connection) {
	buf := make([]byte, CONS_MSGHEADBODY_MAXLENGTH)

	for {
		/*//	First length
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
			//log.Printf("rec %d left %d", recl, leftlength)
		}

		//	OK
		if msglength < 8 {
			log.Println("Discard a msg because the length of the msg is less than 8")
			continue
		}*/
		if 0 != this.timeoutSec {
			conn.conn.SetReadDeadline(time.Now().Add(time.Duration(this.timeoutSec) * time.Second))
		}

		msglength, err := this.ReadPacketFromConn(buf, conn)

		if 0 != this.timeoutSec {
			conn.conn.SetReadDeadline(time.Time{})
		}

		if err != nil {
			shareutils.LogErrorln("Read data from connection failed.Error:", err)
			return
		}

		//	dispatch the packet
		this.connEventsCh <- this.createReadReadyEvt(conn, buf[0:msglength])
	}
}

func (this *DefaultServerHandler) RunConnectionWriteLoop(conn *Connection, stpch chan bool) {
	defer func() {
		err := recover()
		if err != nil {
			shareutils.LogErrorln("Exception.Error occurs,Error[", err, "]")
		}
		shareutils.LogInfoln("Connection writeloop of[", conn.GetConnTag(), "] quit...")
	}()

	shareutils.LogInfoln("Goroutine [RunConnectionWriteLoop] begin...")
	for {
		select {
		case msg := <-conn.wrtch:
			{
				_, err := conn.conn.Write(msg)
				if err != nil {
					this.checkError(err, "Write msg error...")
					return
				}
			}
		case <-stpch:
			{
				shareutils.LogInfoln("Receive read loop stop signal")
				return
			}
		}
	}
}

func (this *DefaultServerHandler) GetEventQueue() chan *ConnEvent {
	return this.connEventsCh
}
