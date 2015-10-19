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
	"strings"
	"time"
	"unsafe"
)

type ServerUser struct {
	User
	//	for server
	serverid     uint16
	serverlsaddr string
	//	for controller
	ctrlverify bool
}

func CreateServerUser(clientconn *server.Connection) *ServerUser {
	user := &ServerUser{
		serverid:   0,
		ctrlverify: false,
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
	log.Println("GameServer ", this.ipaddr, " connected... id:", this.serverid)
	if this.serverid >= 0 &&
		this.serverid < 100 {

	}
	rankListData := getPlayerRankList()
	//log.Println("get rank list:", rankListData)
	this.SendUserMsg(loginopstart+22, rankListData)
}

func (this *ServerUser) OnVerified() {

}

func (this *ServerUser) OnDisconnect() {
	if g_AvaliableGS == uint32(this.serverid) {
		g_AvaliableGS = 0
		log.Println("Lose game server...")
	}
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
	case loginopstart + 10:
		{
			//	player request to save
			this.OnRequestSave(msg)
		}
	case loginopstart + 16:
		{
			//	connidx
			/*var gsidx uint32 = 0
			var lsidx uint32 = 0
			binary.Read(bytes.NewBuffer(msg[8:8+4]), binary.LittleEndian, &gsidx)
			binary.Read(bytes.NewBuffer(msg[8+4:8+4+4]), binary.LittleEndian, &lsidx)

			cuser := g_UserList.GetUser(lsidx)
			if cuser == nil {
				log.Println("Can't registe user[", lsidx, "]")
			} else {
				user := cuser.(*User)
				user.svrconnidx = gsidx
				log.Println("Registe user gs index ok!")
			}*/
		}
	case loginopstart + 17:
		{
			//	player request to save data
			var namelen uint8
			var userindex uint32
			binary.Read(bytes.NewBuffer(msg[8+8:8+8+1]), binary.LittleEndian, &namelen)
			binary.Read(bytes.NewBuffer(msg[8:8+4]), binary.LittleEndian, &userindex)
			var uid uint32
			binary.Read(bytes.NewBuffer(msg[8+4:8+4+4]), binary.LittleEndian, &uid)
			var datalen uint32
			binary.Read(bytes.NewBuffer(msg[8+8+1+namelen+2:8+8+1+namelen+2+4]), binary.LittleEndian, &datalen)
			var calclen int = int(uint32(namelen) + datalen + 1 + 4 + 4 + 2 + 8 + 4)

			iuser := g_UserList.GetUser(userindex)
			var cuser *User
			var ok bool = false
			if nil == iuser {
				log.Println("Can't get the player wants save data")
				this.OnOfflineSave(msg)
				return
			} else {
				cuser, ok = iuser.(*User)
				if !ok {
					log.Println("Can't transform IUser to *User")
					return
				}
			}

			if len(msg) != calclen {
				log.Println("Invalid packet length[", len(msg), "], calc[", calclen, "]", "namelen", namelen, "datalen", datalen)
			} else {
				if uid != 0 {
					cuser.OnRequestSaveGameRole(msg)
				}
			}
		}
	case loginopstart + 19:
		{
			//	connidx
			var gsidx uint32 = 0
			var lsidx uint32 = 0
			var conncode uint32 = 0
			binary.Read(bytes.NewBuffer(msg[8:8+4]), binary.LittleEndian, &gsidx)
			binary.Read(bytes.NewBuffer(msg[8+4:8+4+4]), binary.LittleEndian, &lsidx)
			binary.Read(bytes.NewBuffer(msg[8+4+4:8+4+4+4]), binary.LittleEndian, &conncode)

			cuser := g_UserList.GetUser(lsidx)
			if cuser == nil {
				log.Println("Can't registe user[", lsidx, "]")
			} else {
				user := cuser.(*User)
				user.svrconnidx = gsidx
				user.conncode = conncode
				log.Println("Registe user gs index ok! gs index ", gsidx, " conn code:", conncode)
			}
		}
	case loginopstart + 21:
		{
			//	update player rank
			var uid uint32 = 0
			var nameLength int32 = 0
			var name string
			var level int32 = 0
			var job int8 = 0
			var power int32 = 0

			//	read
			binary.Read(bytes.NewBuffer(msg[8:8+4]), binary.LittleEndian, &uid)
			binary.Read(bytes.NewBuffer(msg[8+4:8+4+4]), binary.LittleEndian, &nameLength)
			if nameLength != 0 {
				name = string(msg[8+4+4 : 8+4+4+nameLength])
				nameLength++
			} else {
				log.Println("Trying to update player rank with no name.")
				return
			}

			binary.Read(bytes.NewBuffer(msg[8+4+4+nameLength:8+4+4+nameLength+4]), binary.LittleEndian, &level)
			binary.Read(bytes.NewBuffer(msg[8+4+4+nameLength+4:8+4+4+nameLength+4+1]), binary.LittleEndian, &job)
			binary.Read(bytes.NewBuffer(msg[8+4+4+nameLength+4+1:]), binary.LittleEndian, &power)

			if 0 == level {
				log.Println("Trying to update player rank with 0 level.")
				return
			}

			//log.Println("Update player rank:", name, "uid:", uid, "job:", job, "level:", level, "power:", power)
			var rankInfo UserRankInfo
			rankInfo.Uid = uid
			rankInfo.Job = int(job)
			rankInfo.Level = int(level)
			rankInfo.Name = name
			rankInfo.Power = int(power)
			if !dbUpdateUserRankInfo(g_DBUser, &rankInfo) {
				log.Println("Failed to insert player rank info")
			}
		}
	}
}

func OfflineSaveUserData(msg []byte) {
	var namelen uint8
	binary.Read(bytes.NewBuffer(msg[8+8:8+8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+8+1 : 8+8+1+namelen])
	var datalen uint32
	binary.Read(bytes.NewBuffer(msg[8+8+1+namelen+2:8+8+1+namelen+2+4]), binary.LittleEndian, &datalen)
	var data []byte = msg[8+8+1+namelen+2+4 : 8+8+1+uint32(namelen)+2+4+datalen]
	var uid uint32
	binary.Read(bytes.NewBuffer(msg[8+4:8+4+4]), binary.LittleEndian, &uid)

	log.Println(name, " request to save data on offline mode.")

	//	Create save file
	userfile := "./login/" + strconv.FormatUint(uint64(uid), 10) + "/hum.sav"
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))
	//no free !r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
	//	Open it
	//no free !r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
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

func (this *ServerUser) OnOfflineSave(msg []byte) {
	OfflineSaveUserData(msg)
	/*var namelen uint8
	binary.Read(bytes.NewBuffer(msg[8+8:8+8+1]), binary.LittleEndian, &namelen)
	var name string = string(msg[8+8+1 : 8+8+1+namelen])
	var datalen uint32
	binary.Read(bytes.NewBuffer(msg[8+8+1+namelen+2:8+8+1+namelen+2+4]), binary.LittleEndian, &datalen)
	var data []byte = msg[8+8+1+namelen+2+4 : 8+8+1+uint32(namelen)+2+4+datalen]
	var uid uint32
	binary.Read(bytes.NewBuffer(msg[8+4:8+4+4]), binary.LittleEndian, &uid)

	log.Println(name, " request to save data on offline mode.")

	//	Create save file
	userfile := "./login/" + strconv.FormatUint(uint64(uid), 10) + "/hum.sav"
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))
	//no free !r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
	//	Open it
	//no free !r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
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
	}*/
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

func (this *ServerUser) OnRequestSave(msg []byte) {
	userfile := "./login/" + strconv.FormatUint(uint64(this.uid), 10) + "/hum.sav"
	//	Open it
	cuserfile := C.CString(userfile)
	defer C.free(unsafe.Pointer(cuserfile))
	// no free!r1, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString(userfile))))
	r1, _, _ := g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(cuserfile)))
	if r1 == 0 {
		log.Println("Can't open hum save.Err:", r1)
		return
	}
	var filehandle uintptr = r1
	//	Close
	defer g_procMap["CloseHumSave"].Call(filehandle)

	//	write
	var lsvrconnidx uint32
	var namelen byte
	var name string
	var level uint16
	var datalen uint32

	buf := bytes.NewBuffer(msg[8:])
	binary.Read(buf, binary.LittleEndian, &lsvrconnidx)
	binary.Read(buf, binary.LittleEndian, &namelen)
	name = string(msg[8+4+1 : 8+4+1+namelen])
	buf = bytes.NewBuffer(msg[8+4+1+namelen:])
	binary.Read(buf, binary.LittleEndian, &level)
	binary.Read(buf, binary.LittleEndian, &datalen)
	humdata := msg[8+4+1+namelen:]

	cname := C.CString(name)
	//	no free
	C.free(unsafe.Pointer(cname))
	r1, _, _ = g_procMap["WriteGameRoleData"].Call(filehandle,
		uintptr(unsafe.Pointer(cname)),
		uintptr(unsafe.Pointer(&humdata[0])),
		uintptr(datalen))
	if r1 != 0 {
		var qm uint16 = 9
		this.SendUserMsg(loginopstart+12, &qm)
		return
	}
	g_procMap["UpdateGameRoleInfo"].Call(filehandle,
		uintptr(level))
}

func ControlValid(addr string) bool {
	if len(g_ControlAddr) == 0 {
		return false
	}

	for _, v := range g_ControlAddr {
		if v == addr {
			return true
		}
	}

	return false
}

func ReadControlAddr(path string) bool {
	//	read control addr
	file, err := os.Open(path)
	if err != nil {
		log.Println(err)
		return false
	}

	buf := make([]byte, 256)
	defer file.Close()
	readbytes, readerr := file.Read(buf)
	if readerr != nil {
		log.Println(err)
		return false
	}

	content := string(buf[0:readbytes])
	g_ControlAddr = strings.Split(content, "\r\n")

	for _, v := range g_ControlAddr {
		log.Println("Controller: ", v, " length", len(v))
	}
	return true
}
