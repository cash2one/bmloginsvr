package main

import (
	"encoding/json"
	"log"
	"net/http"
	_ "net/http/pprof"
	"runtime"
	"runtime/pprof"
	"strconv"
	"strings"
)

//	global variables
var g_enableGsListRequest bool

func init() {
	g_enableGsListRequest = true
}

func startHttpServer(addr string) {
	if len(addr) == 0 {
		return
	}
	http.HandleFunc("/removegs", removeGsAddrHandler)
	http.HandleFunc("/getgsaddr", getGsAddressHandler)
	http.HandleFunc("/registergs", registergsHandler)
	http.HandleFunc("/getgslist", getGsListHandler)
	http.HandleFunc("/debug", debugHandler)
	http.HandleFunc("/admin", adminHandler)
	http.HandleFunc("/rs", rsHandler)

	log.Println("Start http server:", addr)
	go http.ListenAndServe(addr, nil)
}

type registergsRsp struct {
	Result int    `json:"Result"`
	Msg    string `json:"Msg"`
}

type getGsListRsp struct {
	Result  int              `json:"Result"`
	Msg     string           `json:"Msg"`
	Servers []UserGameServer `json:"Servers"`
}

type DebugHandlerRsp struct {
	Result int    `json:"Result"`
	Msg    string `json:"Msg"`
}

func debugHandler(w http.ResponseWriter, r *http.Request) {
	var rsp DebugHandlerRsp
	defaultRsp := true

	defer func() {
		if defaultRsp {
			bytes, _ := json.Marshal(&rsp)
			w.Write(bytes)
		}
	}()

	debugType := r.FormValue("type")
	switch debugType {
	case "goroutinecount":
		{
			rsp.Msg = strconv.Itoa(runtime.NumGoroutine())
		}
	case "heap":
		{
			pprof.Lookup("heap").WriteTo(w, 1)
			defaultRsp = false
		}
	default:
		{
			rsp.Result = -1
			rsp.Msg = "invalid debug type"
		}
	}
}

func removeGsAddrHandler(w http.ResponseWriter, r *http.Request) {
	defer func() {
		exceptionDetails()
	}()

	id := r.FormValue("id")
	gsId, err := strconv.Atoi(id)

	if nil != err {
		return
	}
	if 0 == gsId {
		return
	}

	tmsg := &MThreadMsg{}
	tmsg.Event = kMThreadMsg_RemoveGsAddr
	tmsg.WParam = gsId
	tmsg.RetChan = make(chan bool, 1)
	PostMThreadMsg(tmsg)

	ret := <-tmsg.RetChan

	rsp := &registergsRsp{}
	rsp.Result = -1
	if ret {
		rsp.Result = 0
	}
	bytes, _ := json.Marshal(rsp)
	w.Write(bytes)
}

func getGsAddressHandler(w http.ResponseWriter, r *http.Request) {
	defer func() {
		exceptionDetails()
	}()

	rsp := &registergsRsp{}
	rsp.Result = -1

	id := r.FormValue("id")
	password := r.FormValue("password")

	gsId, err := strconv.Atoi(id)
	if err != nil {
		log.Println("Invalid id argument.content:", id, "error:", err)
		rsp.Msg = "Invalid id argument"
		bytes, _ := json.Marshal(rsp)
		w.Write(bytes)
		return
	}

	tmsg := &MThreadMsg{}
	tmsg.Event = kMThreadMsg_GetGsAddr
	tmsg.RetChan = make(chan bool, 1)
	tmsg.WParam = gsId
	tmsg.Msg = password
	PostMThreadMsg(tmsg)

	ret := <-tmsg.RetChan

	if ret {
		rsp.Result = 0
		rsp.Msg = tmsg.Msg + ":" + strconv.Itoa(tmsg.LParam)
		bytes, _ := json.Marshal(rsp)
		w.Write(bytes)
	} else {
		rsp.Msg = "Invalid game server id or incorrect password.please check again."
		log.Println(rsp.Msg)
		bytes, _ := json.Marshal(rsp)
		w.Write(bytes)
	}
}

func getGsListHandler(w http.ResponseWriter, r *http.Request) {
	defer func() {
		exceptionDetails()
	}()

	if !g_enableGsListRequest {
		w.Write([]byte("{\"Result\":0,\"Msg\":\"\",\"Servers\":[]}"))
		return
	}

	rsp := &getGsListRsp{}
	rsp.Result = -1

	tmsg := &MThreadMsg{}
	tmsg.Event = kMThreadMsg_GetGsList
	tmsg.RetChan = make(chan bool, 1)
	PostMThreadMsg(tmsg)

	<-tmsg.RetChan
	gsList, ok := tmsg.Data.([]UserGameServer)
	if !ok {
		return
	}
	rsp.Result = 0
	rsp.Servers = gsList
	bytes, err := json.Marshal(rsp)
	if err != nil {
		log.Println("Failed to marshal json.content:", gsList, "error:", err)
		return
	}

	w.Write(bytes)
}

func registergsHandler(w http.ResponseWriter, r *http.Request) {
	defer func() {
		exceptionDetails()
	}()

	addrArg := r.FormValue("address")

	rsp := &registergsRsp{}
	rsp.Result = -1

	if len(addrArg) == 0 {
		ipAndPortList := strings.Split(r.RemoteAddr, ":")
		if len(ipAndPortList) != 2 {
			rsp.Msg = "Invalid address arguments"
			data, err := json.Marshal(rsp)
			if err != nil {
				log.Println("Failed to marshal json cotent.error:", err, "content:", rsp)
				return
			}
			w.Write(data)
			return
		}

		addrArg = ipAndPortList[0]
	}

	portArg := r.FormValue("port")
	if len(portArg) == 0 {
		rsp.Msg = "Invalid port arguments"
		data, err := json.Marshal(rsp)
		if err != nil {
			log.Println("Failed to marshal json cotent.error:", err, "content:", rsp)
			return
		}
		w.Write(data)
		return
	}

	onlineArg := r.FormValue("online")
	noteArg := r.FormValue("note")
	passwordArg := r.FormValue("password")
	versionArg := r.FormValue("version")

	us := &UserGameServer{}
	us.Address = addrArg
	portInt, err := strconv.Atoi(portArg)
	if err != nil {
		log.Println("Invalid port argument.not number.error:", err)
		return
	}
	us.Port = portInt
	us.Note = noteArg
	us.Password = passwordArg
	us.Version = versionArg
	if len(onlineArg) != 0 {
		us.Online, _ = strconv.Atoi(onlineArg)
	}

	tmsg := &MThreadMsg{}
	tmsg.Event = kMThreadMsg_RegisterGS
	bytes, err := json.Marshal(us)
	if nil != err {
		log.Println("Failed to marshal json content.error:", err)
		return
	}
	tmsg.Msg = string(bytes)
	tmsg.RetChan = make(chan bool, 1)
	PostMThreadMsg(tmsg)

	ret := <-tmsg.RetChan

	if ret {
		rsp.Result = 0
		rsp.Msg = strconv.Itoa(tmsg.WParam)
		data, err := json.Marshal(rsp)
		if err != nil {
			log.Println("Failed to marshal json cotent.error:", err, "content:", rsp)
			return
		}
		w.Write(data)
	} else {
		rsp.Msg = "Failed to register gs"
		data, err := json.Marshal(rsp)
		if err != nil {
			log.Println("Failed to marshal json cotent.error:", err, "content:", rsp)
			return
		}
		w.Write(data)
	}
}
