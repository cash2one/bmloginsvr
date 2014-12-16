package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net/http"
	"net/smtp"
	"strings"
)

//	global variables
var g_listenAddress string
var g_mailAccount string
var g_mailPassword string
var g_smtpAddress string
var g_lsAddress string

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

func getRegKeyFromMailAddress(mailAddress string) string {
	return "10g5awe4g8wa4g8GRs"
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
	mailAddr := mailAddrArg[0]
	userRegKey := getRegKeyFromMailAddress(mailAddr)
	sendMailErr := SendMail(g_mailAccount, g_mailPassword, g_smtpAddress, mailAddr, "您的BackMIR注册秘钥", fillRegKeyMsg(userRegKey), "text")
	if sendMailErr == nil {
		//	成功
		retJson.ErrCode = 0
		retJson.ErrMsg = userRegKey
		jsData, _ := json.Marshal(retJson)
		w.Write(jsData)
	} else {
		log.Println("send mail error: ", sendMailErr)
		retJson.ErrCode = 1
		retJson.ErrMsg = "can't send mail"
		jsData, _ := json.Marshal(retJson)
		w.Write(jsData)
	}
}

func regAccountHandler(w http.ResponseWriter, r *http.Request) {

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

func main() {
	fmt.Println("bmregsvrapp started.")
	defer fmt.Println("bmregsvrapp terminated")

	flag.StringVar(&g_listenAddress, "listenaddress", "", "listen address")
	flag.StringVar(&g_mailAccount, "mailaccount", "", "mail account")
	flag.StringVar(&g_mailPassword, "mailpassword", "", "mail password")
	flag.StringVar(&g_smtpAddress, "smtpaddr", "", "mail smtp server address")
	flag.StringVar(&g_lsAddress, "lsaddress", "", "login server address")
	flag.Parse()

	http.HandleFunc("/", httpRequestEntry)
	err := http.ListenAndServe(g_listenAddress, nil)
	if err != nil {
		log.Fatal("ListenAndServe error: ", err)
	}
}
