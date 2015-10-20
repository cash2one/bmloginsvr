#include "MapLooker.h"
#include "MirMap.h"
#include <iostream>
#include <sstream>
#include <Shlwapi.h>
#include <assert.h>
#include <set>
#pragma comment(lib, "shlwapi.lib")
using namespace std;
//////////////////////////////////////////////////////////////////////////
const char* g_szIntro ="usage:\n\
	//	setdata x,y o[n] : set barea to n\n\
	//	setdata x,y b[n] : set bkimg to n\n\
	//	setdata x,y m[n] : set midimg to n\n\
	//	setdata x,y f[n] : set frimg to n\n\
	//	setdata x,y a[n] : set aniframe to n\n\
	//	setdata x,y d[n] : set doorindex to n\n\
	//	setdata x,y c[n] : set dooroffset to n\n\
	//	getdata x,y\n\
	//	load path\n\
	//	getcoord o[n] : get coordinate where barea equals n\n\
	//	getcoord a[n] : get coordinate where has an animation\n\
	//	getcoord d[n] : get coordinate where has a door\n\
	//	whole o[n] : set the whole map brea to n\n\
	//	sort o[n],m : sort the o[n] from m\n\
	//	list o[n] : list object n FrImg\n\
	//	list b[n] : list BkImg\n\
	//	list m[n] : list MidImg\n\
	//	getindex : get the map resource index\n\
	//	setindex [n] : set the map resource index\n\
	//	save path : save the map\n";
//////////////////////////////////////////////////////////////////////////
void MapLooker()
{
	TileMap xMap;
	string xInput;
	string xEle;
	char szBuf[1024];
	GetModuleFileName(NULL, szBuf, 260);
	PathRemoveFileSpec(szBuf);
	strcat(szBuf, "\\log.txt");
	FILE* pfile = fopen(szBuf, "w");
	if(pfile == NULL)
	{
		printf("can't create log file!\n");
		return;
	}

	//printf("命令说明:\nload [path] : 读取地图\ngetcoord [a,o[n]] : a显示地图动画坐标 o[n]显示objectn的坐标\n");
	//printf("getdata [x],[y] 读取x,y处数据\nsetdata [x],[y] [a[n], o[n], d[n]]\nquit 退出\n");
	//printf("[param] 在实际输入的时候不带[]\n");
	printf(g_szIntro);

	int nMode = 0;
	int nParams = 0;
	bool bQuit = false;
	int nCoordX = 0;
	int nCoordY = 0;

	while(getline(cin, xInput))
	{
		istringstream istrstream(xInput);
		while(istrstream >> xEle)
		{
			if(nMode == 0)
			{
				if(xEle == "load")
				{
					nMode = MODE_LOADMAP;
					continue;
				}
				else if(xEle == "getcoord")
				{
					nMode = MODE_GETCOORD;
				}
				else if(xEle == "getdata")
				{
					nMode = MODE_GETDATA;
				}
				else if(xEle == "quit")
				{
					bQuit = true;
					return;
				}
				else if(xEle == "setdata")
				{
					nMode = MODE_SETDATA;
				}
				else if(xEle == "whole")
				{
					nMode = MODE_WHOLE;
				}
				else if(xEle == "save")
				{
					nMode = MODE_SAVE;
				}
				else if(xEle == "list")
				{
					nMode = MODE_LIST;
				}
				else if(xEle == "getindex")
				{
					nMode = MODE_GETINDEX;
				}
				else if(xEle == "setindex")
				{
					nMode = MODE_SETINDEX;
				}
				else
				{
					printf("Unsolved command!\n");
					break;
				}
			}
			else
			{
				if(nMode == MODE_LOADMAP &&
					nParams == 0)
				{
					xMap.DestroyMap();
					if(!xMap.LoadMap(xEle.c_str()))
					{
						printf("can't load map [%s]",
							xEle.c_str());
						break;
					}
					else
					{
						printf("load ok!\n");
					}
				}
				else if(nMode == MODE_SAVE)
				{
					if(xMap.SaveMap(xEle.c_str()))
					{
						sprintf(szBuf, "save to %s ok!\n",
							xEle.c_str());
						PrintAndWrite(pfile, szBuf);
					}
				}
				else if(nMode == MODE_GETCOORD)
				{
					if(xMap.IsDataInside())
					{
						int nPtr = 0;
						char szNumber[20];
						szBuf[0] = 0;

						if(xEle[0] == 'o')
						{
							strcpy(szNumber, xEle.c_str() + 1);
							int nLook = atoi(szNumber);

							MapInfo info;
							MapTile* pTile = NULL;
							xMap.GetMapInfo(info);
							for(int i = 0; i < info.nRow; ++i)
							{
								for(int j = 0; j < info.nCol; ++j)
								{
									pTile = xMap.GetCellData(i, j);
									if(pTile)
									{
										if(pTile->bArea == nLook &&
											(pTile->wFrImg & 0x8000) > 0)
										{
											sprintf(szNumber, "%d,%d\n",
												j, i);
											if(strlen(szBuf) < 1000)
											{
												printf(szBuf);
												fprintf(pfile, "%s", szBuf);
												szBuf[0] = 0;
											}
											strcat(szBuf, szNumber);
										}
									}
								}
							}
							if(strlen(szBuf))
							{
								printf(szBuf);
								fprintf(pfile, "%s", szBuf);
								fflush(pfile);
							}
						}
						else if(xEle[0] == 'a')
						{
							MapInfo info;
							MapTile* pTile = NULL;
							xMap.GetMapInfo(info);
							for(int i = 0; i < info.nRow; ++i)
							{
								for(int j = 0; j < info.nCol; ++j)
								{
									pTile = xMap.GetCellData(i, j);
									if(pTile)
									{
										if(pTile->bAniFrame & 0x80)
										{
											sprintf(szBuf, "(%d,%d) AniFrame:%d AniTick:%d\n",
												j, i, pTile->bAniFrame & 0x7F, pTile->bAniTick);
											PrintAndWrite(pfile, szBuf);
										}
									}
								}
							}
							//if(strlen(szBuf))
							{
								//printf(szBuf);
								//fprintf(pfile, "%s", szBuf);
								//fflush(pfile);
							}
						}
						else if(xEle[0] == 'd')
						{
							MapTile* pTile = NULL;
							MapInfo info;
							xMap.GetMapInfo(info);
							for(int i = 0; i < info.nRow; ++i)
							{
								for(int j = 0; j < info.nCol; ++j)
								{
									pTile = xMap.GetCellData(i, j);
									if(pTile)
									{
										if(pTile->bDoorIndex & 0x80)
										{
											sprintf(szBuf, "(%d,%d) DoorIndex:%d DoorOffset:%d\n",
												j, i, pTile->bAniFrame & 0x7F, pTile->bDoorOffset & 0x7F);
											PrintAndWrite(pfile, szBuf);
										}
									}
								}
							}
						}
						else if(xEle[0] == 'b')
						{
							MapTile* pTile = NULL;
							MapInfo info;
							xMap.GetMapInfo(info);

							strcpy(szNumber, xEle.c_str() + 1);
							int nLook = atoi(szNumber);

							for(int i = 0; i < info.nRow; ++i)
							{
								for(int j = 0; j < info.nCol; ++j)
								{
									pTile = xMap.GetCellData(i, j);
									if(pTile)
									{
										if(pTile->wBkImg & 0x8000)
										{
											if(pTile->wBkImg & (~0x8000) == nLook)
											{
												sprintf(szBuf, "(%d,%d) BkImg:%d",
													j, i, pTile->wBkImg & (~0x8000));
												PrintAndWrite(pfile, szBuf);
											}
											
										}
									}
								}
							}
						}
					}
				}
				else if(nMode == MODE_GETDATA)
				{
					szBuf[0] = 0;

					if(0 == sscanf(xEle.c_str(), "%d,%d", &nCoordX, &nCoordY))
					{
						printf("params error\n");
						break;
					}
					else
					{
						MapInfo info;
						xMap.GetMapInfo(info);
						if(xMap.IsDataInside())
						{
							MapTile* pTile = xMap.GetCellData(nCoordY, nCoordX);
							if(pTile)
							{
								sprintf(szBuf, "wBkImg:%d,0x%04x\n",
									pTile->wBkImg, pTile->wBkImg);
								PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "wMidImg:%d,0x%04x\n",
									pTile->wMidImg, pTile->wMidImg);
								PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "wFrImg:%d,0x%04x\n",
									pTile->wFrImg, pTile->wFrImg);
								PrintAndWrite(pfile, szBuf);
								//sprintf(szBuf, "wFrImg:%d,0x%04x\n",
									//pTile->wFrImg, pTile->wFrImg);
								//PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "bDoorIndex:%d,0x%02x\n",
									pTile->bDoorIndex, pTile->bDoorIndex);
								PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "bDoorOffset:%d,0x%02x\n",
									pTile->bDoorOffset, pTile->bDoorOffset);
								PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "bAniFrame:%d,0x%02x\n",
									pTile->bAniFrame, pTile->bAniFrame);
								PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "bAniTick:%d,0x%02x\n",
									pTile->bAniTick, pTile->bAniTick);
								PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "bArea:%d,0x%02x\n",
									pTile->bArea, pTile->bArea);
								PrintAndWrite(pfile, szBuf);
								sprintf(szBuf, "bLight:%d,0x%02x\n",
									pTile->bLight, pTile->bLight);
								PrintAndWrite(pfile, szBuf);
							}
						}
					}
				}
				else if(nMode == MODE_WHOLE)
				{
					MapInfo info;
					xMap.GetMapInfo(info);
					MapTile* pTile = NULL;

					if(xEle[0] == 'o')
					{
						int nNumber = 0;
						nNumber = atoi(xEle.c_str() + 1);

						for(int i = 0; i < info.nRow; ++i)
						{
							for(int j = 0; j < info.nCol; ++j)
							{
								pTile = xMap.GetCellData(i, j);
								if(pTile)
								{
									pTile->bArea = nNumber;
								}
							}
						}

						sprintf(szBuf, "set the whole tile to area %d ok!\n",
							nNumber);
						PrintAndWrite(pfile, szBuf);
					}
				}
				else if(nMode == MODE_LIST)
				{
					MapInfo info;
					xMap.GetMapInfo(info);
					MapTile* pTile = NULL;
					szBuf[0] = 0;
					std::set<int> xSets;

					if(xEle[0] == 'o')
					{
						int nNumber = 0;
						nNumber = atoi(xEle.c_str() + 1);
						sprintf(szBuf, "list o %d:\n", nNumber);

						for(int i = 0; i < info.nRow; ++i)
						{
							for(int j = 0; j < info.nCol; ++j)
							{
								pTile = xMap.GetCellData(i, j);

								if((pTile->wFrImg & (~0x8000)) > 0 &&
									pTile->bArea == nNumber)
								{
									int nValue = (pTile->wFrImg & (~0x8000));
									if((pTile->bAniFrame & 0x80) > 0)
									{
										int nFrBase = nValue;
										for(int k = 0; k < (pTile->bAniFrame & (~0x80)); ++k)
										{
											nValue = nFrBase + k;
											--nValue;
											xSets.insert(nValue);
										}
									}
									else
									{
										--nValue;
										xSets.insert(nValue);
									}
								}
							}
						}

						PrintAndWrite(pfile, szBuf);

						for(std::set<int>::const_iterator begiter = xSets.begin();
							begiter != xSets.end();
							++begiter)
						{
							sprintf(szBuf, "%d\n", *begiter);
							PrintAndWrite(pfile, szBuf);
						}
					}
					else if(xEle[0] == 'b')
					{
						int nValue = 0;

						sprintf(szBuf, "list b:\n");

						for(int i = 0; i < info.nRow; ++i)
						{
							for(int j = 0; j < info.nCol; ++j)
							{
								pTile = xMap.GetCellData(i, j);

								if((pTile->wBkImg & (~0x8000)) > 0)
								{
									int nValue = (pTile->wBkImg & (~0x8000)) - 1;
									if(nValue > 30000)
									{
										//assert("b" == NULL);
									}
									xSets.insert(nValue);
								}
							}
						}

						PrintAndWrite(pfile, szBuf);

						for(std::set<int>::const_iterator begiter = xSets.begin();
							begiter != xSets.end();
							++begiter)
						{
							sprintf(szBuf, "%d\n", *begiter);
							PrintAndWrite(pfile, szBuf);
						}
					}
					else if(xEle[0] == 'm')
					{
						int nValue = 0;

						sprintf(szBuf, "list m:\n");

						for(int i = 0; i < info.nRow; ++i)
						{
							for(int j = 0; j < info.nCol; ++j)
							{
								pTile = xMap.GetCellData(i, j);

								if((pTile->wMidImg & (~0x8000)) > 0)
								{
									int nValue = (pTile->wMidImg & (~0x8000)) - 1;
									xSets.insert(nValue);
								}
							}
						}

						PrintAndWrite(pfile, szBuf);

						for(std::set<int>::const_iterator begiter = xSets.begin();
							begiter != xSets.end();
							++begiter)
						{
							sprintf(szBuf, "%d\n", *begiter);
							PrintAndWrite(pfile, szBuf);
						}
					}
				}
				else if(nMode == MODE_GETINDEX)
				{
					sprintf(szBuf, "resource index:%d\n", xMap.GetResIndex());
					PrintAndWrite(pfile, szBuf);
				}
				else if(nMode == MODE_SETINDEX)
				{
					if(0 == nParams)
					{
						int nResIndex = atoi(xEle.c_str());
						xMap.SetResIndex(nResIndex);
						sprintf(szBuf, "set resource index to %d ok!\n", nResIndex);
						PrintAndWrite(pfile, szBuf);
					}
				}
				else if(nMode == MODE_SETDATA)
				{
					if(0 == nParams)
					{
						if(0 == sscanf(xEle.c_str(), "%d,%d", &nCoordX, &nCoordY))
						{
							printf("params error\n");
							break;
						}
						else
						{
							++nParams;
						}
					}
					else if(1 == nParams)
					{
						if(xEle[0] == 'o')
						{
							int nNumber = 0;
							nNumber = atoi(xEle.c_str() + 1);

							MapTile* pTile = xMap.GetCellData(nCoordY, nCoordX);
							if(pTile != NULL)
							{
								pTile->bArea = nNumber;
								sprintf(szBuf, "object set to %d ok!\n",
									nNumber);
								PrintAndWrite(pfile, szBuf);
							}
						}
						else if(xEle[0] == 'b')
						{
							int nNumber = 0;
							nNumber = atoi(xEle.c_str() + 1);

							MapTile* pTile = xMap.GetCellData(nCoordY, nCoordX);
							if(pTile != NULL)
							{
								pTile->wBkImg = nNumber;
								sprintf(szBuf, "bkimg set to %d ok!\n",
									nNumber);
								PrintAndWrite(pfile, szBuf);
							}
						}
						else if(xEle[0] == 'm')
						{
							int nNumber = 0;
							nNumber = atoi(xEle.c_str() + 1);

							MapTile* pTile = xMap.GetCellData(nCoordY, nCoordX);
							if(pTile != NULL)
							{
								pTile->wMidImg = nNumber;
								sprintf(szBuf, "midimg set to %d ok!\n",
									nNumber);
								PrintAndWrite(pfile, szBuf);
							}
						}
						else if(xEle[0] == 'f')
						{
							int nNumber = 0;
							nNumber = atoi(xEle.c_str() + 1);

							MapTile* pTile = xMap.GetCellData(nCoordY, nCoordX);
							if(pTile != NULL)
							{
								pTile->wFrImg = nNumber;
								sprintf(szBuf, "frimg set to %d ok!\n",
									nNumber);
								PrintAndWrite(pfile, szBuf);
							}
						}
						else if(xEle[0] == 'a')
						{
							int nNumber = 0;
							nNumber = atoi(xEle.c_str() + 1);

							MapTile* pTile = xMap.GetCellData(nCoordY, nCoordX);
							if(pTile != NULL)
							{
								pTile->bAniFrame = nNumber;
								sprintf(szBuf, "aniframe set to %d ok!\n",
									nNumber);
								PrintAndWrite(pfile, szBuf);
							}
						}
						else if(xEle[0] == 'd')
						{
							int nNumber = 0;
							nNumber = atoi(xEle.c_str() + 1);

							MapTile* pTile = xMap.GetCellData(nCoordY, nCoordX);
							if(pTile != NULL)
							{
								pTile->bDoorIndex = nNumber;
								sprintf(szBuf, "aniframe set to %d ok!\n",
									nNumber);
								PrintAndWrite(pfile, szBuf);
							}
						}
					}
				}
			}
		}

		nMode = 0;
		nParams = 0;
		fflush(pfile);
	}

	fclose(pfile);
}
//////////////////////////////////////////////////////////////////////////
void PrintAndWrite(FILE* _pfile, const char* _pbuf)
{
	fprintf(_pfile, _pbuf);
	printf(_pbuf);
}
//////////////////////////////////////////////////////////////////////////