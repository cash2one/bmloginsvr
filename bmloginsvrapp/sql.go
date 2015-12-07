package main

import (
	"database/sql"
	"errors"
	"github.com/axgle/mahonia"
	_ "github.com/mattn/go-sqlite3"
	//"log"
	"os"
	"shareutils"
	"strconv"
	"time"
)

func initDatabaseUser(path string) *sql.DB {
	newdb := false
	if !PathExist(path) {
		file, err := os.Create(path)
		if err != nil {
			shareutils.LogErrorln("Can't create db file.", err)
			return nil
		} else {
			newdb = true
			file.Close()
		}
	}

	//	Connect db
	db, err := sql.Open("sqlite3", path)
	if err != nil {
		shareutils.LogErrorln("Can't open db file.", err)
		return nil
	}

	if newdb {
		sqlexpr := `
		create table useraccount(uid integer primary key, account varchar(20), password varchar(20), name0 varchar(20), name1 varchar(20), name2 varchar(20), online bool)
		`
		_, err = db.Exec(sqlexpr)
		if err != nil {
			shareutils.LogErrorln("Create new table failed.Error:", err)
			db.Close()
			return nil
		}
	} else {
		//	reset all online state
	}

	//	check user donate table
	userDonateTableExist, err := dbTableExist(db, "userdonate")
	if err != nil {
		shareutils.LogErrorln("failed to check userDonate table.err:", err)
	} else {
		if !userDonateTableExist {
			sqlexpr := `
		create table userdonate(uid integer primary key, donate integer, lastdonatetime integer, expiretime integer)
		`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				shareutils.LogErrorln("Create new table failed.Error:", err)
				db.Close()
				return nil
			} else {
				shareutils.LogInfoln("create a new table[userdonate]")
			}
		}
	}

	//	check user donate history table
	userDonateHistoryTableExists, err := dbTableExist(db, "userdonatehistory")
	if err != nil {
		shareutils.LogErrorln("failed to check userDonateHistory table.err:", err)
	} else {
		if !userDonateHistoryTableExists {
			sqlexpr := `
			create table userdonatehistory(id integer primary key, uid integer, donate integer, donatetime integer, donateorderid varchar(50))
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				shareutils.LogErrorln("Create new table failed.Error:", err)
				db.Close()
				return nil
			} else {
				shareutils.LogInfoln("create a new table[userdonatehistory]")
			}
		}
	}

	//	check systemgift table
	systemGiftTableExists, err := dbTableExist(db, "systemgift")
	if err != nil {
		shareutils.LogErrorln("failed to check systemgift table.err:", err)
	} else {
		if !systemGiftTableExists {
			sqlexpr := `
			create table systemgift(id integer primary key, uid integer, giftid integer, giftsum integer, givetime integer, expiretime integer)
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				shareutils.LogErrorln("Create new table failed.Error:", err)
				db.Close()
				return nil
			} else {
				shareutils.LogInfoln("create a new table[systemgift]")
			}
		}
	}

	//	check user donate consume
	userDonateConsumeExists, err := dbTableExist(db, "userdonateconsume")
	if err != nil {
		shareutils.LogErrorln("failed to check userdonateconsume table.err:", err)
	} else {
		if !userDonateConsumeExists {
			sqlexpr := `
			create table userdonateconsume(id integer primary key, uid integer, name varchar(21), itemid integer, cost integer, buytime integer)
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				shareutils.LogErrorln("Failed to create new table,err:", err)
				db.Close()
				return nil
			} else {
				shareutils.LogInfoln("Create new table[userdonateconsume]")
			}
		}
	}

	//	check player rank
	playerRankExists, err := dbTableExist(db, "player_rank")
	if err != nil {
		shareutils.LogErrorln("failed to check player_rank table.err:", err)
	} else {
		if !playerRankExists {
			sqlexpr := `
			create table player_rank(id integer primary key, uid integer, name varchar(21), job integer, level integer, expr integer, power integer)
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				shareutils.LogErrorln("Failed to create new table,err:", err)
				db.Close()
				return nil
			} else {
				shareutils.LogInfoln("Create new table[player_rank]")
			}
		}
	}

	//	check admin_account
	adminAccountExists, err := dbTableExist(db, "admin_account")
	if err != nil {
		shareutils.LogErrorln("failed to check admin_account table.err:", err)
	} else {
		if !adminAccountExists {
			sqlexpr := `
			create table admin_account(id integer primary key, account varchar(20), level integer)
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				shareutils.LogErrorln("Failed to create new table,err:", err)
				db.Close()
				return nil
			} else {
				shareutils.LogInfoln("Create new table[admin_account]")
			}
		}
	}

	//	check donate_request
	donateRequestExists, err := dbTableExist(db, "donate_request")
	if err != nil {
		shareutils.LogErrorln("failed to check donate_request table.err:", err)
	} else {
		if !donateRequestExists {
			sqlexpr := `
			create table donate_request(id integer primary key, name varchar(20), uid integer, orderid varchar(50), money integer, note varchar(40), result integer)
			`

			_, err = db.Exec(sqlexpr)
			if err != nil {
				shareutils.LogErrorln("Failed to create new table,err:", err)
				db.Close()
				return nil
			} else {
				shareutils.LogInfoln("Create new table[donate_request]")
			}
		}
	}

	return db
}

//	donate_request
type DonateRequest struct {
	Id      int
	Name    string
	Uid     int
	OrderId string
	Money   int
	Note    string
	Result  int
}

const (
	kDonateResult_None = iota
	kDonateResult_Ok
	kDonateResult_Invalid
)

func dbDonateRequestExists(db *sql.DB, orderId string) bool {
	rows, err := db.Query("select count(*) as cnt from donate_request where orderid = '" + orderId + "'")
	if err != nil {
		shareutils.LogErrorf("Error on selecting uid,error[%s]", err.Error())
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

func dbInsertDonateRequest(db *sql.DB, dr *DonateRequest) bool {
	if nil == dr {
		return false
	}

	if len(dr.Name) == 0 ||
		len(dr.OrderId) == 0 ||
		0 == dr.Uid {
		return false
	}
	if dbDonateRequestExists(db, dr.OrderId) {
		return true
	}
	expr := "insert into donate_request values(null, '" + dr.Name + "'," + strconv.Itoa(dr.Uid) + ",'" + dr.OrderId + "'," + strconv.Itoa(dr.Money) + ",'" + dr.Note + "'," + strconv.Itoa(dr.Result) + ")"
	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorln("db exec failed.expr:", expr, " err:", err)
		return false
	}

	return true
}

func dbUpdateDonateRequestResult(db *sql.DB, orderId string, result int) bool {
	expr := "update donate_request set result=" + strconv.Itoa(result) + " where orderid='" + orderId + "'"

	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return false
	}

	return true
}

func dbUpdateDonateRequestMoney(db *sql.DB, orderId string, money int) bool {
	expr := "update donate_request set money=" + strconv.Itoa(money) + " where orderid='" + orderId + "'"

	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return false
	}

	return true
}

func dbRemoveDonateRequest(db *sql.DB, orderId string) bool {
	if !dbDonateRequestExists(db, orderId) {
		return true
	}

	expr := "delete from donate_request where orderid='" + orderId + "'"
	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return false
	}

	return true
}

//	admin_account
type AdminAccountInfo struct {
	account string
	level   int
}

func dbAdminAccountVerify(db *sql.DB, account string, password string) (bool, int) {
	if !dbAdminAccountExists(db, account) {
		return false, 0
	}

	var adminAccountInfo AdminAccountInfo
	if !dbGetAdminAccountInfo(db, account, &adminAccountInfo) {
		return false, 0
	}

	var userAccountInfo UserAccountInfo
	ok, _ := dbGetUserAccountInfo(db, account, &userAccountInfo)
	if !ok {
		return false, 0
	}

	if userAccountInfo.password == password {
		return true, adminAccountInfo.level
	}

	return false, 0
}

func dbInsertAdminAccount(db *sql.DB, account string, level int) bool {
	if level <= 0 {
		return false
	}

	if dbAdminAccountExists(db, account) {
		return true
	}

	var ainfo UserAccountInfo
	ok, _ := dbGetUserAccountInfo(db, account, &ainfo)
	if !ok {
		shareutils.LogErrorln("Unexists account, opeartion failed.")
		return false
	}

	expr := "insert into admin_account values(null, '" + account + "'," + strconv.Itoa(level) + ")"
	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorln("db exec failed.expr:", expr, " err:", err)
		return false
	}

	return true
}

func dbGetAdminAccountInfo(db *sql.DB, account string, info *AdminAccountInfo) bool {
	if nil == db {
		return false
	}
	if len(account) > 20 {
		return false
	}

	//	Select
	fetched := false
	sqlexpr := "select level from admin_account where account = '" + account + "'"
	rows, err := db.Query(sqlexpr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.level)
			info.account = account
		}
	}

	return fetched
}

func dbAdminAccountExists(db *sql.DB, account string) bool {
	rows, err := db.Query("select count(*) as cnt from admin_account where account = '" + account + "'")
	if err != nil {
		shareutils.LogErrorf("Error on selecting uid,error[%s]", err.Error())
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

func dbUpdateAdminAccount(db *sql.DB, account string, level int) bool {
	expr := "update admin_account set level=" + strconv.Itoa(level) + " where account='" + account + "'"

	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return false
	}

	return true
}

func dbRemoveAdminAccount(db *sql.DB, account string) bool {
	if !dbAdminAccountExists(db, account) {
		return true
	}

	expr := "delete from admin_account where account='" + account + "'"
	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return false
	}

	return true
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

	shareutils.LogInfoln("table size:", tableCount, " table name:", tableName)

	if tableCount == 1 {
		return true, nil
	}

	return false, nil
}

type UserRankInfo struct {
	Uid   uint32 `json:"uid"`
	Name  string `json:"name"`
	Job   int    `json:"job"`
	Level int    `json:"level"`
	Expr  int    `json:"expr"`
	Power int    `json:"power"`
}

func dbIsUserRankExists(db *sql.DB, name string) bool {
	rows, err := db.Query("select count(*) as cnt from player_rank where name = '" + name + "'")
	if err != nil {
		shareutils.LogErrorf("Error on selecting uid,error[%s]", err.Error())
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

func dbRemoveUserRankInfo(db *sql.DB, name string) bool {
	if !dbIsUserRankExists(db, name) {
		return true
	}

	expr := "delete from player_rank where name='" + name + "'"
	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return false
	}

	return true
}

func dbGetUserRankInfo(db *sql.DB, uid uint32, info *UserRankInfo) bool {
	if nil == db {
		return false
	}

	//	Select
	fetched := false
	sqlexpr := "select name,job,level,expr,power from player_rank where uid = " + strconv.Itoa(int(uid))
	rows, err := db.Query(sqlexpr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.Name, &info.Job, &info.Level, &info.Expr, &info.Power)
			info.Uid = uid
		}
	}

	return fetched
}

func dbUpdateUserRankInfo(db *sql.DB, info *UserRankInfo) bool {
	if nil == db {
		return false
	}

	if !dbIsUserRankExists(db, info.Name) {
		//	new record
		expr := "insert into player_rank values(null, " + strconv.FormatUint(uint64(info.Uid), 10) + ",'" + info.Name + "'," + strconv.Itoa(int(info.Job)) + "," + strconv.Itoa(int(info.Level)) + "," + strconv.FormatUint(uint64(info.Expr), 10) + "," + strconv.FormatUint(uint64(info.Power), 10) + ")"
		_, err := db.Exec(expr)
		if err != nil {
			shareutils.LogErrorln("db exec failed.expr:", expr, " err:", err)
			return false
		}

		return true
	} else {
		//	update record
		expr := "update player_rank set "
		exprInitialLength := len(expr)

		if 0 == info.Level &&
			0 == info.Expr &&
			0 == info.Power {
			return true
		}

		if 0 != info.Level {
			expr += " level=" + strconv.Itoa(info.Level)
		}
		if 0 != info.Expr {
			if len(expr) != exprInitialLength {
				expr += ","
			}
			expr += " expr=" + strconv.Itoa(info.Expr)
		}
		if 0 != info.Power {
			if len(expr) != exprInitialLength {
				expr += ","
			}
			expr += " power=" + strconv.Itoa(info.Power)
		}
		expr += " where name='" + info.Name + "'"

		_, err := db.Exec(expr)
		if err != nil {
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
				expr, err.Error())
			return false
		}

		return true
	}
}

func dbGetUserRankInfoOrderByPower(db *sql.DB, limit int, job int) []UserRankInfo {
	var ret []UserRankInfo = nil

	expr := "select uid, name, job, level, expr, power from player_rank "
	if job >= 0 &&
		job <= 2 {
		expr += " where job=" + strconv.Itoa(job)
	}

	expr += " order by power desc, level desc limit " + strconv.Itoa(limit)
	rows, err := db.Query(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return nil
	}

	defer rows.Close()
	ret = make([]UserRankInfo, limit)
	index := 0
	//	Read data
	for rows.Next() {
		rows.Scan(&ret[index].Uid, &ret[index].Name, &ret[index].Job, &ret[index].Level, &ret[index].Expr, &ret[index].Power)
		//	gbk转utf8
		decoder := mahonia.NewDecoder("gb18030")
		u8Str := decoder.ConvertString(ret[index].Name)
		ret[index].Name = u8Str
		index++
	}

	if index == 0 {
		return nil
	}

	return ret[0:index]
}

func dbGetUserRankInfoOrderByLevel(db *sql.DB, limit int, job int) []UserRankInfo {
	var ret []UserRankInfo = nil

	expr := "select uid, name, job, level, expr, power from player_rank "
	if job >= 0 &&
		job <= 2 {
		expr += " where job=" + strconv.Itoa(job)
	}

	expr += " order by level desc, power desc limit " + strconv.Itoa(limit)
	rows, err := db.Query(expr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			expr, err.Error())
		return nil
	}

	defer rows.Close()
	ret = make([]UserRankInfo, limit)
	index := 0
	//	Read data
	for rows.Next() {
		rows.Scan(&ret[index].Uid, &ret[index].Name, &ret[index].Job, &ret[index].Level, &ret[index].Expr, &ret[index].Power)
		//	gbk转utf8
		decoder := mahonia.NewDecoder("gb18030")
		u8Str := decoder.ConvertString(ret[index].Name)
		ret[index].Name = u8Str
		index++
	}

	if index == 0 {
		return nil
	}

	return ret[:index]
}

type UserDonateConsume struct {
	uid     uint32
	name    string
	itemid  int
	cost    int
	buytime int64
}

func dbInsertUserDonateConsume(db *sql.DB, uid uint32, name string, itemid int, cost int) bool {
	expr := "insert into userdonateconsume values(null, " + strconv.FormatUint(uint64(uid), 10) + ",'" + name + "' ," + strconv.FormatUint(uint64(itemid), 10) + "," + strconv.FormatUint(uint64(cost), 10) + "," + strconv.FormatUint(uint64(time.Now().Unix()), 10) + ")"
	_, err := db.Exec(expr)
	if err != nil {
		shareutils.LogErrorln("db exec failed.expr:", expr, " err:", err)
		return false
	}

	return true
}

func dbGetUserDonateConsumeSum(db *sql.DB, uid uint32) int {
	expr := "select sum(cost) from userdonateconsume where uid=" + strconv.FormatUint(uint64(uid), 10)
	sum := 0

	rows, err := db.Query(expr)
	if nil != err {
		shareutils.LogErrorln("db query failed.expr:", expr, " err:", err)
		return 0
	}

	defer rows.Close()

	if rows.Next() {
		rows.Scan(&sum)
	}

	return sum
}

func dbGetUserDonateLeft(db *sql.DB, uid uint32) int {
	var info UserDonateInfo
	if !dbGetUserDonateInfo(db, uid, &info) {
		return 0
	}

	used := dbGetUserDonateConsumeSum(db, uid)
	left := int(info.donate) - used
	if left >= 0 {
		return left
	}
	return 0
}

func dbCheckConsumeDonate(db *sql.DB, uid uint32, cost int) bool {
	if cost < 0 {
		return false
	}

	//	先检查是否有捐赠的金币
	donateInfo := &UserDonateInfo{}
	if !dbGetUserDonateInfo(db, uid, donateInfo) {
		return false
	}

	nCostMoney := dbGetUserDonateConsumeSum(db, uid)
	nLeftMoney := int(donateInfo.donate) - nCostMoney
	if nLeftMoney < cost {
		//	钱不够
		return false
	}

	return true
}

func dbOnConsumeDonate(db *sql.DB, uid uint32, name string, itemid int, cost int) (bool, int) {
	if cost < 0 {
		return false, -1
	}
	//	先检查是否有捐赠的金币
	donateInfo := &UserDonateInfo{}
	if !dbGetUserDonateInfo(db, uid, donateInfo) {
		return false, -2
	}

	nCostMoney := dbGetUserDonateConsumeSum(db, uid)
	nLeftMoney := int(donateInfo.donate) - nCostMoney
	if nLeftMoney < cost {
		//	钱不够
		return false, -3
	}

	//	钱够了
	if !dbInsertUserDonateConsume(db, uid, name, itemid, cost) {
		return false, -4
	}

	return true, nLeftMoney - cost
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
		shareutils.LogErrorf("Error on selecting uid,error[%s]", err.Error())
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
		shareutils.LogErrorln("db exec failed.expr:", expr, " err:", err)
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
		shareutils.LogErrorf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
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
		shareutils.LogErrorln("donate order id already been used")
		return false
	}

	//	添加记录
	history := &UserDonateHistory{}
	history.uid = uid
	history.donatetime = int(time.Now().Unix())
	history.donate = donateMoney
	history.donateorderid = donateOrderId
	if !dbInsertUserDonateHistory(db, history) {
		shareutils.LogErrorln("failed to insert donate history ", history)
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
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
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
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
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
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
				expr, err.Error())
			return false
		}

		//	remove all donate history
		expr = "delete from userdonatehistory where uid=" + strconv.FormatUint(uint64(uid), 10)
		_, err = db.Exec(expr)
		if err != nil {
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
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
		shareutils.LogErrorln("dbIsUserDonateHistoryExists err:", err)
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
		shareutils.LogErrorln("db exec failed.expr:", expr, " err:", err)
		return false
	}

	return true
}

func dbGetUserDonateHistorySum(db *sql.DB, uid uint32) int {
	expr := "select sum(donate) from userdonatehistory where uid=" + strconv.FormatUint(uint64(uid), 10)
	sum := 0

	rows, err := db.Query(expr)
	if nil != err {
		shareutils.LogErrorln("db query failed.expr:", expr, " err:", err)
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
		shareutils.LogErrorf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
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
		shareutils.LogErrorf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.account, &info.password, &info.name0, &info.name1, &info.name2, &info.online)
			info.uid = uid
			//shareutils.LogInfoln("Fetched uid:", info.uid, " password:", info.password, " online:", info.online, " name0:", info.name0, " name1:", info.name1, " name2:", info.name2)
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

	for i, v := range users {
		if uniquequeue[i] {
			continue
		}
		if len(v.account) > 19 || len(v.password) > 19 {
			continue
		}

		sqlexpr := "insert into useraccount values(null, '" + v.account + "','" + v.password + "','','',''," + strconv.FormatInt(0, 10) + ")"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]", sqlexpr, err.Error())
		}
	}

	return true
}

func dbUserAccountExist(db *sql.DB, account string) bool {
	rows, err := db.Query("select uid from useraccount where account = '" + account + "'")
	if err != nil {
		shareutils.LogErrorf("Error on selecting uid,error[%s]", err.Error())
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
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
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
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}

	return true
}

func dbUpdateUserAccountPassword(db *sql.DB, account string, password string) bool {
	sqlexpr := "update useraccount set password = '" + password + "' where account = '" + account + "'"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}

	return true
}

func dbResetUserAccountOnlineState(db *sql.DB) bool {
	sqlexpr := "update useraccount set online = 0"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}

	return true
}

func dbUserNameExist(db *sql.DB, name string) bool {
	sqlexpr := "select account from useraccount where name0 ='" + name + "' or name1 ='" + name + "' or name2 ='" + name + "'"
	rows, err := db.Query(sqlexpr)

	if err != nil {
		shareutils.LogErrorln(err)
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
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
				sqlexpr, err.Error())
			return false
		}
	} else if len(info.name1) == 0 {
		sqlexpr := "update useraccount set name1 = '" + name + "' where account='" + account + "'"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
				sqlexpr, err.Error())
			return false
		}
	} else if len(info.name2) == 0 {
		sqlexpr := "update useraccount set name2 = '" + name + "' where account='" + account + "'"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
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
		shareutils.LogErrorf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}
	return true
}

func dbGetUserUidByName(db *sql.DB, name string) uint32 {
	expr := "select uid from useraccount where name0='" + name + "' or name1='" + name + "' or name2='" + name + "'"
	rows, err := db.Query(expr)

	if err != nil {
		shareutils.LogErrorln(err)
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
		shareutils.LogErrorln(err)
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
		shareutils.LogErrorln("db exec error, expr:", expr, "err:", err)
		return false
	}

	return true
}

func dbGetSystemGiftByUid(db *sql.DB, uid uint32, gift *SystemGift) bool {
	expr := "select giftid,giftsum,givetime,expiretime from systemgift where uid=" + strconv.FormatUint(uint64(uid), 10)
	rows, err := db.Query(expr)

	if err != nil {
		shareutils.LogErrorln("db query expr", expr, "err:", err)
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
		shareutils.LogErrorln("sql expr ", expr, " error:", err)
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
		shareutils.LogErrorln("sql expr ", expr, " error:", err)
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
		shareutils.LogErrorln("sql expr ", expr, " error:", err)
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
