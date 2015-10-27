package main

import (
	"encoding/json"
	"net/http"
	"strings"
)

type LsHandlerRsp struct {
	Result int    `json:"errcode"`
	Msg    string `json:"errmsg"`
}

func lsHandler(w http.ResponseWriter, r *http.Request) {
	var rsp LsHandlerRsp
	rsp.Result = -1

	defer func() {
		bytes, _ := json.Marshal(&rsp)
		w.Write(bytes)
	}()

	//	check access
	if nil == g_ControlAddr ||
		len(g_ControlAddr) == 0 {
		rsp.Msg = "Access denied"
		return
	}

	stringList := strings.Split(r.RemoteAddr, ":")
	ip := stringList[0]
	ipExists := false

	for _, v := range g_ControlAddr {
		if v == ip {
			ipExists = true
			break
		}
	}

	if !ipExists {
		rsp.Msg = "Access denied"
		return
	}

	action := r.FormValue("action")

	switch action {
	case "register":
		{
			account := r.FormValue("account")
			password := r.FormValue("password")

			evt := &MThreadMsg{}
			evt.Event = kMThreadMsg_LsRegisterAccount
			evt.Msg = account + " " + password
			evt.RetChan = make(chan bool, 1)
			PostMThreadMsg(evt)
			evtRet, timeout := WaitMThreadMsg(evt, 500)

			if timeout {
				rsp.Result = -2
				rsp.Msg = "Request timeout"
			} else {
				if !evtRet {
					rsp.Result = -3
					rsp.Msg = "Register account failed"
				} else {
					//	注册成功
					rsp.Msg = "Register success"
				}
			}
		}
	default:
		{
			rsp.Msg = "Invalid action type"
		}
	}
}
