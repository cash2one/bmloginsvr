package main

import (
	"LSControlProto"
	"code.google.com/p/goprotobuf/proto"
	"encoding/json"
	"log"
	"net/http"
	"net/smtp"
	"strings"
	"uuid"
)

/*
	ReqType:
			 0:unrecognized request
			 1:mailVerify
			 2:reqAccount
*/

/*
	ErrCode: -1:unknown reason 0:success 1:fail
*/
type httpResult struct {
	ReqType int
	ErrCode int
	ErrMsg  string
}

/*
 *	user : example@example.com login smtp server user
 *	password: xxxxx login smtp server password
 *	host: smtp.example.com:port   smtp.163.com:25
 *	to: example@example.com;example1@163.com;example2@sina.com.cn;...
 *  subject:The subject of mail
 *  body: The content of mail
 *  mailtyoe: mail type html or text
 */

func SendMail(user, password, host, to, subject, body, mailtype string) error {
	hp := strings.Split(host, ":")
	auth := smtp.PlainAuth("", user, password, hp[0])
	var content_type string
	if mailtype == "html" {
		content_type = "Content-Type: text/" + mailtype + "; charset=UTF-8"
	} else {
		content_type = "Content-Type: text/plain" + "; charset=UTF-8"
	}

	msg := []byte("To: " + to + "\r\nFrom: " + user + "<" + user + ">\r\nSubject: " + subject + "\r\n" + content_type + "\r\n\r\n" + body)
	send_to := strings.Split(to, ";")
	err := smtp.SendMail(host, auth, user, send_to, msg)
	return err
}

func getRegKey() string {
	return uuid.NewRandom().String()
}

func fillRegKeyMsg(regKey string) string {
	content := "您的BackMIR服务器注册秘钥为:" + regKey + "\r\n请妥善保管，并在注册界面填入此秘钥来注册账号\r\n\r\n\r\n\r\nBackMIR服务器的地址为" + g_lsAddress + "， 感谢您的支持"
	return content
}

func mailVerifyHandler(w http.ResponseWriter, r *http.Request) {
	retJson := &httpResult{
		ReqType: 1,
		ErrCode: -1,
		ErrMsg:  "",
	}

	//	获得参数
	mailAddrArg, ok := r.Form["mail"]
	if !ok {
		retJson.ErrMsg = "mail arg is nil"
	}
	if ok {
		if len(mailAddrArg) == 0 {
			retJson.ErrMsg = "mail arg is nil"
			ok = false
		}
	}
	if ok {
		if len(mailAddrArg[0]) == 0 {
			retJson.ErrMsg = "mail arg list is nil"
			ok = false
		} else if !strings.Contains(mailAddrArg[0], "@") {
			retJson.ErrMsg = "invalid mail address"
			ok = false
		}
	}

	if !ok {
		retJson.ErrCode = 1
		jsData, err := json.Marshal(retJson)
		if err == nil {
			w.Write(jsData)
		} else {
			log.Println("failed to marshal httpRet", retJson)
		}
		return
	}

	//	解析参数
	mailAddr := strings.ToLower(mailAddrArg[0])
	userRegKey := ""

	//	查找是否得到过key
	userInfo := &UserRegKeyInfo{}
	evt := &goChanEvent{}
	evt.command = CHANEVENT_DBOP_SELECTBYMAIL
	evt.arguments = make([]string, 1)
	evt.arguments[0] = mailAddr
	evt.argument = userInfo
	evt.retChan = make(chan bool)
	g_ChanEvent <- evt
	dbExist := false

	select {
	case dbExist = <-evt.retChan:
		{
			// nothing
		}
	}

	if dbExist {
		log.Println("reg key already exist.[", mailAddr, "]")
		userRegKey = userInfo.regKey
	} else {
		log.Println("generate a new reg key.[", mailAddr, "]")
		userRegKey = getRegKey()

		//	插入记录
		userInfo.mailAddress = mailAddr
		userInfo.regKey = userRegKey

		dbevt := &goChanEvent{}
		dbevt.command = CHANEVENT_DBOP_INSERT
		dbevt.argument = userInfo
		dbevt.retChan = make(chan bool)
		g_ChanEvent <- dbevt

		insertOk := false

		select {
		case insertOk = <-dbevt.retChan:
			{
				if !insertOk {
					retJson.ErrCode = 1
					retJson.ErrMsg = "failed to insert record in db"
					jsData, err := json.Marshal(retJson)
					if err == nil {
						w.Write(jsData)
					} else {
						log.Println("failed to marshal httpRet", retJson)
					}
					return
				}
			}
		}
	}

	//sendMailErr := SendMail(g_mailAccount, g_mailPassword, g_smtpAddress, mailAddr, "您的BackMIR注册秘钥", fillRegKeyMsg(userRegKey), "text")
	evt = &goChanEvent{}
	evt.command = CHANEVENT_SENDMAIL
	evt.arguments = make([]string, 3)
	evt.arguments[0] = mailAddr
	evt.arguments[1] = "您的BackMIR注册秘钥"
	evt.arguments[2] = fillRegKeyMsg(userRegKey)

	if len(userInfo.account) != 0 {
		evt.arguments[2] += "\r\n\r\n您的邮箱已注册过账户，账户名为:" + userInfo.account
	}

	evt.retChan = make(chan bool)
	g_ChanEvent <- evt

	sendMailRet := false

	select {
	case ret := <-evt.retChan:
		{
			if ret {
				sendMailRet = true
			}
		}
	}

	//if sendMailErr == nil {
	if sendMailRet {
		//	成功
		retJson.ErrCode = 0
		retJson.ErrMsg = userRegKey
		jsData, _ := json.Marshal(retJson)
		w.Write(jsData)
	} else {
		log.Println("send mail error")
		retJson.ErrCode = 1
		retJson.ErrMsg = "can't send mail"
		jsData, _ := json.Marshal(retJson)
		w.Write(jsData)
	}
}

func regAccountHandler(w http.ResponseWriter, r *http.Request) {
	retJson := &httpResult{
		ReqType: 2,
		ErrCode: -1,
		ErrMsg:  "",
	}

	//	获得参数
	mailAddrArg, ok := r.Form["mail"]
	if !ok {
		retJson.ErrMsg = "mail arg is nil"
	}
	if ok {
		if len(mailAddrArg) == 0 {
			retJson.ErrMsg = "mail arg is nil"
			ok = false
		}
	}
	if ok {
		if len(mailAddrArg[0]) == 0 {
			retJson.ErrMsg = "mail arg list is nil"
			ok = false
		} else if !strings.Contains(mailAddrArg[0], "@") {
			retJson.ErrMsg = "invalid mail address"
			ok = false
		}
	}

	regKeyArg, keyOk := r.Form["key"]
	if !keyOk {
		retJson.ErrMsg = "key arg is nil"
	}
	if keyOk {
		if len(regKeyArg) == 0 {
			retJson.ErrMsg = "key arg is nil"
			keyOk = false
		}
	}
	if keyOk {
		if len(regKeyArg[0]) == 0 {
			retJson.ErrMsg = "key arg list is nil"
			keyOk = false
		} else if len(regKeyArg[0]) >= 50 {
			retJson.ErrMsg = "key arg is too long"
			keyOk = false
		}
	}

	pwdArg, pwdOk := r.Form["password"]
	if !pwdOk {
		retJson.ErrMsg = "password arg is nil"
	}
	if pwdOk {
		if len(pwdArg) == 0 {
			retJson.ErrMsg = "password arg is nil"
			pwdOk = false
		}
	}
	if pwdOk {
		if len(pwdArg[0]) == 0 {
			retJson.ErrMsg = "password arg list is nil"
			pwdOk = false
		} else if len(pwdArg[0]) >= 20 {
			retJson.ErrMsg = "password arg is too long"
			pwdOk = false
		}
	}

	accountArg, actOk := r.Form["account"]
	if !actOk {
		retJson.ErrMsg = "account arg is nil"
	}
	if actOk {
		if len(accountArg) == 0 {
			retJson.ErrMsg = "account arg is nil"
			actOk = false
		}
	}
	if actOk {
		if len(accountArg[0]) == 0 {
			retJson.ErrMsg = "account arg list is nil"
			actOk = false
		} else if len(accountArg[0]) >= 20 {
			retJson.ErrMsg = "account arg is too long"
			actOk = false
		}
	}

	if !ok ||
		!keyOk ||
		!pwdOk ||
		!actOk {
		retJson.ErrCode = 1
		jsData, err := json.Marshal(retJson)
		if err == nil {
			w.Write(jsData)
		} else {
			log.Println("failed to marshal httpRet", retJson)
		}
		return
	}

	//	解析参数
	mailAddr := strings.ToLower(mailAddrArg[0])
	userRegKey := regKeyArg[0]
	password := pwdArg[0]
	account := accountArg[0]

	//	是否已经注册过
	userInfo := &UserRegKeyInfo{}

	evt := &goChanEvent{}
	evt.command = CHANEVENT_DBOP_SELECTBYMAIL
	evt.arguments = make([]string, 1)
	evt.arguments[0] = mailAddr
	evt.argument = userInfo
	evt.retChan = make(chan bool)
	g_ChanEvent <- evt

	exist := false

	select {
	case exist = <-evt.retChan:
		{
			// nothing
		}
	}

	if exist {
		if len(userInfo.account) != 0 {
			//	注册过 返回错误
			retJson.ErrCode = 1
			retJson.ErrMsg = "mail address already registed"
			jsData, err := json.Marshal(retJson)
			if err == nil {
				w.Write(jsData)
			} else {
				log.Println("failed to marshal httpRet", retJson)
			}
			return
		} else {
			//	检测key是否正确
			if userInfo.regKey != userRegKey {
				retJson.ErrCode = 1
				retJson.ErrMsg = "invalid register key"
				jsData, err := json.Marshal(retJson)
				if err == nil {
					w.Write(jsData)
				} else {
					log.Println("failed to marshal httpRet", retJson)
				}
				return
			}
		}
	} else {
		//	没有对应的邮件信息 错误
		retJson.ErrCode = 1
		retJson.ErrMsg = "mail address not registed"
		jsData, err := json.Marshal(retJson)
		if err == nil {
			w.Write(jsData)
		} else {
			log.Println("failed to marshal httpRet", retJson)
		}
		return
	}

	//	发送成功接收json
	retJson.ErrCode = 0
	retJson.ErrMsg = "您的请求已接收，请注意查收邮件"
	jsData, _ := json.Marshal(retJson)
	w.Write(jsData)

	//	发送注册数据包到LS
	pkg := &LSControlProto.RSRegistAccountReq{}
	pkg.Account = proto.String(account)
	pkg.Password = proto.String(password)
	pkg.Mail = proto.String(mailAddr)
	data, pkgErr := proto.Marshal(pkg)
	if pkgErr == nil {
		SendProtoBuf(uint32(LSControlProto.Opcode_PKG_RegistAccountWithInfoReq), data)
	}
}

func defHandler(w http.ResponseWriter, r *http.Request) {
	retJson := &httpResult{
		ReqType: 0,
		ErrCode: 0,
		ErrMsg:  "",
	}

	jsData, err := json.Marshal(retJson)
	if err == nil {
		w.Write(jsData)
	} else {
		log.Println("failed to marshal httpRet", retJson)
	}
}

func httpRequestEntry(w http.ResponseWriter, r *http.Request) {
	r.ParseForm() //开始解析参数，默认是不会解析的

	//	dispatch不同路径的请求
	reqAddr := strings.ToLower(r.URL.Path)
	log.Println("new request [", reqAddr, "] from ", r.RemoteAddr)

	if !g_clientRegisted &&
		!g_clientVerified {
		retJson := &httpResult{
			ReqType: 0,
			ErrCode: 1,
			ErrMsg:  "ls not connected",
		}

		jsData, err := json.Marshal(retJson)
		if err == nil {
			w.Write(jsData)
		} else {
			log.Println("failed to marshal httpRet", retJson)
		}

		return
	}

	switch reqAddr {
	case "/mailverify":
		{
			mailVerifyHandler(w, r)
		}
	case "/regaccount":
		{
			regAccountHandler(w, r)
		}
	default:
		{
			defHandler(w, r)
		}
	}
}
