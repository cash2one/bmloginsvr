package main

import (
	"math/rand"
	"strconv"
	//"database/sql"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
	"testing"
	"time"
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

	if int(info.donate) != donateMoney {
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

	//	inc
	if !dbIncUserDonateInfo(db, testUid, donateMoney, "34252543") {
		t.Error("dbIncUserDonateInfo failed")
		return
	}

	//	constom
	consumeOk, leftMoney := dbOnConsumeDonate(db, testUid, 1, 50)
	if !consumeOk {
		t.Error("dbOnConsumeDonate failed:", leftMoney)
		return
	}
	consumeOk, leftMoney = dbOnConsumeDonate(db, testUid, 2, 30)
	if !consumeOk ||
		leftMoney != 20 {
		t.Error("dbOnConsumeDonate failed:", leftMoney)
		return
	}
	consumeOk, leftMoney = dbOnConsumeDonate(db, testUid, 3, 30)
	if consumeOk ||
		leftMoney != -3 {
		t.Error("dbOnConsumeDonate failed:", leftMoney)
		return
	}
	consumeOk, leftMoney = dbOnConsumeDonate(db, testUid, 3, 20)
	if !consumeOk ||
		leftMoney != 0 {
		t.Error("dbOnConsumeDonate failed:", leftMoney)
		return
	}

	//	player rank
	rankInfo := &UserRankInfo{}
	rankInfo.uid = 1
	rankInfo.job = 1
	rankInfo.level = 33
	rankInfo.name = "test"
	rankInfo.expr = 12333
	playerRankOk := dbUpdateUserRankInfo(db, rankInfo)
	if !playerRankOk {
		t.Error("dbUpdateUserRankInfo failed")
	}

	getRankInfo := &UserRankInfo{}
	playerRankOk = dbGetUserRankInfo(db, 1, getRankInfo)
	if !playerRankOk ||
		getRankInfo.uid != rankInfo.uid ||
		getRankInfo.job != rankInfo.job ||
		getRankInfo.level != rankInfo.level ||
		getRankInfo.expr != rankInfo.expr {
		t.Error("dbGetUserRankInfo failed")
	}

	rankInfo.level = 34
	rankInfo.power = 55
	rankInfo.expr = 0
	playerRankOk = dbUpdateUserRankInfo(db, rankInfo)
	if !playerRankOk {
		t.Error("dbUpdateUserRankInfo failed")
	}

	playerRankOk = dbGetUserRankInfo(db, 1, getRankInfo)
	if !playerRankOk ||
		getRankInfo.uid != 1 ||
		getRankInfo.job != 1 ||
		getRankInfo.level != 34 ||
		getRankInfo.expr != 12333 ||
		getRankInfo.power != 55 {
		t.Error("dbGetUserRankInfo failed")
	}

	randGenerator := rand.New(rand.NewSource(time.Now().UnixNano()))
	for uid := 1000; uid <= 1250; uid++ {
		rankInfo.uid = uint32(uid)
		rankInfo.job = randGenerator.Intn(3)
		rankInfo.expr = 10000 + randGenerator.Intn(5000)
		rankInfo.level = 20 + randGenerator.Intn(40)
		rankInfo.name = "tester_" + strconv.Itoa(uid)
		if !dbUpdateUserRankInfo(db, rankInfo) {
			t.Error("dbUpdateUserRankInfo failed")
		}
	}

	//	select by level
	rankList := dbGetUserRankInfoOrderByLevel(db, 10, -1)
	if nil != rankList {
		log.Println(rankList)
	} else {
		t.Error("dbGetUserRankInfoOrderByLevel failed")
	}
	rankList = dbGetUserRankInfoOrderByPower(db, 10, 1)
	if nil != rankList {
		log.Println(rankList)
	} else {
		t.Error("dbGetUserRankInfoOrderByPower failed")
	}
}
