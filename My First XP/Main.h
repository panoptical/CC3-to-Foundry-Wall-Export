#pragma once
#include <windows.h>
#include <fstream> // For file operations
#include <string>  // For std::string
#include <random>
#include <XT_Entities.h>
#include "XP.H"

#define XPID 0xF000

void XPCALL About(void);
void XPCALL Demo(void);
void XPCALL ToFoundry(void);
void XPCALL TestNewFunctions(void);
void XPCALL ExportWalls(void);
void XPCALL AppendSceneName(void);
void XPCALL ExportRect(void);
void XPCALL FindGridExt();
void XPCALL HandleExc(const int Result, int Result2, int Result3);
void XPCALL GetBB(void);
void XPCALL MSGBB(const int Result, int Result2, int Result3);
void SaveTextToFile(const std::string& filename, const std::string& text);
void SetParameters(void);
void ProcessWallEntity(const pENTREC entity);
void MakeWall(const pENTREC entity, int node1, int node2);
pENTREC XPCALL FindGridEntities(hDLIST list, pENTREC entity, const DWORD parm1, DWORD parm2);
pENTREC XPCALL ProcessWall(hDLIST list, const pENTREC entity, const DWORD parm1, DWORD parm2);
std::string GetSaveFilePath();
void GetWallsJSON(void);
std::string GetWallText(const int, const int, const int, const int);
std::string random_string(std::size_t length);

extern char CList[];
extern PCMDPROC PList[];
extern XP MyXP;
