#ifndef _INC_BMHUMSAVECONTROL_
#define _INC_BMHUMSAVECONTROL_
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
#define BM_EXPORT	extern "C" __declspec (dllexport)
#else
#define BM_EXPORT	__declspec (dllexport)
#endif
//////////////////////////////////////////////////////////////////////////
//	Create a new hum save, 0:success|1:file already exists|2:create file failed
BM_EXPORT int CreateHumSave(const char* _pszPath);
BM_EXPORT int OpenHumSave(const char* _pszPath);
BM_EXPORT void CloseHumSave(int _hFileHandle);
BM_EXPORT int AddGameRole(int _hFileHandle, const char* _pszRoleName, char _job, char _sex);
BM_EXPORT int DelGameRole(int _hFileHandle, const char* _pszRoleName);
BM_EXPORT int GetGameRoleInfo(int _index, char* _pszRoleName, char* _job, char* _sex);
//////////////////////////////////////////////////////////////////////////
#endif