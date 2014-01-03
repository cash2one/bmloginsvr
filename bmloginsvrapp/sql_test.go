package main

import (
	//"database/sql"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
	"testing"
)

func TestSql(t *testing.T) {
	testdbdir := "./dbtest"
	if !PathExist(testdbdir) {
		os.Mkdir(testdbdir, os.ModeDir)
	}

	//	initialize
	db := initDatabase("./dbtest/user.db")
	if db == nil {
		t.Fatal("Can't initialize database.")
	}
	defer db.Close()

	//	select
	var userinfo UserAccountInfo
	_, err := dbGetUserAccountInfo(db, "tk2222", &userinfo)
	if err != nil {
		log.Println("can't select the row of ", "tk2222")
	} else {
		log.Println("uid", userinfo.uid, "accout:", userinfo.account, " password:", userinfo.password, " online:", userinfo.online)
	}

	//	insert
	userlist := make([]UserAccountInfo, 4)
	userlist[0].account = "tk1111"
	userlist[0].password = "3j98fa"
	userlist[1].account = "tk3333"
	userlist[1].password = "vcxz04s"
	userlist[2].account = "tk4444"
	userlist[2].password = "04bv8a"
	userlist[3].account = "tk5555"
	userlist[3].password = "y0dt4"
	if !dbInsertUserAccountInfo(db, userlist) {
		log.Println("Can't insert users")
	} else {
		log.Println("Insert users ok")
	}

	//	exist
	if !dbUserAccountExist(db, "tk3333") {
		t.Fatal("Can't find user.")
	}

	//	update
	if !dbUpdateUserAccountState(db, "tk4444", true) {
		t.Fatal("Can't update user state")
	}

	//	select
	_, err = dbGetUserAccountInfo(db, "tk4444", &userinfo)
	if err != nil {
		log.Println("can't select the row of ", "tk2222")
	} else {
		log.Println("uid", userinfo.uid, "accout:", userinfo.account, " password:", userinfo.password, " online:", userinfo.online)
	}

	//	remove
	if !dbRemoveUserAccountInfo(db, "tk5555") {
		t.Fatal("Can't remove user info")
	}

	//	reset state
	if !dbResetUserAccountOnlineState(db) {
		t.Fatal("Can't reset user state")
	}
	_, err = dbGetUserAccountInfo(db, "tk4444", &userinfo)
	if err != nil {
		log.Println("can't select the row of ", "tk2222")
	} else {
		log.Println("uid", userinfo.uid, "accout:", userinfo.account, " password:", userinfo.password, " online:", userinfo.online)
	}
}
