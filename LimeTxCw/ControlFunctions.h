#pragma once
#include "CmdParameter.h"

//
// prototypes
//
int handle_About();
int handle_Syscmd(const CCmdParameter& cPara);
int handle_Help(const std::string s);
int handle_Devid(const std::string s);
int handle_Init();
int handle_Reset();
int handle_GetChipTemperature(const std::string s);
int handle_EnableChannel(const std::string s);
int handle_SetAntenna(const std::string s);
int handle_GetAntenna(const std::string s);
int handle_SetLOFrequency(const std::string s);
int handle_GetLOFrequency(const std::string s);
int handle_SetGaindB(const std::string s);
int handle_GetGaindB(const std::string s);
int handle_SetSampleRate(const std::string s);
int handle_GetSampleRate(const std::string s);
int handle_LoadConfig(const std::string s);
int handle_SaveConfig(const std::string s);
int handle_Synchronize(const std::string s);
int handle_SetClockFreq(const std::string s);
int handle_GetClockFreq(const std::string s);
int handle_VCTCXORead(const std::string s);
int handle_VCTCXOWrite(const std::string s);
int handle_Close();
