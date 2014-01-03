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
		create table useraccount(uid integer primary key, account varchar(20), password varchar(20),online bool)
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
			rows.Scan(&info.uid, &info.password, &info.online)
			info.account = account
			//log.Println("Fetched uid:", info.uid, " password:", info.password, " online:", info.online)
		}
	}

	return fetched, nil
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
