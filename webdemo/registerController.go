package main

import (
	"html/template"
	"log"
	"net/http"
)

type registerController struct {
}

func (this *registerController) IndexAction(w http.ResponseWriter, r *http.Request) {
	t, err := template.ParseFiles("template/html/register/index.html")
	if err != nil {
		log.Println(err)
	}
	t.Execute(w, nil)
}
