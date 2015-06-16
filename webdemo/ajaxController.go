package main

import (
	"log"
	"net/http"
	//"github.com/ziutek/mymysql/mysql"
	//_ "github.com/ziutek/mymysql/thrsafe"
	"encoding/json"
	"io/ioutil"
	//	"log"
)

type Result struct {
	Ret    int
	Reason string
	Data   interface{}
}

type MailVerifyResult struct {
	ErrCode int    `json:errcode`
	ErrMsg  string `json:errmsg`
}

type ajaxController struct {
}

func (this *ajaxController) LoginAction(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("content-type", "application/json;charset=UTF-8")
	err := r.ParseForm()
	if err != nil {
		OutputJson(w, 0, "参数错误", nil)
		return
	}

	admin_name := r.FormValue("admin_name")
	admin_password := r.FormValue("admin_password")

	if admin_name == "" || admin_password == "" {
		OutputJson(w, 0, "参数错误", nil)
		return
	}

	/*db := mysql.New("tcp", "", "192.168.100.166", "root", "test", "webdemo")
	if err := db.Connect(); err != nil {
		log.Println(err)
		OutputJson(w, 0, "数据库操作失败", nil)
		return
	}
	defer db.Close()

	rows, res, err := db.Query("select * from webdemo_admin where admin_name = '%s'", admin_name)
	if err != nil {
		log.Println(err)
		OutputJson(w, 0, "数据库操作失败", nil)
		return
	}*/

	admin_password_db := dbGetPasswordByAccount(g_DBUser, admin_name)

	if admin_password_db != admin_password {
		OutputJson(w, 0, "密码输入错误", nil)
		return
	}

	// 存入cookie,使用cookie存储
	cookie := http.Cookie{Name: "admin_name", Value: admin_name, Path: "/"}
	http.SetCookie(w, &cookie)

	OutputJson(w, 1, "操作成功", nil)
	return
}

func (this *ajaxController) RegkeyAction(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("content-type", "application/json;charset=UTF-8")
	err := r.ParseForm()
	if err != nil {
		OutputJson(w, 0, "参数错误", nil)
		return
	}

	regMail := r.FormValue("reg_mail")
	if "" == regMail {
		OutputJson(w, 0, "请输入注册邮箱", nil)
		return
	}

	reqAddr := g_RegServerAddr + g_MailVerify + "?mail=" + regMail
	var rsp *http.Response
	rsp, err = http.Get(reqAddr)
	if err != nil {
		OutputJson(w, 0, "http请求失败", nil)
		log.Println("http address:", reqAddr)
		return
	}
	defer rsp.Body.Close()
	body, err := ioutil.ReadAll(rsp.Body)
	if err != nil {
		OutputJson(w, 0, "读取缓冲区失败", nil)
		return
	}

	regMailRet := &MailVerifyResult{}
	err = json.Unmarshal(body, regMailRet)
	if err != nil {
		OutputJson(w, 0, "反序列化json失败", nil)
		return
	}

	if 0 == regMailRet.ErrCode {
		OutputJson(w, 1, regMailRet.ErrMsg, nil)
	} else {
		OutputJson(w, 0, "获取密钥失败", nil)
	}
}

func (this *ajaxController) RegisterAction(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("content-type", "application/json;charset=UTF-8")
	err := r.ParseForm()
	if err != nil {
		OutputJson(w, 0, "参数错误", nil)
		return
	}

	regMail := r.FormValue("reg_mail2")
	if "" == regMail {
		OutputJson(w, 0, "邮箱错误", nil)
		return
	}

	regKey := r.FormValue("reg_key")
	if "" == regMail {
		OutputJson(w, 0, "密钥错误", nil)
		return
	}

	regAccount := r.FormValue("reg_account")
	if "" == regAccount {
		OutputJson(w, 0, "账户非法", nil)
		return
	}

	regPassword := r.FormValue("reg_password")
	if "" == regMail {
		OutputJson(w, 0, "密码非法", nil)
		return
	}

	reqAddr := g_RegServerAddr + g_RegAccount + "?mail=" + regMail + "&key=" + regKey + "&account=" + regAccount + "&password=" + regPassword
	var rsp *http.Response
	rsp, err = http.Get(reqAddr)
	if err != nil {
		OutputJson(w, 0, "http请求失败", nil)
		log.Println("http address:", reqAddr)
		return
	}
	defer rsp.Body.Close()
	body, err := ioutil.ReadAll(rsp.Body)
	if err != nil {
		OutputJson(w, 0, "读取缓冲区失败", nil)
		return
	}

	regMailRet := &MailVerifyResult{}
	err = json.Unmarshal(body, regMailRet)
	if err != nil {
		OutputJson(w, 0, "反序列化json失败", nil)
		return
	}

	if 0 == regMailRet.ErrCode {
		OutputJson(w, 1, regMailRet.ErrMsg, nil)
	} else {
		OutputJson(w, 0, regMailRet.ErrMsg, nil)
	}
}

func (this *ajaxController) ModifyAction(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("content-type", "application/json;charset=UTF-8")
	err := r.ParseForm()
	if err != nil {
		OutputJson(w, 0, "参数错误", nil)
		return
	}

	regMail := r.FormValue("modify_mail")
	if "" == regMail {
		OutputJson(w, 0, "邮箱错误", nil)
		return
	}

	regAccount := r.FormValue("modify_account")
	if "" == regAccount {
		OutputJson(w, 0, "账户非法", nil)
		return
	}

	regPassword := r.FormValue("modify_password")
	if "" == regMail {
		OutputJson(w, 0, "密码非法", nil)
		return
	}

	reqAddr := g_RegServerAddr + g_ModifyPassword + "?mail=" + regMail + "&account=" + regAccount + "&password=" + regPassword
	var rsp *http.Response
	rsp, err = http.Get(reqAddr)
	if err != nil {
		OutputJson(w, 0, "http请求失败", nil)
		log.Println("http address:", reqAddr)
		return
	}
	defer rsp.Body.Close()
	body, err := ioutil.ReadAll(rsp.Body)
	if err != nil {
		OutputJson(w, 0, "读取缓冲区失败", nil)
		return
	}

	regMailRet := &MailVerifyResult{}
	err = json.Unmarshal(body, regMailRet)
	if err != nil {
		OutputJson(w, 0, "反序列化json失败", nil)
		return
	}

	if 0 == regMailRet.ErrCode {
		OutputJson(w, 1, regMailRet.ErrMsg, nil)
	} else {
		OutputJson(w, 0, regMailRet.ErrMsg, nil)
	}
}

func (this *ajaxController) IndexAction(w http.ResponseWriter, r *http.Request) {
	NotFoundHandler(w, r)
}

func OutputJson(w http.ResponseWriter, ret int, reason string, i interface{}) {
	out := &Result{ret, reason, i}
	b, err := json.Marshal(out)
	if err != nil {
		return
	}
	w.Write(b)

	if 0 == ret {
		log.Println("request failed.", reason)
	}
}
