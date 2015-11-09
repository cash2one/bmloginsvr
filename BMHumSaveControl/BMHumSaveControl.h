#ifndef _INC_BMHUMSAVECONTROL_
#define _INC_BMHUMSAVECONTROL_
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
#define BM_EXPORT	extern "C" __declspec (dllexport)
#else
#define BM_EXPORT	__declspec (dllexport)
#endif
//////////////////////////////////////////////////////////////////////////
//	Setting functions
//	Set password
BM_EXPORT int SetPassword(const char* _pszPassword);
//////////////////////////////////////////////////////////////////////////
//	Create a new hum save, 0:success|1:file already exists|2:create file failed
BM_EXPORT int CreateHumSave(const char* _pszPath);
//	Delete a exist hum save 0:success|1:delete failed|2:non-exist file
BM_EXPORT int DeleteHumSave(const char* _pszPath);
//	Open a new hum save, 0:failed|
BM_EXPORT int OpenHumSave(const char* _pszPath);
BM_EXPORT void CloseHumSave(int _hFileHandle);
//	Create a new gamerole, 0:success|1:no empty room|2:Invalid save handle|3:index already exist|4:data invalid|5:data empty|6:repeated name
BM_EXPORT int AddGameRole(int _hFileHandle, const char* _pszRoleName, char _job, char _sex);
//	Delete a exist gamerole, 0:success|1:invalid hum
BM_EXPORT int DelGameRole(int _hFileHandle, const char* _pszRoleName);
//	Get a gamerole information, 0:success|1:invalid index
BM_EXPORT int GetGameRoleInfo_Value(int _hFileHandle, int _index, char* _job, char* _sex, unsigned short* _level);
//	Get a gamerole information, 0:failed
BM_EXPORT char* GetGameRoleInfo_Name(int _hFileHandle, int _index);
//	Get name index < 0 || > 2 failed
BM_EXPORT int GetGameRoleIndex(int _hFileHandle, const char* _pszRoleName);
//	Read gamerole size |ret:size
BM_EXPORT int ReadGameRoleSize(int _hFileHandle, const char* _pszRoleName);
//	Read gamerole data 0:success|1:failed
BM_EXPORT int ReadGameRoleData(int _hFileHandle, const char* _pszRoleName, void* _pData);
//	Write gamerole data 0:success|1:non-exist gamerole
BM_EXPORT int WriteGameRoleData(int _hFileHandle, const char* _pszRoleName, const char* _pData, int _datalen);
//	Update gamerole head 0:success|1:non-exist gamerole
BM_EXPORT int WriteGameRoleInfo(int _hFileHandle, const char* _pszRoleName, char _job, char _sex, unsigned short _level);
//	Update gamerole head 0:success|1:failed
BM_EXPORT int UpdateGameRoleInfo(int _hFileHandle, const char* _pszRoleName, unsigned short _level);
//	Read gamerole head data 0:failed|other:size
BM_EXPORT int ReadGameRoleHeadInfo(int _hFileHandle, int _index, void* _pData);
//	Repair hum save 0:success|1:file not exists|2:can't open the specified file
BM_EXPORT int RepairHumSave(const char* _pszSavePath);
//	Read extend data size|ret:size
BM_EXPORT int ReadExtendDataSize(int _hFileHandle, const char* _pszRoleName, int _nExtendIndex);
//	Read extend data 0:success|1:failed
BM_EXPORT int ReadExtendData(int _hFileHandle, const char* _pszRoleName, int _nExtendIndex, void* _pData);
//	Write extend data 0:success|1:failed
BM_EXPORT int WriteExtendData(int _hFileHandle, const char* _pszRoleName, int _nExtendIndex, const char* _pData, int _nLength);

//////////////////////////////////////////////////////////////////////////
#endif