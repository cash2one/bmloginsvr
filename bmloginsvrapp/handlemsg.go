package main

import (
	"log"
	"server"
	"time"
)

type User struct {
	ipaddr   string
	uid      uint32
	conn     server.Connection
	verified bool
	conntime time.Time
}

func CreateUser(clientconn server.Connection) *User {
	user := &User{
		ipaddr:   clientconn.GetInternalConn().RemoteAddr().String(),
		conn:     clientconn,
		verified: false,
		conntime: time.Now(),
	}
	user.conn.SetConnTag(server.GetSeed())
	return user
}

func (this *User) OnUserMsg(msg []byte) {

}

type UserInfoList struct {
	allusers map[uint32]*User
}

func (this *UserInfoList) AddUser(user *User) bool {
	inlistuser := this.allusers[user.conn.GetConnTag()]
	if nil == inlistuser {
		this.allusers[user.conn.GetConnTag()] = user
		return true
	}
	return false
}

func (this *UserInfoList) GetUser(conntag uint32) *User {
	user := this.allusers[conntag]
	return user
}

func (this *UserInfoList) RemoveUser(conntag uint32) {
	delete(this.allusers, conntag)
}

////////////////////////////////////////////////////////
func HandleConnect(msg *server.ConnEvent) {
	newuser := CreateUser(msg.Conn)
	g_UserList.AddUser(newuser)
}

func HandleDisconnect(msg *server.ConnEvent) {
	g_UserList.RemoveUser(msg.Conn.GetConnTag())
}

func HandleMsg(msg *server.ConnEvent) {
	var length, opcode uint32 = 0, 0
	var headreader server.IMsgReader = &server.DefaultMsgReader{}
	headreader.SetDataSource(msg.Msg)
	length = headreader.ReadMsgLength()
	opcode = headreader.ReadMsgOpCode()
	log.Println("Receive msg[length:", length, " opcode:", opcode, "]")

	defer func() {
		err := recover()
		if err != nil {
			log.Println("A exception occured while processing msg[length:", length, " opcode:", opcode, "]")
		}
	}()

	user := g_UserList.GetUser(msg.Conn.GetConnTag())
	if user != nil {
		if !user.verified {
			//	Only handle the verify message
		} else {
			user.OnUserMsg(msg.Msg)
		}
	}
}
