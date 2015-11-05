package main

//#include <stdlib.h>
import "C"

//	Go
import (
	"database/sql"
	//	"encoding/json"
	"flag"
	"fmt"
	"log"
	"os"
	"runtime/debug"
	"server"
	"shareutils"
	"time"
)

var (
	g_ServerS        *server.Server
	g_ServerC        *server.Server
	g_UserList       *UserInfoList
	g_ServerList     *UserInfoList
	g_CtrlCh         chan uint8
	g_DBUser         *sql.DB
	g_Redis          *RedisOperator
	g_AvaliableGS    uint32
	g_strVersionInfo string = "1.0.1"
	g_ControlAddr    []string
)

func exceptionDetails() {
	if err := recover(); err != nil {
		log.Println("Exception!error:", err, "stack:")
		debug.PrintStack()
	}
}

func main() {
	defer func() {
		log.Println("Server terminated.")
		exceptionDetails()
		var input string
		fmt.Scanln(&input)
	}()
	//log.SetFlags(log.Lshortfile | log.LstdFlags)

	g_ControlAddr = make([]string, 0, 10)
	ReadControlAddr("./login/gmlist.txt")

	//	Load config
	ipaddrclient := flag.String("lsaddr", "", "Listen clients")
	ipaddrserver := flag.String("lsgsaddr", "", "Listen gameserver")
	redisAddress := flag.String("redisaddr", "", "Redis address")
	httpAddr := flag.String("httpaddr", "", "http listen address")
	logConfig := flag.String("logconfig", "LogToFile:false_LogPrefix:LEVEL+FILELINE_LogPriority:DEBUG", "log config")
	flag.Parse()
	if len(*ipaddrclient) == 0 || len(*ipaddrserver) == 0 {
		log.Println("invalid input parameters.")
		flag.PrintDefaults()
		return
	}
	server.InitSeed(0)

	shareutils.DefaultLogHelper().Init("bmloginsvrapp", *logConfig)

	log.Println("BackMIR Login Server started.")
	//	Initialize directory
	if !PathExist("./login") {
		os.Mkdir("./login", os.ModeDir)
	}
	//	Set log file
	/*logfile, lferr := os.Create("./login/login.log")
	if lferr != nil {
		log.Println(lferr)
	} else {
		log.SetOutput(logfile)
	}*/
	//	Initialize dll module
	if !initDllModule("./login/BMHumSaveControl.dll") {
		log.Println("Can't load the save control module.")
		//return
	}
	//	Initialize the database
	g_DBUser = initDatabase("./login/users.db")
	if nil == g_DBUser {
		log.Println("Initialize database failed.")
		return
	}
	defer g_DBUser.Close()

	//	Initialize redis
	g_Redis = NewRedisOperator()
	if len(*redisAddress) != 0 {
		g_Redis.Run(*redisAddress, "bmevent")
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

	//	test
	/*extInfo := &UserLoginExtendInfo{}
	donateInfo := &UserDonateInfo{}
	if dbGetUserDonateInfo(g_DBUser, 1, donateInfo) {
		//	nothing
		log.Println("player[", 1, "] donate money:", donateInfo.donate)
	}
	extInfo.DonateMoney = donateInfo.donate
	extInfo.SystemGift = dbGetSystemGiftIdByUid(g_DBUser, 1)
	binaryExtInfo, jsErr := json.Marshal(extInfo)
	if jsErr != nil {
		log.Println("failed to marshal user extend login information:", jsErr)
	} else {
		//	发送扩展信息
		log.Println(string(binaryExtInfo))
	}*/
	//	test

	//	http server
	if httpAddr != nil &&
		len(*httpAddr) != 0 {
		startHttpServer(*httpAddr)
	}

	//	main thread message handler
	MainThreadInit()

	g_CtrlCh = make(chan uint8, 10)

	ch := make(chan string, 10)
	go go_handleInput(ch)

	timerTick := time.Tick(time.Duration(5) * time.Second)

	if g_ServerS.StartListen(*ipaddrserver) && g_ServerC.StartListen(*ipaddrclient) {
		log.Println("Start process event.listen server:", *ipaddrserver, " listen client:", *ipaddrclient)

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
			case evt := <-g_Redis.outputChan:
				{
					ProcessRedisEvent(evt)
				}
			case <-time.After(time.Duration(5) * time.Minute):
				{
					ReadControlAddr("./login/gmlist.txt")
				}
			case evt := <-g_chanMainThread:
				{
					ProcessMThreadMsg(evt)
				}
			case <-time.After(time.Duration(30) * time.Second):
				{
					UpdateMThreadMsg()
				}
			case <-timerTick:
				{
					UpdateTimerEvent()
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

func ProcessRedisEvent(evt *RedisEvent) {
	switch evt.CommandType {
	case RedisEvent_SavePlayerData:
		{
			OfflineSaveUserData(evt.BinaryData)
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
