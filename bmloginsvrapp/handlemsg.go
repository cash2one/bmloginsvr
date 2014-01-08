package main

//#include <stdlib.h>
import "C"

import (
	"bytes"
	"encoding/binary"
	"log"
	//	"os"
	"server"
	//	"strconv"
	//	"time"
	//	"unsafe"
)

var (
	loginopstart uint32 = 10000
	//	+0	force quit
	//	+1	server verify	//	verify msg,serverid 4bytes;verifycode 4bytes
	//	+2	client verify	//	namelen 4bytes;name namelen;pswlen 4bytes;psw pswlen
	//	+3	verify result	//	ret:1byte
	//	+4	client request to add game role	//	namelen 1byte;name namelen;job 1byte;sex 1byte
	//	+5	add role result	//	ret 1byte;namelen 1byte;name namelen;job 1byte;sex 1byte
	//	+6	client request to delete game role	//	namelen 1byte;name namelen
	//	+7	delete role result	//	namelen 1byte;name namelen
	//	+8	client request to login gamesvr	//	namelen 1byte;name namelen;svrindex 1byte
	//	+9	login gamesvr result	//	0
	//	+10 client request to save
	//	+11 send gamerole data to gameserver
	//	+12 send quick message	//	msg 2bytes
	//	+13 game type 1:normal 2:login
	//	+14	client start game	//	0
)

/*
//	quick message
	0:none
	1:没有可用的游戏服务器
	2:任务存档不存在
	3:异常的存档读取
*/

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
					//	user name and password	namelen 4bytes;name namelen;pswlen 4bytes;psw pswlen
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
