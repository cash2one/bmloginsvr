package main

import (
	"time"
)

//	timer player rank
var g_lastSendPlayerTime int64 = 0

func UpdateTimerEvent() {
	nowTime := time.Now().Unix()

	if nowTime-g_lastSendPlayerTime > 30 {
		g_lastSendPlayerTime = nowTime
		//	update player rank
		if nil != g_DBUser {
			rankListData := getPlayerRankList()
			for _, svr := range g_ServerList.allusers {
				svrUser, ok := svr.(*ServerUser)
				if !ok ||
					nil == svrUser {
					continue
				}

				if svrUser.serverid >= 0 &&
					svrUser.serverid < 100 {
					svrUser.SendUserMsg(loginopstart+22, rankListData)
				}
			}
		}
	}
}
