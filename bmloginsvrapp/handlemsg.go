package main

//#include <stdlib.h>
import "C"

import (
	"bytes"
	"encoding/binary"
	"log"
	//	"os"
	"server"
	//	"strings"
	//	"strconv"
	//	"time"
	//	"unsafe"
)

var (
	loginopstart uint32 = 10000
	//	+0	force quit
	//	+1	server verify	//	verify msg,serverid 4bytes;verifycode 4bytes;addrlen  1bytes;addr addrlen
	//	+2	client verify	//	namelen 4bytes;name namelen;pswlen 4bytes;psw pswlen
	//	+3	verify result	//	ret:1byte
	//	+4	client request to add game role	//	namelen 1byte;name namelen;job 1byte;sex 1byte
	//	+5	add role result	//	ret 1byte;namelen 1byte;name namelen;job 1byte;sex 1byte
	//	+6	client request to delete game role	//	namelen 1byte;name namelen
	//	+7	delete role result	//	namelen 1byte;name namelen
	//	+8	client request to login gamesvr	//	namelen 1byte;name namelen;svrindex 2byte
	//	+9	login gamesvr result	//	0
	//	+10 client request to save
	//	+11 send gamerole data to gameserver // svrconnidx 4bytes;lsconnidx 4bytes;heroheader;datalen 4bytes;data datalen
	//	+12 send quick message	//	msg 2bytes
	//	+13 game type //	type 1byte(1:normal 2:login);connindex 4bytes
	//	+14	client start game	//	0
	//	+15	server address info //	iplen 4byte;ip iplen;server index 2bytes
	//	+16	login-game connindex //	login connindex 4bytes;game connindex 4bytes
	//	+17	send gamerole data to loginserver // lsvrconnidx 4bytes;uid 4bytes;namelen 1byte;name namelen;level 2bytes;datalen 4bytes;data datalen
	//	+18	send gamerole head data to client	//	roleidx 1byte;namelen 1byte;name namelen;job 1byte;sex 1byte;level 2byte
	//	+19	login-game connindex //	login connindex 4bytes;game connindex 4bytes;conn code 4bytes
)

/*
//	quick message
	0:none
	1:没有可用的游戏服务器
	2:任务存档不存在
	3:异常的存档读取
	4:角色名存在
	5:不存在的玩家数据
	6:无法创建角色
	7:无可用的游戏服务器
	8:用户名或者密码错误
	9:存档失败
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
			log.Println("A exception occured while processing msg[length:", length, " opcode:", opcode, "]", err)
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
					var namelen uint8 = 0
					binary.Read(bytes.NewBuffer(data[8:8+1]), binary.LittleEndian, &namelen)
					namebuf := make([]byte, namelen)
					binary.Read(bytes.NewBuffer(data[9:9+namelen]), binary.BigEndian, namebuf)
					var namestr string = string(namebuf)
					var pswlen uint8 = 0
					binary.Read(bytes.NewBuffer(data[9+namelen:9+namelen+1]), binary.LittleEndian, &pswlen)
					namebuf = make([]byte, pswlen)
					binary.Read(bytes.NewBuffer(data[9+namelen+1:9+namelen+1+pswlen]), binary.BigEndian, namebuf)
					var pswstr string = string(namebuf)

					log.Println("Begin to verify user " + namestr)

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

	//log.Println("Receive server[", msg.Conn.GetConnTag(), "] msg[length:", length, " opcode:", opcode, "]")

	defer func() {
		err := recover()
		if err != nil {
			log.Println("A exception occured while processing msg[length:", length, " opcode:", opcode, "]", err)
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
					//	verify msg,serverid 4bytes;verifycode 4bytes;addrlen 1byte;addr addrlen
					if length >= 8+2+4+1 {
						var serverid uint16 = 0
						var verifycode uint32 = 0
						var iplen uint8 = 0
						binary.Read(bytes.NewBuffer(data[8:8+2]), binary.LittleEndian, &serverid)
						binary.Read(bytes.NewBuffer(data[10:10+4]), binary.LittleEndian, &verifycode)
						binary.Read(bytes.NewBuffer(data[10+4:10+4+1]), binary.LittleEndian, &iplen)

						if length == 8+2+4+1+uint32(iplen) {
							verifyok := false
							verifyok = true
							if verifyok {
								log.Println("server ", serverid, "verify ok")
								svruser.serverid = serverid
								svruser.serverlsaddr = string(data[10+4+1 : 10+4+1+iplen])

								var vok uint8 = 1
								user.SendUserMsg(loginopstart+3, &vok)
								//g_AvaliableGS = msg.Conn.GetConnTag()
								if svruser.serverid >= 0 && svruser.serverid < 100 {
									g_AvaliableGS = msg.Conn.GetConnTag()
									log.Println("Server[", serverid, "] registed... Tag[", g_AvaliableGS, "]")
									svruser.verified = true
								} else if svruser.serverid >= 100 && svruser.serverid < 150 {
									//	verify
									svruser.verified = true
								}
							} else {
								log.Println("server ", serverid, "verify failed")
								var vok uint8 = 0
								user.SendUserMsg(loginopstart+3, &vok)
							}
						} else {
							log.Println("verify length not equal", 8+2+4+1+uint32(iplen), " ", length)
						}
					} else {
						log.Println("verify pkg length not equal ", 8+2+4+1)
					}
				}
			} else {
				if svruser.serverid >= 0 && svruser.serverid < 100 {
					log.Println("Receive server[", svruser.serverid, "] msg[length:", length, " opcode:", opcode, "]")
					user.OnUserMsg(msg.Msg)
				} else if svruser.serverid >= 100 && svruser.serverid < 150 {
					//log.Println("Receive ctrl terminal[", svruser.serverid, "] msg[length:", length, "]")
					svruser.OnCtrlMsg(msg.Msg)
				}
			}
		} else {
			log.Println("Can't convert user to type ServerUser")
		}
	} else {
		log.Println("Can't find the server tag[", msg.Conn.GetConnTag(), "]")
	}
}
