package main

import (
	"encoding/json"
	"log"
	"time"
)

type PlayerRankList struct {
	RankLevel   []UserRankInfo `json:"rank_level"`
	RankZhanShi []UserRankInfo `json:"rank_zhanshi"`
	RankFaShi   []UserRankInfo `json:"rank_fashi"`
	RankDaoShi  []UserRankInfo `json:"rank_daoshi"`
}

var g_RankListCache string = ""
var g_LastRankListCacheTime int64 = 0

func getPlayerRankList() string {
	if time.Now().Unix()-g_LastRankListCacheTime < 30 {
		return g_RankListCache
	}
	g_LastRankListCacheTime = time.Now().Unix()
	rankList := ""

	var rankData PlayerRankList
	rankData.RankLevel = dbGetUserRankInfoOrderByLevel(g_DBUser, 10, -1)
	rankData.RankZhanShi = dbGetUserRankInfoOrderByPower(g_DBUser, 10, 0)
	rankData.RankFaShi = dbGetUserRankInfoOrderByPower(g_DBUser, 10, 1)
	rankData.RankDaoShi = dbGetUserRankInfoOrderByPower(g_DBUser, 10, 2)

	jsBytes, err := json.Marshal(&rankData)
	if err != nil {
		log.Println("Err:Failed to marshal rank data.err:", err)
		return ""
	} else {
		rankList = string(jsBytes)
		g_RankListCache = rankList
		return rankList
	}
}
