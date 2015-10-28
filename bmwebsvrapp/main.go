package main

import (
	"database/sql"
	"flag"
	"log"
	"net/http"
)

var g_DBUser *sql.DB
var g_RegServerAddr string = "http://localhost:8081"
var g_MailVerify string = "/mailverify"
var g_RegAccount string = "/regaccount"
var g_ModifyPassword string = "/modifypassword"
var g_InsertDonate string = "/insertdonaterecord"
var g_InsertGift string = "/insertsystemgift"

func main() {
	log.Println("main")

	//	get settings
	flag.StringVar(&g_RegServerAddr, "http://localhost:8081", "register server request address.exp:localhost:8888")
	flag.Parse()

	//	initialize sqlite...
	g_DBUser = initDatabase("./webusers.db")
	if nil == g_DBUser {
		log.Println("Initialize database failed.")
		return
	}
	defer g_DBUser.Close()

	http.Handle("/css/", http.FileServer(http.Dir("template")))
	http.Handle("/js/", http.FileServer(http.Dir("template")))

	http.HandleFunc("/admin/", adminHandler)
	http.HandleFunc("/login/", loginHandler)
	http.HandleFunc("/ajax/", ajaxHandler)
	http.HandleFunc("/register", registerHandler)
	http.HandleFunc("/", NotFoundHandler)
	http.ListenAndServe(":8888", nil)
}
