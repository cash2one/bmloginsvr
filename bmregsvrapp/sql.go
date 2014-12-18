package main

import (
	"database/sql"
	"errors"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
	//	"strconv"
)

type UserRegKeyInfo struct {
	uid         int
	mailAddress string
	regKey      string
	account     string
}

func PathExist(_path string) bool {
	_, err := os.Stat(_path)
	if err != nil {
		if os.IsNotExist(err) {
			return false
		}
	}
	return true
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
		create table userregkey(uid integer primary key, mail varchar(50), regkey varchar(50), account varchar(20))
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

func dbGetUserRegKeyInfoByMail(db *sql.DB, mailAddress string, info *UserRegKeyInfo) (bool, error) {
	if nil == db {
		return false, errors.New("nil database")
	}
	if len(mailAddress) > 50 {
		return false, errors.New("too long account characters")
	}

	//	Select
	fetched := false
	sqlexpr := "select uid,regkey,account from userregkey where mail = '" + mailAddress + "'"
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false, errors.New("Select error." + err.Error())
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.uid, &info.regKey, &info.account)
			info.mailAddress = mailAddress
		}
	}

	return fetched, nil
}

func dbGetUserRegKeyByKey(db *sql.DB, key string, info *UserRegKeyInfo) (bool, error) {
	if nil == db {
		return false, errors.New("nil database")
	}
	if len(key) > 50 {
		return false, errors.New("too long key characters")
	}

	//	Select
	fetched := false
	sqlexpr := "select uid,mail,account from userregkey where regkey = '" + key + "'"
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return false, errors.New("Select error." + err.Error())
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.uid, &info.mailAddress, &info.account)
			info.regKey = key
		}
	}

	return fetched, nil
}

func dbInsertUserRegKey(db *sql.DB, info *UserRegKeyInfo) bool {
	sqlexpr := "insert into userregkey values(null, '" + info.mailAddress + "','" + info.regKey + "','" + info.account + "')"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]", sqlexpr, err.Error())
		return false
	}
	return true
}

func dbUpdateAccountByMail(db *sql.DB, mail string, account string) bool {
	sqlexpr := "update userregkey set account = '" + account + "' where mail = '" + mail + "'"
	_, err := db.Exec(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s] Error[%s]",
			sqlexpr, err.Error())
		return false
	}

	return true
}
