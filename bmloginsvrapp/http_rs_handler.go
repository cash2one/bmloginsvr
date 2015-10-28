package main

import (
	"encoding/json"
	"log"
	"net/http"
	"strings"
)

type RsHandlerRsp struct {
	Result int    `json:"Result"`
	Msg    string `json:"Msg"`
}

func rsHandler(w http.ResponseWriter, r *http.Request) {
	var rsp RsHandlerRsp
	rsp.Result = -1

	defer func() {
		bytes, _ := json.Marshal(&rsp)
		w.Write(bytes)
	}()

	//	check access
	accessible := true
	if nil == g_ControlAddr ||
		len(g_ControlAddr) == 0 {
		accessible = false
	} else {
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
			accessible = false
		}
	}

	if !accessible {
		rsp.Msg = "Access denied"
		return
	}

	action := r.FormValue("action")

	log.Println("register server request:", action)

	switch action {
	case "registeraccount":
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
					rsp.Result = 0
					rsp.Msg = "Register success"
				}
			}

			if evtRet {
				log.Println("register account [", account, "] success")
			} else {
				log.Println("register account [", account, "] failed")
			}
		}
	case "modifypassword":
		{
			account := r.FormValue("account")
			password := r.FormValue("password")

			evt := &MThreadMsg{}
			evt.Event = kMThreadMsg_LsModifyPassword
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
					rsp.Msg = "modify password failed"
				} else {
					//	注册成功
					rsp.Result = 0
					rsp.Msg = "modify password"
				}
			}

			if evtRet {
				log.Println("modify password [", account, "] success")
			} else {
				log.Println("modify password [", account, "] failed")
			}
		}
	default:
		{
			rsp.Msg = "Invalid action type"
		}
	}
}
