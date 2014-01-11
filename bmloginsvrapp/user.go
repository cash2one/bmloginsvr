package main

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
	ipaddr     string
	uid        uint32
	svrconnidx uint32
	conn       *server.Connection
	verified   bool
	conntime   time.Time
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

	//	Send game type
	var gametype uint8 = 1
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.LittleEndian, &gametype)
	var loginconnidx = this.GetUserTag()
	binary.Write(buf, binary.LittleEndian, &loginconnidx)
	this.SendUseData(loginopstart+13, buf.Bytes())

	//	Open it
	r1, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	if r1 == 0 {
		log.Println("Can't open hum save.Err:", r1)
		return
	}
	var filehandle uintptr = r1
	//	Close
	defer g_procMap["CloseHumSave"].Call(filehandle)

	//	Send head info
	databuf := make([]byte, 100)
	var datalen uintptr
	for i := 0; i < 3; i++ {
		datalen, _, _ = g_procMap["ReadGameRoleHeadInfo"].Call(filehandle, uintptr(i), uintptr(unsafe.Pointer(&databuf[0])))
		if datalen != 0 {
			buf := new(bytes.Buffer)
			var roleidx byte = byte(i)
			var headsize int32 = int32(datalen)
			binary.Write(buf, binary.LittleEndian, &roleidx)
			binary.Write(buf, binary.LittleEndian, databuf[0:headsize])
			this.SendUseData(loginopstart+18, buf.Bytes())
		}
	}
}

func (this *User) SendUseData(opcode uint32, data []byte) bool {
	if opcode < loginopstart {
		return false
	}

	server.WriteMsgLittleEndian(this.conn, opcode, data)
	return true
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
			binary.Write(buf, binary.LittleEndian, []byte(*addr))
			server.WriteMsgLittleEndian(this.conn, opcode, buf.Bytes())
		}
	case loginopstart + 12:
		{
			var qkmsgcode uint16 = 0
			for i, v := range args {
				if i == 0 {
					switch argtype := v.(type) {
					case *uint16:
						{
							qkmsgcode = *argtype
						}
					default:
						{
							logSendMsgTypeErr(opcode, "", "*uint16")
							return false
						}
					}
				}
			}

			buf := new(bytes.Buffer)
			binary.Write(buf, binary.LittleEndian, &qkmsgcode)
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
			reqlength := int(8 + 1 + namelen + 2)
			if packetlen == reqlength {
				this.OnRequestLoginGameSvr(msg)
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
	var filehandle uintptr = r1
	//	Close
	defer g_procMap["CloseHumSave"].Call(filehandle)

	//	Check username valid
	if dbUserNameExist(g_DBUser, name) {
		buf := new(bytes.Buffer)
		var msg uint16 = 4
		binary.Write(buf, binary.LittleEndian, &msg)
		this.SendUseData(loginopstart+12, buf.Bytes())
		return
	}

	//	Add user name
	var userinfo UserAccountInfo
	if !dbGetUserAccountInfoByUID(g_DBUser, this.uid, &userinfo) {
		buf := new(bytes.Buffer)
		var msg uint16 = 5
		binary.Write(buf, binary.LittleEndian, &msg)
		this.SendUseData(loginopstart+12, buf.Bytes())
		return
	}
	if !dbAddUserName(g_DBUser, userinfo.account, name) {
		buf := new(bytes.Buffer)
		var msg uint16 = 6
		binary.Write(buf, binary.LittleEndian, &msg)
		this.SendUseData(loginopstart+12, buf.Bytes())
		return
	}

	//	Add game role
	r1, _, _ = g_procMap["AddGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString(name))))
	if r1 == 0 {
		//	Success
		//	add role result ret 1byte;namelen 1byte;name namelen
		buf := new(bytes.Buffer)
		var ret uint8 = 0
		ret = 1
		binary.Write(buf, binary.LittleEndian, &ret)
		binary.Write(buf, binary.LittleEndian, &namelen)
		binary.Write(buf, binary.LittleEndian, []byte(name))
		binary.Write(buf, binary.LittleEndian, &job)
		binary.Write(buf, binary.LittleEndian, &sex)
		this.SendUseData(loginopstart+4, buf.Bytes())
	} else {
		//	failed
		buf := new(bytes.Buffer)
		var ret uint8 = 0
		binary.Write(buf, binary.LittleEndian, &ret)
		binary.Write(buf, binary.LittleEndian, &namelen)
		binary.Write(buf, binary.LittleEndian, []byte(name))
		binary.Write(buf, binary.LittleEndian, &job)
		binary.Write(buf, binary.LittleEndian, &sex)
		this.SendUseData(loginopstart+4, buf.Bytes())
	}
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
	defer g_procMap["CloseHumSave"].Call(filehandle)

	//	Remove name from db
	var userinfo UserAccountInfo
	if !dbGetUserAccountInfoByUID(g_DBUser, this.uid, &userinfo) {
		var msg uint16 = 5
		this.SendUserMsg(loginopstart+12, &msg)
		return
	}
	if !dbRemoveUserName(g_DBUser, userinfo.account, name) {
		var msg uint16 = 5
		this.SendUserMsg(loginopstart+12, &msg)
		return
	}

	r1, _, _ := g_procMap["DelGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString(name))))
	if r1 != 0 {
		log.Println("Can't remove gamerole ", name)
		return
	}

	//	success , send a packet
	//	delete role result,namelen 1byte;name namelen
	buf := new(bytes.Buffer)
	namelen = uint8(len(name))
	namedata := []byte(name)
	binary.Write(buf, binary.LittleEndian, &namelen)
	binary.Write(buf, binary.LittleEndian, namedata[0:])
	this.SendUserMsg(loginopstart+7, buf.Bytes())
}

func (this *User) OnRequestLoginGameSvr(msg []byte) {
	var namelen uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+1 : 8+1+namelen])
	var svrindex uint16 = 0
	binary.Read(bytes.NewBuffer(msg[8+1+namelen:8+1+namelen+2]), binary.LittleEndian, &svrindex)
	//	read role data
	//	send the data to the gamesvr
	log.Println(name)

	if this.svrconnidx == 0 {
		return
	}

	//	Get the avaliable game server
	igs := g_ServerList.GetUser(uint32(svrindex))
	if nil == igs {
		var qm uint16 = 7
		this.SendUserMsg(loginopstart+12, &qm)
		return
	}

	gs, ok := igs.(*ServerUser)
	if !ok {
		var qm uint16 = 7
		this.SendUserMsg(loginopstart+12, &qm)
		return
	}

	if len(g_ServerList.allusers) == 0 {
		//	no available game server
		buf := new(bytes.Buffer)
		var qm uint16 = 1
		binary.Write(buf, binary.LittleEndian, &qm)
		this.SendUserMsg(loginopstart+12, buf.Bytes())
		return
	}

	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	if !PathExist(userfile) {
		log.Printf("non-exist user[%d] request to login gamerole")
		buf := new(bytes.Buffer)
		var qm uint16 = 2
		binary.Write(buf, binary.LittleEndian, &qm)
		this.SendUserMsg(loginopstart+12, buf.Bytes())
		return
	}

	filehandle, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	if filehandle == 0 {
		log.Println("Can't open hum save.")
		var qm uint16 = 2
		this.SendUserMsg(loginopstart+12, &qm)
		return
	}

	defer g_procMap["CloseHumSave"].Call(filehandle)

	cname := C.CString(name)
	//	Get data size
	r1, _, _ := g_procMap["ReadGameRoleSize"].Call(filehandle,
		uintptr(unsafe.Pointer(cname)))
	if r1 == 0 {
		var qm uint16 = 5
		this.SendUserMsg(loginopstart+12, &qm)
		return
	}

	humdata := make([]byte, r1)
	r1, _, _ = g_procMap["ReadGameRoleData"].Call(filehandle,
		uintptr(unsafe.Pointer(cname)),
		uintptr(unsafe.Pointer(&humdata[0])))
	if r1 != 0 {
		var qm uint16 = 3
		this.SendUserMsg(loginopstart+12, &qm)
		return
	}

	//	send gamerole data to server
	buf := new(bytes.Buffer)
	var datalen uint32 = uint32(len(humdata))
	binary.Write(buf, binary.LittleEndian, &this.svrconnidx)
	binary.Write(buf, binary.LittleEndian, &datalen)
	binary.Write(buf, binary.LittleEndian, humdata)
	gs.SendUseData(loginopstart, buf.Bytes())
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

	if 0 == ret {
		//	send server information
		if len(g_ServerList.allusers) == 0 {
			var msg uint16 = 7
			this.SendUserMsg(loginopstart+12, &msg)
		} else {
			igs := g_ServerList.GetUser(1)
			if igs != nil {
				gs, ok := igs.(*ServerUser)
				if ok {
					buf := new(bytes.Buffer)
					iplen := len(gs.ipaddr)
					binary.Write(buf, binary.LittleEndian, &iplen)
					binary.Write(buf, binary.LittleEndian, []byte(gs.ipaddr))
					var svridx uint16 = 1
					binary.Write(buf, binary.LittleEndian, &svridx)
					this.SendUseData(loginopstart+15, buf.Bytes())
				}
			}
		}
	} else {
		var msg uint16 = 8
		this.SendUserMsg(loginopstart+12, &msg)
	}

	return ret
}
