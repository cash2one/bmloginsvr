package main

import (
	"bytes"
	"encoding/binary"
	"log"
	"server"
	"time"
)

var (
	loginopstart uint32 = 1000
	//	+0	force quit
	//	+1	server verify
	//	+2	client verify
	//	+3	verify result
)

type IUserInterface interface {
	OnConnect()
	OnDisconnect()
	OnUserMsg(msg []byte)
	SendUserMsg(opcode uint32, args ...interface{}) bool
	GetUserTag() uint32
	IsVerified() bool
}

type User struct {
	ipaddr   string
	uid      uint32
	conn     *server.Connection
	verified bool
	conntime time.Time
}

func CreateUser(clientconn *server.Connection) *User {
	user := &User{
		ipaddr:   clientconn.GetInternalConn().RemoteAddr().String(),
		conn:     clientconn,
		verified: false,
		conntime: time.Now(),
	}
	user.conn.SetConnTag(server.GetSeed())
	return user
}

func (this *User) OnConnect() {
	log.Println("Peer ", this.ipaddr, " connected...")
}

func (this *User) OnDisconnect() {
	log.Println("Peer ", this.ipaddr, " disconnected...")
	this.SendUserMsg(loginopstart, nil)
}

func (this *User) OnUserMsg(msg []byte) {
	var headreader server.IMsgReader = &server.DefaultMsgReader{}
	headreader.SetDataSource(msg)
	opcode := headreader.ReadMsgOpCode()

	switch opcode {
	case loginopstart:
		{
			//	Read package
			//	type 1byte;id 4bytes
			var peertype uint8 = 0
			var peerid uint32 = 0
			err := binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &peertype)
			if err != nil {
				logErr(err, "")
				return
			}
			err = binary.Read(bytes.NewBuffer(msg[9:9+4]), binary.LittleEndian, &peerid)
			if err != nil {
				logErr(err, "")
				return
			}
			//	process
			if !this.verified {
				if peerid == 1 {
					//	client,read database???
				} else if peerid == 2 {
					//	server
				}
			}
		}
	}
}

func (this *User) GetUserTag() uint32 {
	return this.conn.GetConnTag()
}

func (this *User) IsVerified() bool {
	return this.verified
}

func logSendMsgFieldErr(opcode uint32, reqlen uint32) {
	log.Println("Msg[", opcode, "] require ", reqlen, " fields")
}

func logSendMsgTypeErr(opcode uint32, destype string, reqtype string) {
	log.Println("Msg[", opcode, "] require type[", reqtype, "] rec type[", destype, "]")
}

func logErr(err error, info string) {
	log.Println("Error occurs, Error[", err, "]")
}

func (this *User) SendUserMsg(opcode uint32, args ...interface{}) bool {
	if opcode < loginopstart {
		return false
	}

	switch opcode {
	case loginopstart + 0:
		{
			server.WriteMsgLittleEndian(this.conn, opcode, nil)
		}
	case loginopstart + 3:
		{
			//	peer regist success,1bytes
			//	Get values
			//	bool 1byte
			var suc byte = 0
			for i, v := range args {
				if i == 0 {
					switch argtype := v.(type) {
					case *byte:
						{
							suc = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*byte")
							return false
						}
					}
				} else {
					logSendMsgFieldErr(opcode, 1)
					return false
				}
			}
			buf := new(bytes.Buffer)
			binary.Write(buf, binary.LittleEndian, &suc)
			server.WriteMsgLittleEndian(this.conn, opcode, buf.Bytes())
		}
	}
	return true
}

func (this *User) VerifyUser(account, password string) {

}

type ServerUser struct {
	User
	//	for server
	serverid uint16
}

func CreateServerUser(clientconn *server.Connection) *ServerUser {
	user := &ServerUser{
		serverid: 0,
	}
	user.User = User{
		ipaddr:   clientconn.GetInternalConn().RemoteAddr().String(),
		conn:     clientconn,
		verified: false,
		conntime: time.Now(),
	}
	user.conn.SetConnTag(server.GetSeed())
	return user
}

func (this *ServerUser) OnUserMsg(msg []byte) {
	var headreader server.IMsgReader = &server.DefaultMsgReader{}
	headreader.SetDataSource(msg)
	opcode := headreader.ReadMsgOpCode()

	switch opcode {
	case loginopstart:
		{
			//	Read package
			//	type 1byte;id 4bytes
			var peertype uint8 = 0
			var peerid uint32 = 0
			err := binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &peertype)
			if err != nil {
				logErr(err, "")
				return
			}
			err = binary.Read(bytes.NewBuffer(msg[9:9+4]), binary.LittleEndian, &peerid)
			if err != nil {
				logErr(err, "")
				return
			}
			//	process
			if !this.verified {
				if peerid == 1 {
					//	client,read database???
				} else if peerid == 2 {
					//	server
				}
			}
		}
	}
}

type UserInfoList struct {
	allusers map[uint32]IUserInterface
}

func (this *UserInfoList) AddUser(user IUserInterface) bool {
	inlistuser := this.allusers[user.GetUserTag()]
	if nil == inlistuser {
		this.allusers[user.GetUserTag()] = user
		return true
	}
	return false
}

func (this *UserInfoList) GetUser(conntag uint32) IUserInterface {
	user := this.allusers[conntag]
	return user
}

func (this *UserInfoList) RemoveUser(conntag uint32) {
	delete(this.allusers, conntag)
}

////////////////////////////////////////////////////////
func HandleCConnect(msg *server.ConnEvent) {
	newuser := CreateUser(msg.Conn)
	g_UserList.AddUser(newuser)
	newuser.OnConnect()
}

func HandleCDisconnect(msg *server.ConnEvent) {
	user := g_UserList.GetUser(msg.Conn.GetConnTag())
	if user != nil {
		user.OnDisconnect()
		g_UserList.RemoveUser(msg.Conn.GetConnTag())
	}
}

func HandleCMsg(msg *server.ConnEvent) {
	var length, opcode uint32 = 0, 0
	var headreader server.IMsgReader = &server.DefaultMsgReader{}
	headreader.SetDataSource(msg.Msg)
	length = headreader.ReadMsgLength()
	opcode = headreader.ReadMsgOpCode()
	log.Println("Receive client[", msg.Conn.GetConnTag(), "] msg[length:", length, " opcode:", opcode, "]")

	defer func() {
		err := recover()
		if err != nil {
			log.Println("A exception occured while processing msg[length:", length, " opcode:", opcode, "]")
		}
	}()

	data := msg.Msg
	user := g_UserList.GetUser(msg.Conn.GetConnTag())
	if user != nil {
		if !user.IsVerified() {
			//	Only handle the verify message
			cltuser, ok := user.(*User)
			if ok {
				if opcode == loginopstart+2 {
					//	user name and password
					var namelen uint32 = 0
					binary.Read(bytes.NewBuffer(data[8:8+4]), binary.LittleEndian, &namelen)
					namebuf := make([]byte, namelen)
					binary.Read(bytes.NewBuffer(data[12:12+namelen]), binary.BigEndian, namebuf)
					var namestr string = string(namebuf)
					var pswlen uint32 = 0
					binary.Read(bytes.NewBuffer(data[12+namelen:12+namelen+4]), binary.LittleEndian, &pswlen)
					namebuf = make([]byte, pswlen)
					binary.Read(bytes.NewBuffer(data[12+namelen+4:12+namelen+4+pswlen]), binary.BigEndian, namebuf)
					var pswstr string = string(namebuf)

					cltuser.VerifyUser(namestr, pswstr)
				}
			}
		} else {
			user.OnUserMsg(msg.Msg)
		}
	}
}

func HandleSConnect(msg *server.ConnEvent) {
	newserver := CreateServerUser(msg.Conn)
	g_ServerList.AddUser(newserver)
	newserver.OnConnect()
}

func HandleSDisconnect(msg *server.ConnEvent) {
	user := g_ServerList.GetUser(msg.Conn.GetConnTag())
	if user != nil {
		user.OnDisconnect()
		g_ServerList.RemoveUser(msg.Conn.GetConnTag())
	}
}

func HandleSMsg(msg *server.ConnEvent) {
	var length, opcode uint32 = 0, 0
	var headreader server.IMsgReader = &server.DefaultMsgReader{}
	headreader.SetDataSource(msg.Msg)
	length = headreader.ReadMsgLength()
	opcode = headreader.ReadMsgOpCode()

	defer func() {
		err := recover()
		if err != nil {
			log.Println("A exception occured while processing msg[length:", length, " opcode:", opcode, "]")
		}
	}()

	data := msg.Msg
	user := g_ServerList.GetUser(msg.Conn.GetConnTag())
	if user != nil {
		//	Convert to ServerUser
		svruser, ok := user.(*ServerUser)
		if ok {
			if !user.IsVerified() {
				//	Only handle the verify message
				if opcode == loginopstart+1 {
					//	verify msg,serverid 4bytes;verifycode 4bytes
					if length == 8+2+4 {
						var serverid uint16 = 0
						var verifycode uint32 = 0
						binary.Read(bytes.NewBuffer(data[8:8+2]), binary.LittleEndian, &serverid)
						binary.Read(bytes.NewBuffer(data[10:10+4]), binary.LittleEndian, &verifycode)

						verifyok := false
						verifyok = true
						if verifyok {
							svruser.serverid = serverid
							svruser.verified = true

							var vok uint8 = 1
							user.SendUserMsg(loginopstart+3, &vok)
							log.Println("Server[", serverid, "] registed...")
						}
					}
				}
			} else {
				log.Println("Receive server[", svruser.serverid, "] msg[length:", length, " opcode:", opcode, "]")
				user.OnUserMsg(msg.Msg)
			}
		} else {
			log.Println("Can't convert user to type ServerUser")
		}
	} else {
		log.Println("Can't find the server tag[", msg.Conn.GetConnTag(), "]")
	}
}
