package main

import (
	"client"
	"database/sql"
	"flag"
	"fmt"
	"log"
	"net/http"
)

//	global variables
var g_listenAddress string
var g_mailAccount string
var g_mailPassword string
var g_smtpAddress string
var g_lsAddress string
var g_lsHttpAddr string
var g_userLsAddress string
var g_usingHttpMode int

var g_Client *client.Client
var g_ChanEvent chan *goChanEvent
var g_DBUser *sql.DB

func init() {
	g_Client = client.CreateClientWithDefaultHandler()
	g_ChanEvent = make(chan *goChanEvent)
}

func main() {
	fmt.Println("bmregsvrapp started.")
	defer fmt.Println("bmregsvrapp terminated")

	g_DBUser = initDatabase("./login/req.db")
	if nil == g_DBUser {
		log.Println("Initialize database failed.")
		return
	}
	defer g_DBUser.Close()

	flag.StringVar(&g_listenAddress, "listenaddress", "", "listen address")
	flag.StringVar(&g_mailAccount, "mailaccount", "", "mail account")
	flag.StringVar(&g_mailPassword, "mailpassword", "", "mail password")
	flag.StringVar(&g_smtpAddress, "smtpaddr", "", "mail smtp server address")
	flag.StringVar(&g_lsAddress, "lsaddress", "", "login server address")
	flag.StringVar(&g_lsHttpAddr, "lshttpaddr", "0", "login server http address")
	flag.StringVar(&g_userLsAddress, "ulsaddress", "", "user login server address")
	flag.IntVar(&g_usingHttpMode, "usinghttpmode", 0, "using http mode")
	flag.Parse()

	//g_lsAddress = "localhost:6050"
	_, err := g_Client.Connect(g_lsAddress)
	if err != nil {
		log.Fatal("can't connect to ls ", err)
		return
	}

	log.Println("server ", g_lsAddress, " connected")

	go go_handleEvent()

	http.HandleFunc("/", httpRequestEntry)
	err = http.ListenAndServe(g_listenAddress, nil)
	if err != nil {
		log.Fatal("ListenAndServe error: ", err)
	}
}
