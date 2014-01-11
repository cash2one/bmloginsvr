package main

import (
	"database/sql"
	"errors"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
	"strconv"
)

type UserAccountInfo struct {
	account  string
	password string
	online   bool
	uid      uint32
	name0    string
	name1    string
	name2    string
}

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

	return db
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
	sqlexpr := "select uid,password,online from useraccount where account = '" + account + "'"
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false, errors.New("Select error." + err.Error())
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.uid, &info.password, &info.online, &info.name0, &info.name1, &info.name2)
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
	sqlexpr := "select account,password,online,name0,name1,name2 from useraccount where account = '" + strconv.FormatUint(uint64(uid), 10) + "'"
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.account, &info.password, &info.online, &info.name0, &info.name1, &info.name2)
			info.uid = uid
			//log.Println("Fetched uid:", info.uid, " password:", info.password, " online:", info.online)
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
		sqlexpr := "insert into useraccount values(null, '" + v.account + "','" + v.password + "'," + strconv.FormatInt(0, 10) + ")"
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
	sqlexpr := "select account from useraccount where name0 ='" + name + "' or where name1 ='" + name + "' or name2 ='" + name + "'"
	rows, err := db.Query(sqlexpr)

	if err != nil {
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
		sqlexpr := "update useraccount set name0 = " + name + "'"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				sqlexpr, err.Error())
			return false
		}
	} else if len(info.name1) == 0 {
		sqlexpr := "update useraccount set name1 = " + name + "'"
		_, err := db.Exec(sqlexpr)
		if err != nil {
			log.Printf("Error on executing expression[%s] Error[%s]",
				sqlexpr, err.Error())
			return false
		}
	} else if len(info.name2) == 0 {
		sqlexpr := "update useraccount set name2 = " + name + "'"
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

	sqlexpr := "delete from useraccount where name" + strconv.FormatInt(int64(nameindex), 10) + " = '" + name + "'"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}
	return true
}
