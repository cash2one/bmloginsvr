package main

import (
	"database/sql"
	"errors"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
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
		create table useraccount(uid integer primary key, account varchar(20), password varchar(20),bool online)
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
	sqlexpr := "select uid,password,online from useraccount where account = " + account
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on execute expression[%s]", sqlexpr)
		return false, errors.New("Select error." + err.Error())
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			fetched = true
			rows.Scan(&info.uid, &info.password, &info.online)
			log.Println("Fetched uid:", info.uid, " password:", info.password, " online:", info.online)
		}
	}

	return fetched, nil
}
