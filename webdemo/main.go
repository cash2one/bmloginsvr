package main

import (
	"database/sql"
	"log"
	"net/http"
)

var g_DBUser *sql.DB

func main() {
	log.Println("main")

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
	http.HandleFunc("/", NotFoundHandler)
	http.ListenAndServe(":8888", nil)
}
