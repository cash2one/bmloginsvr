package main

//#include <stdlib.h>
import "C"

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
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

	//	Send game type
	var gametype uint8 = 2
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.LittleEndian, &gametype)
	var loginconnidx = this.GetUserTag()
	binary.Write(buf, binary.LittleEndian, &loginconnidx)
	this.SendUseData(loginopstart+13, buf.Bytes())
}

func (this *User) OnDisconnect() {
	log.Println("Peer ", this.ipaddr, " disconnected...")
	this.SendUserMsg(loginopstart, nil)
}

func (this *User) OnVerified() {
	//	Get the uid,so we create the directory and file
	//	Create sub directory
	defer func() {
		err := recover()
		if err != nil {
			fmt.Println(err)
			var input string
			fmt.Scanln(&input)
		}
	}()

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
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))
	// no free!g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))

	//	Open it
	// no free!r1, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
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

func (this *User) OnRequestSaveGameRole(msg []byte) {
	var namelen uint8
	binary.Read(bytes.NewBuffer(msg[8+8:8+8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+8+1 : 8+8+1+namelen])
	var datalen uint32
	binary.Read(bytes.NewBuffer(msg[8+8+1+namelen+2:8+8+1+namelen+2+4]), binary.LittleEndian, &datalen)
	var data []byte = msg[8+8+1+namelen+2+4 : 8+8+1+uint32(namelen)+2+4+datalen]

	log.Println(name, " request to save data.")

	//	Create save file
	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))

	//	no free!r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))

	//	Open it
	// no free!r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
	if r1 == 0 {
		log.Println("Can't open hum save.Err:", r1)
		return
	}
	var filehandle uintptr = r1
	//	Close
	defer g_procMap["CloseHumSave"].Call(filehandle)

	cname := C.CString(name)
	//	no free!
	defer C.free(unsafe.Pointer(cname))

	var level uint16
	binary.Read(bytes.NewBuffer(msg[8+8+1+namelen:8+8+1+namelen+2]), binary.LittleEndian, &level)
	r1, _, _ = g_procMap["UpdateGameRoleInfo"].Call(filehandle, uintptr(unsafe.Pointer(cname)), uintptr(level))
	if r1 != 0 {
		log.Println("Failed to update gamerole head data")
	}

	r1, _, _ = g_procMap["WriteGameRoleData"].Call(filehandle, uintptr(unsafe.Pointer(cname)), uintptr(unsafe.Pointer(&data[0])), uintptr(datalen))
	if r1 != 0 {
		log.Println("Failed to write gamerole data")
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
	log.Println("Create new hero name ", name, " job ", job, " sex ", sex)
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
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))

	//	no free!r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))

	//	Open it
	//	no free!r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
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

	//	Add game role
	cname := C.CString(name)
	defer C.free(unsafe.Pointer(cname))

	r1, _, _ = g_procMap["AddGameRole"].Call(filehandle,
		//	no free!uintptr(unsafe.Pointer(C.CString(name))),
		uintptr(unsafe.Pointer(cname)),
		uintptr(job),
		uintptr(sex))
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
		this.SendUseData(loginopstart+5, buf.Bytes())

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
	} else {
		//	failed
		buf := new(bytes.Buffer)
		var ret uint8 = 0
		binary.Write(buf, binary.LittleEndian, &ret)
		binary.Write(buf, binary.LittleEndian, &namelen)
		binary.Write(buf, binary.LittleEndian, []byte(name))
		binary.Write(buf, binary.LittleEndian, &job)
		binary.Write(buf, binary.LittleEndian, &sex)
		this.SendUseData(loginopstart+5, buf.Bytes())
	}
}

func (this *User) OnRequestDelGameRole(msg []byte) {
	var namelen uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+1 : 8+1+namelen])
	//	delete a role
	log.Println(name)

	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))

	if !PathExist(userfile) {
		log.Printf("non-exist user[%d] request to delete gamerole")
		return
	}
	//	no free!filehandle, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	filehandle, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
	if filehandle == 0 {
		log.Println("Can't open hum save.")
		return
	}
	defer g_procMap["CloseHumSave"].Call(filehandle)

	cname := C.CString(name)
	defer C.free(unsafe.Pointer(cname))

	r1, _, _ := g_procMap["DelGameRole"].Call(filehandle,
		//	no free!uintptr(unsafe.Pointer(C.CString(name))))
		uintptr(unsafe.Pointer(cname)))
	if r1 != 0 {
		log.Println("Can't remove gamerole ", name)
		return
	}

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

	//	success , send a packet
	//	delete role result,namelen 1byte;name namelen
	buf := new(bytes.Buffer)
	namelen = uint8(len(name))
	namedata := []byte(name)
	binary.Write(buf, binary.LittleEndian, &namelen)
	binary.Write(buf, binary.LittleEndian, namedata[0:])
	this.SendUseData(loginopstart+7, buf.Bytes())
}

type UserLoginExtendInfo struct {
	DonateMoney int32
	SystemGift  []int
}

func (this *User) OnRequestLoginGameSvr(msg []byte) {
	var namelen uint8 = 0
	binary.Read(bytes.NewBuffer(msg[8:8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+1 : 8+1+namelen])
	var svrindex uint16 = 0
	binary.Read(bytes.NewBuffer(msg[8+1+namelen:8+1+namelen+2]), binary.LittleEndian, &svrindex)
	//	read role data
	//	send the data to the gamesvr
	log.Println(name, " request to enter game server")

	if this.svrconnidx == 0 {
		log.Println("invalid svr index")
		return
	}

	//	Get the avaliable game server
	igs := g_ServerList.GetUser(g_AvaliableGS)
	if nil == igs {
		var qm uint16 = 7
		this.SendUserMsg(loginopstart+12, &qm)
		log.Println("Tag[", g_AvaliableGS, "] not available")
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
		log.Println("Tag[", g_AvaliableGS, "] not available")
		return
	}

	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))

	if !PathExist(userfile) {
		log.Printf("non-exist user[%d] request to login gamerole")
		buf := new(bytes.Buffer)
		var qm uint16 = 2
		binary.Write(buf, binary.LittleEndian, &qm)
		this.SendUserMsg(loginopstart+12, buf.Bytes())
		return
	}

	//	no free!filehandle, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	filehandle, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
	if filehandle == 0 {
		log.Println("Can't open hum save.")
		var qm uint16 = 2
		this.SendUserMsg(loginopstart+12, &qm)
		return
	}

	defer g_procMap["CloseHumSave"].Call(filehandle)

	cname := C.CString(name)
	defer C.free(unsafe.Pointer(cname))

	var newhum bool = false
	var datasize uint32 = 0
	//	Get data size
	r1, _, _ := g_procMap["ReadGameRoleSize"].Call(filehandle,
		uintptr(unsafe.Pointer(cname)))
	if r1 == 0 {
		//var qm uint16 = 5
		//this.SendUserMsg(loginopstart+12, &qm)
		//return
		newhum = true
	} else {
		datasize = uint32(r1)
	}

	//	read head
	buf := new(bytes.Buffer)
	binary.Write(buf, binary.LittleEndian, &this.svrconnidx)
	var tag uint32 = this.conn.GetConnTag()
	binary.Write(buf, binary.LittleEndian, &tag)
	binary.Write(buf, binary.LittleEndian, &this.uid)

	r1, _, _ = g_procMap["GetGameRoleIndex"].Call(filehandle, uintptr(unsafe.Pointer(cname)))
	if r1 < 0 || r1 > 2 {
		var qm uint16 = 5
		this.SendUserMsg(loginopstart+12, &qm)
		log.Println("Can't get role index")
		return
	}

	var heroidx int = int(r1)
	var job, sex uint8
	var level uint16
	var namelen4 uint32
	var zero uint8
	r1, _, _ = g_procMap["GetGameRoleInfo_Value"].Call(filehandle, uintptr(heroidx), uintptr(unsafe.Pointer(&job)),
		uintptr(unsafe.Pointer(&sex)), uintptr(unsafe.Pointer(&level)))
	namelen4 = uint32(len(name))
	log.Println(name, "job ", job, " sex ", sex, "login success")
	binary.Write(buf, binary.LittleEndian, &namelen4)
	binary.Write(buf, binary.LittleEndian, []byte(name))
	binary.Write(buf, binary.LittleEndian, &zero)
	binary.Write(buf, binary.LittleEndian, &sex)
	binary.Write(buf, binary.LittleEndian, &job)
	binary.Write(buf, binary.LittleEndian, &level)

	if newhum {
		datasize = 0
		binary.Write(buf, binary.LittleEndian, &datasize)

		//	donate money
		//binary.Write(buf, binary.LittleEndian, &donateInfo.donate)
		//gs.SendUseData(loginopstart+11, buf.Bytes())
	} else {
		log.Println("Not new hum, read size ", datasize)

		humdata := make([]byte, datasize)
		r1, _, _ = g_procMap["ReadGameRoleData"].Call(filehandle,
			uintptr(unsafe.Pointer(cname)),
			uintptr(unsafe.Pointer(&humdata[0])))
		if r1 != 0 {
			var qm uint16 = 3
			this.SendUserMsg(loginopstart+12, &qm)
			return
		}

		//	send gamerole data to server
		var datalen uint32 = uint32(len(humdata))
		binary.Write(buf, binary.LittleEndian, &datalen)
		binary.Write(buf, binary.LittleEndian, humdata)
		//	donate money
		//binary.Write(buf, binary.LittleEndian, &donateInfo.donate)
		//gs.SendUseData(loginopstart+11, buf.Bytes())
	}

	//	发送登录扩展信息，json格式
	extInfo := &UserLoginExtendInfo{}
	donateInfo := &UserDonateInfo{}
	if dbGetUserDonateInfo(g_DBUser, this.uid, donateInfo) {
		//	nothing
		log.Println("player[", this.uid, "] donate money:", donateInfo.donate)
	}
	extInfo.DonateMoney = donateInfo.donate
	extInfo.SystemGift = dbGetSystemGiftIdByUid(g_DBUser, this.uid)
	binaryExtInfo, jsErr := json.Marshal(extInfo)
	if jsErr != nil {
		log.Println("failed to marshal user extend login information:", jsErr)

		//	字符长度为0
		zeroLength := uint32(0)
		binary.Write(buf, binary.LittleEndian, &zeroLength)
	} else {
		//	发送扩展信息
		zeroTerminate := uint8(0)
		jsonLength := uint32(len(binaryExtInfo))

		binary.Write(buf, binary.LittleEndian, &jsonLength)
		if 0 != jsonLength {
			//	写入json数据
			binary.Write(buf, binary.LittleEndian, binaryExtInfo)
			//	写入结束符
			binary.Write(buf, binary.LittleEndian, &zeroTerminate)
		}
	}

	gs.SendUseData(loginopstart+11, buf.Bytes())
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
	defer func() {
		err := recover()
		if err != nil {
			fmt.Println(err)
			var input string
			fmt.Scanln(&input)
		}
	}()

	var ret int = 0
	if !dbUserAccountExist(g_DBUser, account) {
		// non-exist account
		ret = 1
	}

	var info UserAccountInfo
	dbret, _ := dbGetUserAccountInfo(g_DBUser, account, &info)
	log.Println("Accout ", info.account, " Password ", info.password)
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
			igs := g_ServerList.GetUser(g_AvaliableGS)
			if igs != nil {
				gs, ok := igs.(*ServerUser)
				if ok {
					buf := new(bytes.Buffer)
					iplen := int32(len(gs.serverlsaddr))
					binary.Write(buf, binary.LittleEndian, &iplen)
					binary.Write(buf, binary.LittleEndian, []byte(gs.serverlsaddr))
					var zero uint8 = 0
					binary.Write(buf, binary.LittleEndian, &zero)
					var svridx uint16 = 1
					binary.Write(buf, binary.LittleEndian, &svridx)
					this.SendUseData(loginopstart+15, buf.Bytes())

					log.Println("Pass")
				}
			} else {
				log.Println("Server tag[", g_AvaliableGS, "] not available")
			}
		}
	} else {
		var msg uint16 = 8
		this.SendUserMsg(loginopstart+12, &msg)
	}

	return ret
}
