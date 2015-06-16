package main

import (
	//"database/sql"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
	"testing"
)

func TestSqlUserAccount(t *testing.T) {
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

	//	update password
	if !dbUpdateUserAccountPassword(db, "tk4444", "hello") {
		t.Fatal("Can't update user password")
	}

	//	set name
	if !dbAddUserName(db, "tk4444", "name1") {
		t.Fatal("Can't add user name")
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

	//	get uid by name
	uid := dbGetUserUidByName(db, "name1")
	if uid != 3 {
		t.Fatal("can't find uid by name")
	}
}

func TestSqlUserDonate(t *testing.T) {
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

	donateMoney := 100
	testUid := uint32(111)

	//	inc
	if !dbIncUserDonateInfo(db, testUid, donateMoney, "111248444") {
		t.Error("dbIncUserDonateInfo failed")
		return
	}

	//	inc twice
	if dbIncUserDonateInfo(db, testUid, donateMoney, "111248444") {
		t.Error("dbIncUserDonateInfo failed")
		return
	}

	//	get
	info := &UserDonateInfo{}
	if !dbGetUserDonateInfo(db, testUid, info) {
		t.Error("dbUpdateUserDonateInfo failed")
		return
	}

	if info.donate != donateMoney {
		t.Error("dbIncUserDonateInfo failed")
		return
	}

	//	exist
	if !dbIsUserDonateExists(db, testUid) {
		t.Error("dbIsUserDonateExists failed")
		return
	}

	//	remove
	if !dbRemoveUserDonateInfo(db, testUid) {
		t.Error("dbRemoveUserDonateInfo failed")
		return
	}

	//	exist
	if dbIsUserDonateExists(db, testUid) {
		t.Error("dbIsUserDonateExists failed")
		return
	}
}
