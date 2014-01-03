package main

import (
	"os"
)

func PathExist(_path string) bool {
	_, err := os.Stat(_path)
	if err != nil {
		if os.IsNotExist(err) {
			return false
		}
	}
	return true
}
