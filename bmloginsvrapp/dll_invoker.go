package main

import (
	//"dbgutil"
	//	"log"
	"shareutils"
	"syscall"
)

var (
	g_dllHumSave syscall.DLL
	g_procMap    = make(map[string]*syscall.Proc)

	g_procDllName = []string{
		"CreateHumSave",
		"OpenHumSave",
		"CloseHumSave",
		"AddGameRole",
		"DelGameRole",
		"GetGameRoleInfo_Value",
		"GetGameRoleInfo_Name",
		"GetGameRoleIndex",
		"ReadGameRoleData",
		"WriteGameRoleData",
		"WriteGameRoleInfo",
		"ReadGameRoleSize",
		"UpdateGameRoleInfo",
		"ReadGameRoleHeadInfo",
	}
)

const (
	FUNC_CreateHumSave = iota
	FUNC_OpenHumSave
	FUNC_CloseHumSave
	FUNC_AddGameRole
	FUNC_DelGameRole
	FUNC_GetGameRoleInfo_Value
	FUNC_GetGameRoleInfo_Name
	FUNC_GetGameRoleIndex
	FUNC_ReadGameRoleData
	FUNC_WriteGameRoleData
	FUNC_WriteGameROleInfo
	FUNC_ReadGameRoleSize
	FUNC_UpdateGameRoleInfo
	FUNC_ReadGameRoleHeadInfo
)

func initDllModule(name string) bool {
	allLoaded := true
	g_dllHumSave, err := syscall.LoadDLL(name)
	if err != nil {
		shareutils.LogErrorln("Can't load [", name, "]")
		return false
	}
	//	Get all module
	for _, str := range g_procDllName {
		proc, err := g_dllHumSave.FindProc(str)
		if err == nil {
			g_procMap[str] = proc
			shareutils.LogInfoln("Proccess address[", str, "] loaded...")
			//dbgutil.Display("ProcName", str, "ProcAddr", proc)
		} else {
			shareutils.LogErrorln("ProcName[", str, "] load failed...", err)
			allLoaded = false
		}
	}

	return allLoaded
}

func releaseDllModule() {
	if len(g_dllHumSave.Name) != 0 {
		for _, str := range g_procDllName {
			delete(g_procMap, str)
		}
		shareutils.LogInfoln(g_dllHumSave.Name, " has been released...")
		g_dllHumSave.Release()
	}
}
