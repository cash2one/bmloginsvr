package main

//#include <stdlib.h>
import "C"

//	Go
import (
	"database/sql"
	"dbgutil"
	"fmt"
	"log"
	"os"
	"server"
	"syscall"
	//"unsafe"
)

var (
	g_ServerS    *server.Server
	g_ServerC    *server.Server
	g_UserList   *UserInfoList
	g_ServerList *UserInfoList
	g_CtrlCh     chan uint8
	g_DBUser     *sql.DB
)

var (
	g_dllHumSave syscall.DLL
	g_procMap    = make(map[string]*syscall.Proc)

	g_procDllName = []string{
		"CreateHumSave",
		"OpenHumSave",
		"CloseHumSave",
		"AddGameRole",
		"DelGameRole",
		"GetGameRoleInfo_Value",
		"GetGameRoleInfo_Name",
		"GetGameRoleIndex",
		"ReadGameRoleData",
		"WriteGameRoleData",
		"WriteGameRoleInfo",
	}
)

func initDllModule(name string) {
	g_dllHumSave, err := syscall.LoadDLL(name)
	if err != nil {
		log.Println("Can't load [", name, "]")
		return
	}
	//	Get all module
	for _, str := range g_procDllName {
		proc, err := g_dllHumSave.FindProc(str)
		if err == nil {
			g_procMap[str] = proc
			//log.Println("Proccess address[", str, "] loaded...")
			dbgutil.Display("ProcName", str, "ProcAddr", proc)
		} else {
			log.Println("ProcName[", str, "] load failed...", err)
		}
	}
}

func releaseDllModule() {
	if len(g_dllHumSave.Name) != 0 {
		for _, str := range g_procDllName {
			delete(g_procMap, str)
		}
		log.Println(g_dllHumSave.Name, " has been released...")
		g_dllHumSave.Release()
	}
}

func main() {
	//bmDllTest()
	defer func() {
		log.Println("Server terminated.")
		var input string
		fmt.Scanln(&input)
	}()
	//	Initialize the database
	//	Create database directory first
	if !PathExist("./db") {
		os.Mkdir("./db", os.ModeDir)
	}
	g_DBUser = initDatabase("./db/users.db")
	if nil == g_DBUser {
		log.Println("Initialize database failed.")
		return
	}
	//	for server
	handler := server.CreateDefaultServerHandler(50)
	g_ServerS = server.CreateWithConfig(nil)
	g_ServerS.EvtHandler = handler
	g_ServerList = &UserInfoList{
		allusers: make(map[uint32]IUserInterface),
	}

	//	for client
	chandler := server.CreateDefaultServerHandler(50)
	g_ServerC = server.CreateWithConfig(nil)
	g_ServerC.EvtHandler = chandler
	g_UserList = &UserInfoList{
		allusers: make(map[uint32]IUserInterface),
	}

	g_CtrlCh = make(chan uint8, 10)

	ch := make(chan string, 10)
	go go_handleInput(ch)

	server.InitSeed(0)
	ipaddrclient := "127.0.0.1:8300"
	ipaddrserver := "127.0.0.1:8200"
	if g_ServerS.StartListen(ipaddrserver) && g_ServerC.StartListen(ipaddrclient) {
		log.Println("Start process event.listen server:", ipaddrserver, " listen client:", ipaddrclient)

		for {
			select {
			case evt := <-handler.GetEventQueue():
				{
					ProcessServerSEvent(evt)
				}
			case evt := <-chandler.GetEventQueue():
				{
					ProcessServerCEvent(evt)
				}
			case input := <-ch:
				{
					ProcessInput(input)
				}
			case ctrl := <-g_CtrlCh:
				{
					if ctrl == 0 {
						break
					}
				}
			}
		}
	}

	log.Println("Quit process event...")
	close(g_CtrlCh)
	releaseDllModule()
}

func ProcessServerCEvent(evt *server.ConnEvent) {
	switch evt.Evtid {
	case server.CONNEVT_CONNECT:
		{
			HandleCConnect(evt)
		}
	case server.CONNEVT_READREADY:
		{
			HandleCMsg(evt)
		}
	case server.CONNEVT_DISCONNECT:
		{
			HandleCDisconnect(evt)
		}
	default:
		{
			log.Println("Unsolved ConnEvent[evtid:", evt.Evtid, "]")
		}
	}
}

func ProcessServerSEvent(evt *server.ConnEvent) {
	switch evt.Evtid {
	case server.CONNEVT_CONNECT:
		{
			HandleSConnect(evt)
		}
	case server.CONNEVT_READREADY:
		{
			HandleSMsg(evt)
		}
	case server.CONNEVT_DISCONNECT:
		{
			HandleSDisconnect(evt)
		}
	default:
		{
			log.Println("Unsolved ConnEvent[evtid:", evt.Evtid, "]")
		}
	}
}

func ProcessInput(input string) {
	log.Println("rec ", input)
	var cmd, param string = "", ""
	_, err := fmt.Sscanf(input, "%s_%s", &cmd, &param)
	if err != nil {
		log.Println("Parse user input error!Error[", err, "]")
		return
	}
	switch cmd {
	case "quit":
		{
			g_CtrlCh <- uint8(0)
		}
	}
	log.Println("Command: ", cmd, " parameters: ", param)
}

func go_handleInput(ch chan string) {
	log.Println("Goroutine [go_handleInput] start...")

	var (
		cmd string
	)

	for {
		_, err := fmt.Scanln(&cmd)
		if err != nil {
			log.Println("Receive user input failed...Error[", err, "]")
			break
		}
		ch <- cmd
	}

	log.Println("Goroutine [go_handleInput] quit...")
}
