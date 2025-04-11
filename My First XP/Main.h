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
void XPCALL TestNewFunctions(void);
void XPCALL ExportWalls(void);
void XPCALL AppendSceneName(void);
void XPCALL ExportRect(void);
void SaveTextToFile(const std::string& filename, const std::string& text);
void SetParameters(void);
void FindGridExt(void);
pENTREC XPCALL FindGridEntities(hDLIST list, pENTREC entity, const DWORD parm1, DWORD parm2);
std::string GetSaveFilePath();
std::string GetWallsJSON(void);
std::string GetWallText(const int, const int, const int, const int);
std::string random_string(std::size_t length);

extern char CList[];
extern PCMDPROC PList[];
extern XP MyXP;
