package main

//#include <stdlib.h>
import "C"

import (
	"bytes"
	"encoding/binary"
	"log"
	"os"
	"server"
	"strconv"
	"time"
	"unsafe"
)

var (
	loginopstart uint32 = 10000
	//	+0	force quit
	//	+1	server verify
	//	+2	client verify
	//	+3	verify result
	//	+4	client request to add game role
	//	+5	add role result
	//	+6	client request to delete game role
	//	+7	delete role result
	//	+8	client request to login gamesvr
	//	+9	login gamesvr result
	//	+10 client request to save
	//	+11 send gamerole data to gameserver
	//	+12 send quick message
)

/*
//	quick message
	0:none
	1:没有可用的游戏服务器
	2:任务存档不存在
*/

type IUserInterface interface {
	OnConnect()
	OnDisconnect()
	OnUserMsg(msg []byte)
	SendUserMsg(opcode uint32, args ...interface{}) bool
	GetUserTag() uint32
	IsVerified() bool
	OnVerified()
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

func (this *User) OnVerified() {
	//	Get the uid,so we create the directory and file
	/*if !PathExist("./netusers") {
		err := os.Mkdir("./netusers", os.ModeDir)
		if err != nil {
			log.Println("Can't create parent directory.Error:", err)
			return
		}
	}*/
	//	Create sub directory
	userpath := "./login/" + strconv.FormatUint(uint64(this.uid), 10)
	if !PathExist(userpath) {
		err := os.Mkdir(userpath, os.ModeDir)
		if err != nil {
			log.Println("Cant't create user directory.Error:", err)
			return
		}
	}
	//	Create save file
	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
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
			var suc uint8 = 0
			for i, v := range args {
				if i == 0 {
					switch argtype := v.(type) {
					case *uint8:
						{
							suc = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*uint8")
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
	case loginopstart + 5:
		{
			//	add role success
			//	ret 1byte
			var suc uint8 = 0
			for i, v := range args {
				if i == 0 {
					switch argtype := v.(type) {
					case *uint8:
						{
							suc = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*uint8")
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
	case loginopstart + 7:
		{
			//	delete role success
			//	ret 1byte
			var suc uint8 = 0
			for i, v := range args {
				if i == 0 {
					switch argtype := v.(type) {
					case *uint8:
						{
							suc = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*uint8")
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
	case loginopstart + 9:
		{
			//	lgoin gamesvr result
			//	ret 1byte;client index uint32;addrlen 1byte;addr addrlen
			var suc uint8 = 0
			var cindex uint32 = 0
			var addrlen uint8 = 0
			var addr *string

			for i, v := range args {
				if i == 0 {
					switch argtype := v.(type) {
					case *uint8:
						{
							suc = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*uint8")
							return false
						}
					}
				} else if i == 1 {
					switch argtype := v.(type) {
					case *uint32:
						{
							cindex = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*uint32")
							return false
						}
					}
				} else if i == 2 {
					switch argtype := v.(type) {
					case *uint8:
						{
							addrlen = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*string")
							return false
						}
					}
				} else if i == 3 {
					switch argtype := v.(type) {
					case *string:
						{
							addr = argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*uint8")
							return false
						}
					}
				} else {
					logSendMsgFieldErr(opcode, 1)
					return false
				}
			}

			//	write buffer
			buf := new(bytes.Buffer)
			binary.Write(buf, binary.LittleEndian, &suc)
			binary.Write(buf, binary.LittleEndian, &cindex)
			binary.Write(buf, binary.LittleEndian, &addrlen)
			binary.Write(buf, binary.LittleEndian, addr)
			server.WriteMsgLittleEndian(this.conn, opcode, buf.Bytes())
		}
	}
	return true
}

func (this *User) OnUserMsg(msg []byte) {
	var headreader server.IMsgReader = &server.DefaultMsgReader{}
	headreader.SetDataSource(msg)
	opcode := headreader.ReadMsgOpCode()
	packetlen := len(msg)

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
	case loginopstart + 4:
		{
			//	player request to add game role
			//	namelen 1byte;name namelen;job 1byte;sex 1byte
			var namelen uint8 = 0
			binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
			reqlength := int(8 + 1 + namelen + 2)
			if packetlen == reqlength {
				this.OnRequestAddGameRole(msg)
			}
		}
	case loginopstart + 6:
		{
			//	player request to delete a game role
			//	namelen 1byte;name namelen
			var namelen uint8 = 0
			binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
			reqlength := int(8 + 1 + namelen)
			if packetlen == reqlength {
				this.OnRequestDelGameRole(msg)
			}
		}
	case loginopstart + 8:
		{
			//	player request to login gamesvr
			//	namelen 1byte;name namelen;svrindex 1byte
			var namelen uint8 = 0
			binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
			reqlength := int(8 + 1 + namelen + 1)
			if packetlen == reqlength {
				this.OnRequestDelGameRole(msg)
			}
		}
	}
}

func (this *User) OnRequestAddGameRole(msg []byte) {
	var namelen uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+1 : 8+1+namelen])
	var job uint8 = 0
	var sex uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8+1+namelen:8+1+namelen+1]), binary.LittleEndian, &job)
	binary.Read(bytes.NewBuffer(msg[8+1+namelen+1:8+1+namelen+1+1]), binary.LittleEndian, &sex)
	//	Add a role
	/*log.Println(name)
	if !PathExist("./netusers") {
		err := os.Mkdir("./netusers", os.ModeDir)
		if err != nil {
			log.Println("Can't create parent directory.Error:", err)
			return
		}
	}*/
	//	Create sub directory
	userpath := "./login/" + strconv.FormatUint(uint64(this.uid), 10)
	if !PathExist(userpath) {
		err := os.Mkdir(userpath, os.ModeDir)
		if err != nil {
			log.Println("Cant't create user directory.Error:", err)
			return
		}
	}
	//	Create save file
	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	//	Open it
	r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	if r1 == 0 {
		log.Println("Can't open hum save.Err:", r1)
		return
	}
	//	Add game role
	var filehandle uintptr = r1
	r1, _, _ = g_procMap["AddGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString(name))))
	if r1 == 0 {
		//	Success
	} else {
		//	failed
	}

	//	Close
	g_procMap["CloseHumSave"].Call(filehandle)
}

func (this *User) OnRequestDelGameRole(msg []byte) {
	var namelen uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+1 : 8+1+namelen])
	//	delete a role
	log.Println(name)

	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	if !PathExist(userfile) {
		log.Printf("non-exist user[%d] request to delete gamerole")
		return
	}
	filehandle, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	if filehandle == 0 {
		log.Println("Can't open hum save.")
		return
	}

	r1, _, _ := g_procMap["DelGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString(name))))
	if r1 != 0 {
		log.Println("Can't remove gamerole ", name)
	}

	//	success , send a packet
	//	delete role result,namelen 4byte;name namelen
	buf := new(bytes.Buffer)
	namelen = uint8(len(name))
	namedata := []byte(name)
	binary.Write(buf, binary.LittleEndian, &namelen)
	binary.Write(buf, binary.LittleEndian, namedata[0:])
	this.SendUserMsg(loginopstart+7, buf.Bytes())
	g_procMap["CloseHumSave"].Call(filehandle)
}

func (this *User) OnRequestLoginGameSvr(msg []byte) {
	var namelen uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+1 : 8+1+namelen])
	var svrindex uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8+1+namelen:8+1+namelen+1]), binary.LittleEndian, &svrindex)
	//	read role data
	//	send the data to the gamesvr
	log.Println(name)

	if len(g_ServerList.allusers) == 0 {
		//	no available game server
		buf := new(bytes.Buffer)
		var qm uint16 = 1
		binary.Write(buf, binary.LittleEndian, &qm)
		this.SendUserMsg(loginopstart+13, buf.Bytes())
	} else {
		userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
		if !PathExist(userfile) {
			log.Printf("non-exist user[%d] request to delete gamerole")
			buf := new(bytes.Buffer)
			var qm uint16 = 2
			binary.Write(buf, binary.LittleEndian, &qm)
			this.SendUserMsg(loginopstart+13, buf.Bytes())
			return
		}
		filehandle, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
		if filehandle == 0 {
			log.Println("Can't open hum save.")
			buf := new(bytes.Buffer)
			var qm uint16 = 2
			binary.Write(buf, binary.LittleEndian, &qm)
			this.SendUserMsg(loginopstart+13, buf.Bytes())
			return
		}
		//var dtsize uint32 = 0
		//r1, _, _ := g_procMap["GetGameRoleData"].Call(filehandle,)
		//buf := new(bytes.Buffer)
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

func (this *User) VerifyUser(account, password string) int {
	var ret int = 0
	if !dbUserAccountExist(g_DBUser, account) {
		// non-exist account
		ret = 1
	}

	var info UserAccountInfo
	dbret, _ := dbGetUserAccountInfo(g_DBUser, account, &info)
	if !dbret {
		ret = 1
	} else {
		if password != info.password {
			ret = 2
		} else {
			//	pass
			this.uid = info.uid
		}
	}

	return ret
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

func (this *ServerUser) OnConnect() {
	log.Println("GameServer ", this.ipaddr, " connected...")
}

func (this *ServerUser) OnVerified() {

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
	case loginopstart + 9:
		{
			//	login gamesvr result
			//	ret 1byte;client index uint32;addrlen 1byte;addr addrlen
			var ret uint8 = 0
			var addrlen uint8 = 0
			binary.Read(bytes.NewBuffer(msg[9:9+1]), binary.LittleEndian, &ret)
			binary.Read(bytes.NewBuffer(msg[9+1:9+2]), binary.LittleEndian, &addrlen)
			reqlength := int(8 + 1 + 1 + addrlen)
			if len(msg) == reqlength {
				this.OnResponseClientLogin(msg)
			}
		}
	}
}

func (this *ServerUser) OnResponseClientLogin(msg []byte) {
	var ret uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &ret)
	if ret == 1 {
		var clientindex uint32 = 0
		binary.Read(bytes.NewBuffer(msg[8+1:8+1+4]), binary.LittleEndian, &ret)
		var addrlen uint8 = 0
		binary.Read(bytes.NewBuffer(msg[8+1+4:8+1+4+1]), binary.LittleEndian, &addrlen)
		var addr string = string(msg[8+1+4+1 : 8+1+4+1+addrlen])
		//	send to client
		log.Println(clientindex)
		log.Println(addr)
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

					var ret int = cltuser.VerifyUser(namestr, pswstr)
					if 0 != ret {
						//	failed
						var bret byte = byte(ret)
						cltuser.SendUserMsg(loginopstart+3, &bret)
					} else {
						var bret byte = 0
						cltuser.SendUserMsg(loginopstart+3, &bret)
						cltuser.verified = true
						cltuser.OnVerified()
					}
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
