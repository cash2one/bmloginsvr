package main

import (
	"encoding/json"
	"log"
	"strconv"
	//	"strings"
	"time"
)

type MThreadMsg struct {
	Event   int
	WParam  int
	LParam  int
	Msg     string
	RetChan chan bool
	Data    interface{}
}

type UserGameServer struct {
	Id           int    `json:"Id"`
	Address      string `json:"Address"`
	Port         int    `json:"Port"`
	Note         string `json:"Note"`
	Online       int    `json:Online`
	Password     string `json:"Password"`
	Version      string `json:"Version"`
	LastLiveTime int64
}

const (
	kMThreadMsg_None = iota
	kMThreadMsg_RegisterGS
	kMThreadMsg_GetGsList
	kMThreadMsg_GetGsAddr
	kMThreadMsg_RemoveGsAddr
)

var g_chanMainThread chan *MThreadMsg
var g_UserGameServerMap map[string]*UserGameServer
var g_UserGameServerSeed int

func MainThreadInit() {
	g_chanMainThread = make(chan *MThreadMsg, 20)
	g_UserGameServerMap = make(map[string]*UserGameServer)
}

func PostMThreadMsg(msg *MThreadMsg) {
	g_chanMainThread <- msg
}

//	user game server process
func AddUserGameServer(gs *UserGameServer) int {
	keyString := gs.Address + ":" + strconv.Itoa(gs.Port)
	rd, ok := g_UserGameServerMap[keyString]
	if !ok {
		//	new record
		rd = &UserGameServer{}
		*rd = *gs
		rd.LastLiveTime = time.Now().Unix()
		g_UserGameServerMap[keyString] = rd

		//	get id
		g_UserGameServerSeed++
		rd.Id = g_UserGameServerSeed
	} else {
		//	update record
		rd.LastLiveTime = time.Now().Unix()
		rd.Note = gs.Note
		rd.Online = gs.Online
	}

	return rd.Id
}

func RemoveUserGameServer(id int) {
	for k, ug := range g_UserGameServerMap {
		if ug.Id == id {
			delete(g_UserGameServerMap, k)
			return
		}
	}
}

func CheckOfflineUserGameServer() {
	nowTime := time.Now().Unix()

	for k, v := range g_UserGameServerMap {
		if nowTime-v.LastLiveTime > 90 {
			delete(g_UserGameServerMap, k)
		}
	}
}

func UpdateMThreadMsg() {
	CheckOfflineUserGameServer()
}

func ProcessMThreadMsg(msg *MThreadMsg) {
	switch msg.Event {
	case kMThreadMsg_RegisterGS:
		{
			//	注册游戏服务器到大厅
			var ugs UserGameServer
			ret := false

			if len(msg.Msg) != 0 {
				err := json.Unmarshal([]byte(msg.Msg), &ugs)
				if err != nil {
					log.Println("Failed to unmarshal content:", msg.Msg, "error:", err)
				} else {
					msg.WParam = AddUserGameServer(&ugs)
					ret = true
				}
			}

			if nil != msg.RetChan {
				msg.RetChan <- ret
			}
		}
	case kMThreadMsg_GetGsList:
		{
			gsList := make([]UserGameServer, len(g_UserGameServerMap))
			index := 0

			for _, ug := range g_UserGameServerMap {
				gsList[index] = *ug
				if len(ug.Password) != 0 {
					gsList[index].Password = "1"
					gsList[index].Port = 0
				}
				index++
			}

			msg.Data = gsList
			if nil != msg.RetChan {
				msg.RetChan <- true
			}
		}
	case kMThreadMsg_GetGsAddr:
		{
			ret := false

			for _, ug := range g_UserGameServerMap {
				if ug.Id == msg.WParam {
					if len(ug.Password) == 0 ||
						(len(ug.Password) != 0 && ug.Password == msg.Msg) {
						ret = true
						msg.LParam = ug.Port
						msg.Msg = ug.Address
					}
					break
				}
			}

			if nil != msg.RetChan {
				msg.RetChan <- ret
			}
		}
	case kMThreadMsg_RemoveGsAddr:
		{
			RemoveUserGameServer(msg.WParam)
			if nil != msg.RetChan {
				msg.RetChan <- true
			}
		}
	default:
		{
			log.Println("Unprocessed mthread event id:", msg.Event)
		}
	}
}
