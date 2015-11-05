package main

//#include <stdlib.h>
import "C"

import (
	"fmt"
	"log"
	"shareutils"
	"unsafe"
)

//	CGO无法创建test 就这样写了
func bmDllTest() {
	initDllModule("BMHumSaveControl.dll")
	defer func() {
		e := recover()
		if e != nil {
			shareutils.LogErrorln(e)
			var input string
			fmt.Scanln(&input)
		}
	}()

	//	Test CreateHumSave
	r1, _, _ := g_procMap["CreateHumSave"].Call(uintptr(unsafe.Pointer(C.CString("hum.zip"))))
	if r1 != 0 {
		shareutils.LogErrorln("CreateHumSave failed.")
	}

	//	Test OpenHumSave
	var filehandle uintptr = 0
	r1, _, _ = g_procMap["OpenHumSave"].Call(uintptr(unsafe.Pointer(C.CString("hum.zip"))))
	if r1 == 0 {
		shareutils.LogErrorln("OpenHumSave failed.")
	} else {
		filehandle = r1
	}

	//	Test AddGameRole
	var job, sex uint8 = 0, 1
	r1, _, _ = g_procMap["AddGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god00"))),
		uintptr(job),
		uintptr(sex))
	if r1 != 0 {
		shareutils.LogErrorln("AddGameRole failed.ret:", r1)
	}
	r1, _, _ = g_procMap["AddGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god01"))),
		uintptr(job),
		uintptr(sex))
	if r1 != 0 {
		log.Println("AddGameRole failed.ret:", r1)
	}
	r1, _, _ = g_procMap["AddGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god02"))),
		uintptr(job),
		uintptr(sex))
	if r1 != 0 {
		log.Println("AddGameRole failed.ret:", r1)
	}

	//	Test WriteGameRoleData
	r1, _, _ = g_procMap["WriteGameRoleData"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god00"))),
		uintptr(unsafe.Pointer(C.CString("data of god00"))),
		uintptr(13))
	if r1 != 0 {
		log.Println("WriteGameRoleData failed.ret:", r1)
	}

	//	Test ReadGameRoleData
	var outsize int32 = 0
	r1, _, _ = g_procMap["ReadGameRoleData"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god00"))),
		uintptr(unsafe.Pointer(&outsize)))
	if r1 == 0 {
		log.Println("ReadGameRoleData failed.")
	} else {
		ccharptr := (*C.char)(unsafe.Pointer(r1))
		log.Println("ReadGameRoleData:", C.GoString(ccharptr))
		C.free(unsafe.Pointer(r1))
	}

	//	Test WriteGameRoleInfo
	var level uint16 = 22
	r1, _, _ = g_procMap["WriteGameRoleInfo"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god00"))),
		uintptr(0),
		uintptr(1),
		uintptr(level))
	if r1 != 0 {
		log.Println("WriteGameRoleInfo failed.ret:", r1)
	}

	//	Test GetGameRoleIndex
	var heroindex uintptr = 0
	r1, _, _ = g_procMap["GetGameRoleIndex"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god00"))))
	if r1 == 3 {
		log.Println("GetGameRoleIndex failed.ret:", r1)
	} else {
		heroindex = r1
	}

	//	Test GetGameRoleInfo_Name
	r1, _, _ = g_procMap["GetGameRoleInfo_Name"].Call(filehandle,
		heroindex)
	if r1 == 0 {
		log.Println("GetGameRoleInfo_Name failed.")
	} else {
		log.Println("Hero name:", C.GoString((*C.char)(unsafe.Pointer(r1))))
	}

	//	Test GetGameRoleInfo_Value
	level = 0
	job = 0
	sex = 0
	r1, _, _ = g_procMap["GetGameRoleInfo_Value"].Call(filehandle,
		heroindex,
		uintptr(unsafe.Pointer(&job)),
		uintptr(unsafe.Pointer(&sex)),
		uintptr(unsafe.Pointer(&level)))
	if r1 != 0 {
		log.Println("GetGameRoleInfo_Value failed.ret", r1)
	} else {
		log.Println("Hero job ", job, " Hero sex ", sex, " Hero level ", level)
	}

	//	Test DelGameRole
	r1, _, _ = g_procMap["DelGameRole"].Call(filehandle,
		uintptr(unsafe.Pointer(C.CString("god01"))))
	if r1 != 0 {
		log.Println("DelGameRole failed.ret:", r1)
	}

	//	Test CloseHumSave
	g_procMap["CloseHumSave"].Call(filehandle)
}
