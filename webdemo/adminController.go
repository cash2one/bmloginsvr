package main

import (
    "log"
    "net/http"
    "html/template"
	"strconv"
	"encoding/json"
	"io/ioutil"
)

type User struct {
    UserName string
}

type adminController struct {
}

func (this *adminController)IndexAction(w http.ResponseWriter, r *http.Request, user string) {
    t, err := template.ParseFiles("template/html/admin/index.html")
    if (err != nil) {
        log.Println(err)
    }
    t.Execute(w, &User{user})
}

func (this *adminController)InsertDonateAction(w http.ResponseWriter, r *http.Request, user string) {
	w.Header().Set("content-type", "application/json;charset=UTF-8")
	err := r.ParseForm()
	if err != nil {
		OutputJson(w, 0, "参数错误", nil)
		return
	}

	donateName := r.FormValue("donate_name")
	if "" == donateName {
		OutputJson(w, 0, "名字错误", nil)
		return
	}

	donateOrder := r.FormValue("donate_order")
	if "" == donateOrder {
		OutputJson(w, 0, "订单号错误", nil)
		return
	}

	donateMoneyStr := r.FormValue("donate_money")
	if "" == donateMoneyStr {
		OutputJson(w, 0, "密码非法", nil)
		return
	}
	
	_, err = strconv.Atoi(donateMoneyStr)
	if err != nil {
		log.Println("failed to convert donateMoneyStr to int type:", err)
		OutputJson(w, 0, "金额非法", nil)
		return
	}
	
	reqAddr := g_RegServerAddr + g_InsertDonate + "?name=" + donateName + "&donate=" + donateMoneyStr + "&orderid=" + donateOrder
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