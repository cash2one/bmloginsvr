package main

import (
	"encoding/json"
	//	"log"
	"net/http"
	"strconv"
	"strings"
	"sync"
	"time"
	"uuid"
)

const (
	kAccess_None = iota
	kAccess_Admin
	kAccess_SuperAdmin
)

type AdminHandlerSessionInfo struct {
	Guid       string
	IP         string
	Account    string
	Level      int
	VerifyTime int64
}

var g_adminHandlerSessions map[string]*AdminHandlerSessionInfo
var g_adminHandlerSessionsLock *sync.RWMutex
var g_adminHandlerSessionOperateTimes int
var g_adminHandlerSessionTimeoutSec int64

func init() {
	g_adminHandlerSessions = make(map[string]*AdminHandlerSessionInfo)
	g_adminHandlerSessionsLock = &sync.RWMutex{}
	g_adminHandlerSessionTimeoutSec = 30 * 60
}

func NewAdminHandlerSession(address string, account string, level int) *AdminHandlerSessionInfo {
	if len(address) == 0 ||
		len(account) == 0 {
		return nil
	}

	nowTime := time.Now().Unix()
	session := &AdminHandlerSessionInfo{}
	session.Account = account

	stringList := strings.Split(address, ":")
	if len(stringList) != 2 {
		return nil
	}
	session.IP = stringList[0]
	session.VerifyTime = nowTime
	session.Level = level

	guid := uuid.NewRandom().String()
	session.Guid = guid
	g_adminHandlerSessionOperateTimes++

	g_adminHandlerSessionsLock.Lock()
	defer g_adminHandlerSessionsLock.Unlock()

	g_adminHandlerSessions[guid] = session

	//	remove all timeout session
	if g_adminHandlerSessionOperateTimes > 10 {
		g_adminHandlerSessionOperateTimes = 0
		for k, v := range g_adminHandlerSessions {
			if nowTime-v.VerifyTime > g_adminHandlerSessionTimeoutSec {
				delete(g_adminHandlerSessions, k)
			}
		}
	}

	return session
}

func DelAdminHandlerSession(guid string) {
	g_adminHandlerSessionsLock.Lock()
	defer g_adminHandlerSessionsLock.Unlock()

	delete(g_adminHandlerSessions, guid)
}

//	0:ok 1:not exists 2:timeout 3.address error
const (
	kCheckAdminError_Ok = iota
	kCheckAdminError_NotExists
	kCheckAdminError_Timeout
	kCheckAdminError_AddressError
)

func CheckAdminHandlerSession(address string, session *AdminHandlerSessionInfo) int {
	if nil == session {
		return kCheckAdminError_NotExists
	}

	//	check timeout
	nowUnix := time.Now().Unix()
	if nowUnix-session.VerifyTime > g_adminHandlerSessionTimeoutSec {
		return kCheckAdminError_Timeout
	}

	//	check address
	stringList := strings.Split(address, ":")
	if len(stringList) != 2 {
		return kCheckAdminError_AddressError
	}

	if stringList[0] != session.IP {
		return kCheckAdminError_AddressError
	}

	return kCheckAdminError_Ok
}

func GetAdminHandlerSession(guid string) *AdminHandlerSessionInfo {
	g_adminHandlerSessionsLock.RLock()
	session, ok := g_adminHandlerSessions[guid]
	g_adminHandlerSessionsLock.RUnlock()

	if !ok {
		return nil
	}

	return session
}

type AdminHandlerRsp struct {
	Result int    `json:"Result"`
	Msg    string `json:"Msg"`
}

func adminHandler(w http.ResponseWriter, r *http.Request) {
	var rsp AdminHandlerRsp

	defer func() {
		bytes, _ := json.Marshal(&rsp)
		w.Write(bytes)
	}()

	action := r.FormValue("action")
	if len(action) == 0 {
		rsp.Msg = "Invalid action type"
		return
	}

	cookie, err := r.Cookie("verifycode")
	if err != nil ||
		len(cookie.Value) == 0 {
		//	需要验证
		if action != "verify" {
			rsp.Msg = "Access denied,not verified, error:" + err.Error()
		} else {
			//	进行验证流程
			account := r.FormValue("account")
			password := r.FormValue("password")

			if len(account) == 0 ||
				len(password) == 0 {
				rsp.Msg = "Invalid input parameters"
				return
			}

			evt := &MThreadMsg{}
			evt.Event = kMThreadMsg_VerifyAdmin
			evt.Msg = account + " " + password
			evt.RetChan = make(chan bool, 1)
			PostMThreadMsg(evt)
			evtRet, timeout := WaitMThreadMsg(evt, 500)

			if timeout {
				rsp.Msg = "Request timeout"
			} else {
				if !evtRet {
					rsp.Msg = "Verify administrator account failed"
				} else {
					//	添加新的session
					session := NewAdminHandlerSession(r.RemoteAddr, account, evt.WParam)
					if nil == session {
						rsp.Msg = "Cannot generate new session"
					} else {
						newCookie := http.Cookie{Name: "verifycode", Value: session.Guid, Path: "/"}
						http.SetCookie(w, &newCookie)
						rsp.Msg = "Verify success"
						LogInfoln("New session  ip:", session.IP, " guid:", session.Guid)
					}
				}
			}
		}

		return
	}

	session := GetAdminHandlerSession(cookie.Value)
	//	验证完毕 分发各个请求
	if checkRet := CheckAdminHandlerSession(r.RemoteAddr, session); checkRet != kCheckAdminError_Ok {
		if checkRet == kCheckAdminError_AddressError {
			rsp.Msg = "Invalid request address"
		} else if checkRet == kCheckAdminError_NotExists {
			rsp.Msg = "Invalid request session"
		} else if checkRet == kCheckAdminError_Timeout {
			rsp.Msg = "Authority timeout"
		}

		return
	}

	switch action {
	case "add_admin":
		{
			account := r.FormValue("account")
			levelStr := r.FormValue("level")

			level, err := strconv.Atoi(levelStr)
			if err != nil {
				level = 1
			}

			if len(account) == 0 {
				rsp.Msg = "Invalid input parameters"
				return
			}

			if session.Level < kAccess_SuperAdmin {
				rsp.Msg = "Access denied"
				return
			}

			evt := &MThreadMsg{}
			evt.Event = kMThreadMsg_AddAdmin
			evt.Msg = account
			evt.WParam = level
			evt.RetChan = make(chan bool, 1)
			PostMThreadMsg(evt)
			evtRet, timeout := WaitMThreadMsg(evt, 500)

			if timeout {
				rsp.Msg = "Request timeout"
			} else {
				if evtRet {
					rsp.Msg = "Add admin account success"
				} else {
					rsp.Msg = "Add admin account failed"
				}
			}
		}
	case "del_admin":
		{
			account := r.FormValue("account")
			if len(account) == 0 {
				rsp.Msg = "Invalid input parameters"
				return
			}

			if session.Level < kAccess_SuperAdmin {
				rsp.Msg = "Access denied"
				return
			}

			evt := &MThreadMsg{}
			evt.Event = kMThreadMsg_DelAdmin
			evt.Msg = account
			evt.RetChan = make(chan bool, 1)
			PostMThreadMsg(evt)
			evtRet, timeout := WaitMThreadMsg(evt, 500)

			if timeout {
				rsp.Msg = "Request timeout"
			} else {
				if evtRet {
					rsp.Msg = "Del admin account success"
				} else {
					rsp.Msg = "Del admin account failed"
				}
			}
		}
	case "enableolroom":
		{
			enableStr := r.FormValue("enable")
			enable, err := strconv.Atoi(enableStr)
			if err != nil {
				enable = 1
			}

			evt := &MThreadMsg{}
			evt.Event = kMThreadMsg_EnableOlRoom
			evt.WParam = enable
			evt.RetChan = make(chan bool, 1)
			PostMThreadMsg(evt)
			_, timeout := WaitMThreadMsg(evt, 500)

			if timeout {
				rsp.Msg = "Request timeout"
			} else {
				rsp.Msg = "enable olroom success"
			}
		}
	default:
		{
			rsp.Msg = "Invalid action type"
		}
	}
}
