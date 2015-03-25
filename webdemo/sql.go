package main

import (
	"database/sql"
	//	"errors"
	_ "github.com/mattn/go-sqlite3"
	"log"
	"os"
	//	"strconv"
)

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
		create table webusers(uid integer primary key, account varchar(25), password varchar(25), authority integer)
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

func dbGetPasswordByAccount(db *sql.DB, account string) string {
	if nil == db {
		return ""
	}
	if len(account) >= 25 {
		return ""
	}

	//	Select
	password := ""
	sqlexpr := "select password from webusers where account = '" + account + "'"
	rows, err := db.Query(sqlexpr)
	if err != nil {
		log.Printf("Error on executing expression[%s]error[%s]", sqlexpr, err.Error())
		return ""
	} else {
		defer rows.Close()
		//	Read data
		if rows.Next() {
			rows.Scan(&password)
			return password
		}
	}

	return ""
}
