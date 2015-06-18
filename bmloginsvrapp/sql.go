package main

import (
	"database/sql"
	"errors"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
	"strconv"
	"time"
)

func initDatabase(path string) *sql.DB {
	newdb := false
	if !PathExist(path) {
		file, err := os.Create(path)
		if err != nil {
			log.Println("Can't create db file.", err)
			return nil
		} else {
			newdb = true
			file.Close()
		}
	}

	//	Connect db
	db, err := sql.Open("sqlite3", path)
	if err != nil {
		log.Println("Can't open db file.", err)
		return nil
	}

	if newdb {
		sqlexpr := `
		create table useraccount(uid integer primary key, account varchar(20), password varchar(20), name0 varchar(20), name1 varchar(20), name2 varchar(20), online bool)
		`
		_, err = db.Exec(sqlexpr)
		if err != nil {
			log.Printf("Create new table failed.Error[%d] DB[%s]", err, db)
			db.Close()
			return nil
		}
	} else {
		//	reset all online state
	}

	//	check user donate table
	userDonateTableExist, err := dbTableExist(db, "userdonate")
	if err != nil {
		log.Printf("failed to check userDonate table.err:", err)
	} else {
		if !userDonateTableExist {
			sqlexpr := `
		create table userdonate(uid integer primary key, donate integer, lastdonatetime integer, expiretime integer)
		`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				log.Printf("Create new table failed.Error[%d] DB[%s]", err, db)
				db.Close()
				return nil
			} else {
				log.Printf("create a new table[userdonate]")
			}
		}
	}

	//	check user donate history table
	userDonateHistoryTableExists, err := dbTableExist(db, "userdonatehistory")
	if err != nil {
		log.Printf("failed to check userDonateHistory table.err:", err)
	} else {
		if !userDonateHistoryTableExists {
			sqlexpr := `
			create table userdonatehistory(id integer primary key, uid integer, donate integer, donatetime integer, donateorderid varchar(50))
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				log.Printf("Create new table failed.Error[%d] DB[%s]", err, db)
				db.Close()
				return nil
			} else {
				log.Printf("create a new table[userdonatehistory]")
			}
		}
	}

	//	check systemgift table
	systemGiftTableExists, err := dbTableExist(db, "systemgift")
	if err != nil {
		log.Printf("failed to check systemgift table.err:", err)
	} else {
		if !systemGiftTableExists {
			sqlexpr := `
			create table systemgift(id integer primary key, uid integer, giftid integer, giftsum integer, givetime integer, expiretime integer)
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				log.Printf("Create new table failed.Error[%d] DB[%s]", err, db)
				db.Close()
				return nil
			} else {
				log.Printf("create a new table[systemgift]")
			}
		}
	}

	return db
}

//	user donate table
func dbTableExist(db *sql.DB, tableName string) (bool, error) {
	if nil == db {
		return false, errors.New("nil database")
	}

	rows, err := db.Query("select count(*) as cnt from sqlite_master where type='table' and name='" + tableName + "'")
	if err != nil {
		return false, err
	}

	defer rows.Close()
	tableCount := 0

	if !rows.Next() {
		return false, errors.New("can't query table count")
	}

	rows.Scan(&tableCount)

	log.Println("table size:", tableCount, " table name:", tableName)

	if tableCount == 1 {
		return true, nil
	}

	return false, nil
}

type UserDonateInfo struct {
	uid            uint32
	donate         int32
	lastdonatetime int
	expiretime     int
}

func dbIsUserDonateExists(db *sql.DB, uid uint32) bool {
	rows, err := db.Query("select count(*) as cnt from userdonate where uid = '" + strconv.FormatUint(uint64(uid), 10) + "'")
	if err != nil {
		log.Printf("Error on selecting uid,error[%s]", err.Error())
		return true
	}

	defer rows.Close()
	if rows.Next() {
		count := 0
		rows.Scan(&count)

		if count == 0 {
			return false
		}
		return true
	}
	return false
}

func dbInsertUserDonateInfo(db *sql.DB, info *UserDonateInfo) bool {
	expr := "insert into userdonate values(" + strconv.FormatUint(uint64(info.uid), 10) + "," + strconv.FormatUint(uint64(info.donate), 10) + "," + strconv.FormatUint(uint64(info.lastdonatetime), 10) + "," + strconv.FormatUint(uint64(info.expiretime), 10) + ")"
	_, err := db.Exec(expr)
	if err != nil {
		log.Println("db exec failed.expr:", expr, " err:", err)
		return false
	}

	return true
}

func dbGetUserDonateInfo(db *sql.DB, uid uint32, info *UserDonateInfo) bool {
	if nil == db {
		return false
	}

	//	Select
	fetched := false
	sqlexpr := "select donate,lastdonatetime,expiretime from userdonate where uid = " + strconv.FormatUint(uint64(uid), 10)
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.donate, &info.lastdonatetime, &info.expiretime)
			info.uid = uid
			//log.Println("Fetched uid:", info.uid, " donate:", info.donate, " lastdonatetime:", info.lastdonatetime, " expiretime:", info.expiretime)
		}
	}

	return fetched
}

func dbIncUserDonateInfo(db *sql.DB, uid uint32, donateMoney int, donateOrderId string) bool {
	//	先查找订单号是否已被记录
	if dbIsUserDonateHistoryExists(db, donateOrderId) {
		log.Println("donate order id already been used")
		return false
	}

	//	添加记录
	history := &UserDonateHistory{}
	history.uid = uid
	history.donatetime = int(time.Now().Unix())
	history.donate = donateMoney
	history.donateorderid = donateOrderId
	if !dbInsertUserDonateHistory(db, history) {
		log.Println("failed to insert donate history ", history)
		return false
	}

	if !dbIsUserDonateExists(db, uid) {
		//	new record
		info := &UserDonateInfo{}
		info.uid = uid
		info.donate = int32(donateMoney)
		info.lastdonatetime = int(time.Now().Unix())
		info.expiretime = 0

		return dbInsertUserDonateInfo(db, info)
	} else {
		//	update record
		info := &UserDonateInfo{}
		if !dbGetUserDonateInfo(db, uid, info) {
			return false
		}
		info.donate = int32(dbGetUserDonateHistorySum(db, uid))
		info.lastdonatetime = int(time.Now().Unix())
		expr := "update userdonate set donate=" + strconv.FormatUint(uint64(info.donate), 10) + ", lastdonatetime=" + strconv.FormatUint(uint64(info.lastdonatetime), 10) + ", expiretime=" + strconv.FormatUint(uint64(info.expiretime), 10) + " where uid=" + strconv.FormatUint(uint64(uid), 10)

		_, err := db.Exec(expr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				expr, err.Error())
			return false
		}

		return true
	}
}

func dbUpdateUserDonateInfo(db *sql.DB, uid uint32, donateMoney int) bool {
	if !dbIsUserDonateExists(db, uid) {
		//	new record
		info := &UserDonateInfo{}
		info.uid = uid
		info.donate = int32(donateMoney)
		info.lastdonatetime = int(time.Now().Unix())
		info.expiretime = 0

		return dbInsertUserDonateInfo(db, info)
	} else {
		//	update record
		info := &UserDonateInfo{}
		if !dbGetUserDonateInfo(db, uid, info) {
			return false
		}
		info.donate = int32(donateMoney)
		info.lastdonatetime = int(time.Now().Unix())
		expr := "update userdonate set donate=" + strconv.FormatUint(uint64(info.donate), 10) + ", lastdonatetime=" + strconv.FormatUint(uint64(info.lastdonatetime), 10) + ", expiretime=" + strconv.FormatUint(uint64(info.expiretime), 10) + " where uid=" + strconv.FormatUint(uint64(uid), 10)

		_, err := db.Exec(expr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				expr, err.Error())
			return false
		}

		return true
	}
}

func dbRemoveUserDonateInfo(db *sql.DB, uid uint32) bool {
	if dbIsUserDonateExists(db, uid) {
		expr := "delete from userdonate where uid=" + strconv.FormatUint(uint64(uid), 10)
		_, err := db.Exec(expr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				expr, err.Error())
			return false
		}
		return true
	}

	return true
}

//	user donate history
type UserDonateHistory struct {
	uid           uint32
	donate        int
	donatetime    int
	donateorderid string
}

func dbIsUserDonateHistoryExists(db *sql.DB, donateOrderId string) bool {
	expr := "select count(*) as cnt from userdonatehistory where donateorderid='" + donateOrderId + "'"
	rows, err := db.Query(expr)

	if nil != err {
		log.Println("dbIsUserDonateHistoryExists err:", err)
		return true
	}

	defer rows.Close()

	count := 1

	if rows.Next() {
		rows.Scan(&count)
	}

	if 0 == count {
		return false
	}

	return true
}

func dbInsertUserDonateHistory(db *sql.DB, history *UserDonateHistory) bool {
	expr := "insert into userdonatehistory values(null, " + strconv.FormatUint(uint64(history.uid), 10) + "," + strconv.Itoa(history.donate) + "," + strconv.Itoa(history.donatetime) + "," + "'" + history.donateorderid + "'" + ")"
	_, err := db.Exec(expr)
	if err != nil {
		log.Println("db exec failed.expr:", expr, " err:", err)
		return false
	}

	return true
}

func dbGetUserDonateHistorySum(db *sql.DB, uid uint32) int {
	expr := "select sum(donate) from userdonatehistory where uid=" + strconv.FormatUint(uint64(uid), 10)
	sum := 0

	rows, err := db.Query(expr)
	if nil != err {
		log.Println("db query failed.expr:", expr, " err:", err)
		return 0
	}

	defer rows.Close()

	if rows.Next() {
		rows.Scan(&sum)
	}

	return sum
}

//	user account table
type UserAccountInfo struct {
	account  string
	password string
	online   bool
	uid      uint32
	name0    string
	name1    string
	name2    string
}

func dbGetUserAccountInfo(db *sql.DB, account string, info *UserAccountInfo) (bool, error) {
	if nil == db {
		return false, errors.New("nil database")
	}
	if len(account) > 20 {
		return false, errors.New("too long account characters")
	}

	//	Select
	fetched := false
	sqlexpr := "select uid,password,name0,name1,name2,online from useraccount where account = '" + account + "'"
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false, errors.New("Select error." + err.Error())
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.uid, &info.password, &info.name0, &info.name1, &info.name2, &info.online)
			info.account = account
			//log.Println("Fetched uid:", info.uid, " password:", info.password, " online:", info.online)
		}
	}

	return fetched, nil
}

func dbGetUserAccountInfoByUID(db *sql.DB, uid uint32, info *UserAccountInfo) bool {
	if nil == db {
		return false
	}

	//	Select
	fetched := false
	sqlexpr := "select account,password,name0,name1,name2,online from useraccount where uid = " + strconv.FormatUint(uint64(uid), 10)
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.account, &info.password, &info.name0, &info.name1, &info.name2, &info.online)
			info.uid = uid
			log.Println("Fetched uid:", info.uid, " password:", info.password, " online:", info.online, " name0:", info.name0, " name1:", info.name1, " name2:", info.name2)
		}
	}

	return fetched
}

func dbInsertUserAccountInfo(db *sql.DB, users []UserAccountInfo) bool {
	queuesize := len(users)
	if queuesize == 0 {
		return true
	}

	uniquequeue := make([]bool, queuesize)
	for i, v := range users {
		if dbUserAccountExist(db, v.account) {
			uniquequeue[i] = true
		}
	}
	/*stmt, err := db.Prepare("insert into useraccount values(?,?,?,?)")
	if err != nil {
		log.Println("Insert failed.", err)
		return false
	}
	defer stmt.Close()*/
	for i, v := range users {
		if uniquequeue[i] {
			continue
		}
		if len(v.account) > 19 || len(v.password) > 19 {
			continue
		}
		/*_, err := stmt.Exec(0, v.account, v.password, 0)
		if err != nil {
			log.Printf("Error on inserting Error[%s]", err.Error())
			return false
		}*/
		sqlexpr := "insert into useraccount values(null, '" + v.account + "','" + v.password + "','','',''," + strconv.FormatInt(0, 10) + ")"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]", sqlexpr, err.Error())
		}
	}

	return true
}

func dbUserAccountExist(db *sql.DB, account string) bool {
	rows, err := db.Query("select uid from useraccount where account = '" + account + "'")
	if err != nil {
		log.Printf("Error on selecting uid,error[%s]", err.Error())
		return true
	}

	defer rows.Close()
	if rows.Next() {
		var uid uint32
		rows.Scan(&uid)
		return true
	}
	return false
}

func dbRemoveUserAccountInfo(db *sql.DB, account string) bool {
	sqlexpr := "delete from useraccount where account = '" + account + "'"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}
	return true
}

func dbUpdateUserAccountState(db *sql.DB, account string, online bool) bool {
	var boolvalue int = 0
	if online {
		boolvalue = 1
	}

	sqlexpr := "update useraccount set online = " + strconv.FormatInt(int64(boolvalue), 10) + " where account = '" + account + "'"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}

	return true
}

func dbUpdateUserAccountPassword(db *sql.DB, account string, password string) bool {
	sqlexpr := "update useraccount set password = '" + password + "' where account = '" + account + "'"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}

	return true
}

func dbResetUserAccountOnlineState(db *sql.DB) bool {
	sqlexpr := "update useraccount set online = 0"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}

	return true
}

func dbUserNameExist(db *sql.DB, name string) bool {
	sqlexpr := "select account from useraccount where name0 ='" + name + "' or name1 ='" + name + "' or name2 ='" + name + "'"
	rows, err := db.Query(sqlexpr)

	if err != nil {
		log.Println(err)
		return true
	} else {
		defer rows.Close()
		if rows.Next() {
			return true
		}
	}

	return false
}

func dbAddUserName(db *sql.DB, account string, name string) bool {
	var info UserAccountInfo
	ret, _ := dbGetUserAccountInfo(db, account, &info)
	if !ret {
		return false
	}

	sameName := dbUserNameExist(db, name)
	if sameName {
		return false
	}

	if len(info.name0) == 0 {
		sqlexpr := "update useraccount set name0 = '" + name + "' where account='" + account + "'"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				sqlexpr, err.Error())
			return false
		}
	} else if len(info.name1) == 0 {
		sqlexpr := "update useraccount set name1 = '" + name + "' where account='" + account + "'"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				sqlexpr, err.Error())
			return false
		}
	} else if len(info.name2) == 0 {
		sqlexpr := "update useraccount set name2 = '" + name + "' where account='" + account + "'"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				sqlexpr, err.Error())
			return false
		}
	}

	return true
}

func dbRemoveUserName(db *sql.DB, account string, name string) bool {
	var info UserAccountInfo
	ret, _ := dbGetUserAccountInfo(db, account, &info)
	if !ret {
		return false
	}

	nameindex := int(-1)
	if info.name0 == name {
		nameindex = 0
	} else if info.name1 == name {
		nameindex = 1
	} else if info.name2 == name {
		nameindex = 2
	}

	if nameindex == -1 {
		return false
	}

	sqlexpr := "update useraccount set name" + strconv.FormatInt(int64(nameindex), 10) + " = '' where account='" + account + "'"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}
	return true
}

func dbGetUserUidByName(db *sql.DB, name string) uint32 {
	expr := "select uid from useraccount where name0='" + name + "' or name1='" + name + "' or name2='" + name + "'"
	rows, err := db.Query(expr)

	if err != nil {
		log.Println(err)
		return 0
	}

	defer rows.Close()

	uid := uint32(0)

	if rows.Next() {
		rows.Scan(&uid)
	}

	return uid
}

func dbGetUserUidByAccount(db *sql.DB, account string) uint32 {
	expr := "select uid from useraccount where account='" + account + "'"
	rows, err := db.Query(expr)

	if err != nil {
		log.Println(err)
		return 0
	}

	defer rows.Close()

	uid := uint32(0)

	if rows.Next() {
		rows.Scan(&uid)
	}

	return uid
}

//	system gift
type SystemGift struct {
	uid        uint32
	giftid     int
	giftsum    int
	givetime   int64
	expiretime int64
}

func dbInsertSystemGift(db *sql.DB, gift *SystemGift) bool {
	count := dbGetSystemGiftCountByUid(db, gift.uid, gift.giftid)
	if count != 0 {
		return false
	}
	
	expr := "insert into systemgift values(null, " +
		strconv.FormatUint(uint64(gift.uid), 10) + "," +
		strconv.FormatUint(uint64(gift.giftid), 10) + "," +
		strconv.FormatUint(uint64(gift.giftsum), 10) + "," +
		strconv.FormatUint(uint64(gift.givetime), 10) + "," +
		strconv.FormatUint(uint64(gift.expiretime), 10) +
		")"

	_, err := db.Exec(expr)
	if err != nil {
		log.Println("db exec error, expr:", expr, "err:", err)
		return false
	}

	return true
}

func dbGetSystemGiftByUid(db *sql.DB, uid uint32, gift *SystemGift) bool {
	expr := "select giftid,giftsum,givetime,expiretime from systemgift where uid=" + strconv.FormatUint(uint64(uid), 10)
	rows, err := db.Query(expr)

	if err != nil {
		log.Println("db query expr", expr, "err:", err)
		return false
	}

	defer rows.Close()

	if rows.Next() {
		gift.uid = uid
		rows.Scan(&gift.giftid)
		rows.Scan(&gift.giftsum)
		rows.Scan(&gift.givetime)
		rows.Scan(&gift.expiretime)
	}

	return true
}

func dbGetSystemGiftCountByUid(db *sql.DB, uid uint32, itemid int) int {
	expr := "select count(*) as cnt from systemgift where uid=" + 
	strconv.FormatUint(uint64(uid), 10) +
	" and giftid=" +
	strconv.FormatUint(uint64(itemid), 10)
	rowsCount, err := db.Query(expr)

	if err != nil {
		log.Println("sql expr ", expr, " error:", err)
		return 0
	}

	count := 0
	defer rowsCount.Close()

	if rowsCount.Next() {
		rowsCount.Scan(&count)
	}
	
	return count
}

func dbGetSystemGiftIdByUid(db *sql.DB, uid uint32) []int {
	expr := "select count(*) as cnt from systemgift where uid=" + strconv.FormatUint(uint64(uid), 10)
	rowsCount, err := db.Query(expr)

	if err != nil {
		log.Println("sql expr ", expr, " error:", err)
		return nil
	}

	count := 0
	defer rowsCount.Close()

	if rowsCount.Next() {
		rowsCount.Scan(&count)
	}

	if 0 == count {
		return nil
	}

	giftsArray := make([]int, count, count)

	expr = "select giftid from systemgift where uid=" + strconv.FormatUint(uint64(uid), 10)
	rowsRet, err := db.Query(expr)

	if err != nil {
		log.Println("sql expr ", expr, " error:", err)
		return nil
	}

	defer rowsRet.Close()

	index := 0
	for rowsRet.Next() {
		giftId := 0
		rowsRet.Scan(&giftId)
		giftsArray[index] = giftId
		index++
	}

	return giftsArray
}
