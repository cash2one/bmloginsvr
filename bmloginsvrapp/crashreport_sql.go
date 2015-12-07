package main

import (
	"database/sql"
	_ "github.com/mattn/go-sqlite3"
	"os"
	"shareutils"
)

func initDatabaseCrashReport(path string) *sql.DB {
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
		create table crashreport(id integer primary key, version varchar(20), errorcode varchar(20), erroraddr varchar(20), times integer)
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

	return db
}

func dbIsCrashReportExists(db *sql.DB, version string, errorcode string, erroraddr string) bool {
	rows, err := db.Query("select count(*) as cnt from crashreport where version='?' and errorcode='?' and erroraddr='?'", version, errorcode, erroraddr)
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

func dbIncCrashReportTimes(db *sql.DB, version string, errorcode string, erroraddr string) bool {
	//	get times
	rows, err := db.Query("select times from crashreport where version = '?' and errorcode='?' and erroraddr='?'", version, errorcode, erroraddr)
	if err != nil {
		shareutils.LogErrorln(err)
		return false
	}

	var times int = -1
	defer rows.Close()
	for rows.Next() {
		rows.Scan(&times)
	}

	if -1 == times {
		return false
	}

	times++

	_, err = db.Exec("update crashreport where version='?' and errorcode='?' and erroraddr='?'", version, errorcode, erroraddr)
	if err != nil {
		shareutils.LogErrorln("db exec failed. err:", err)
		return false
	}

	return true
}

func dbInsertCrashReport(db *sql.DB, version string, errorcode string, erroraddr string) bool {
	if len(version) == 0 ||
		len(errorcode) == 0 ||
		len(erroraddr) == 0 {
		return false
	}
	if dbIsCrashReportExists(db, version, errorcode, erroraddr) {
		dbIncCrashReportTimes(db, version, errorcode, erroraddr)
		return true
	}

	_, err := db.Exec("insert into crashreport values(null, '?', '?', '?', ?)", version, errorcode, erroraddr, 1)
	if err != nil {
		shareutils.LogErrorln("db exec failed.err:", err)
		return false
	}

	return true
}
