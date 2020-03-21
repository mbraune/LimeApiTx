// ControlFunctions.cpp
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "LimeTxCw.h"
#include "lime/LimeSuite.h"
#include "ControlFunctions.h"


using namespace std;

// print cmd_ok and return value
int ret_cmdok(int i) {
    if (i == 0) cout << "cmd_ok" << endl;
    else        cout << "err_1001" << endl;
    return i;
}

//############################################
// handle_cmd  functions

// execute system command , e.g.
//      syscmd=cls
//      or syscmd=ls calls system("ls")
int handle_Syscmd(const CCmdParameter& cPara) 
{
    int res;
    string s1 = cPara.m_stPara[0].valString;
    string s2 = "\"" + s1+ "\"";
    res = system(s2.c_str());
    return ret_cmdok(res);
}

int handle_About()
{
    cout << "\tversion " << LIMETXCW_VERSION << endl;
    return ret_cmdok(0);
}

int handle_Help(const CCmdParameter& cPara)
{
    // if s empty
    if (cPara.m_stPara[0].valString.empty())
        show_usage();
    else
        cout << "\thelp cmd todo \n";
    return ret_cmdok(0);
}

int handle_Devid()
{
    stringstream ss;
    const lms_dev_info_t* devInfo;
    devInfo = LMS_GetDeviceInfo(device);
    ss << "\t" << hex << uppercase << devInfo->boardSerialNumber << endl;
    cout << ss.str();
    return ret_cmdok(0);
}

int handle_Init()
{
    int res = LMS_Init(device);
    return ret_cmdok(res);
}

int handle_Reset()
{
    int res = LMS_Reset(device);
    return ret_cmdok(res);
}

int handle_GetChipTemperature(const CCmdParameter& cPara)
{
    int res = -1;
    size_t chipindex = 0;  //
    float_type temp;
    res = LMS_GetChipTemperature(device, chipindex, &temp);
    cout << "\t" << temp << endl;
    return ret_cmdok(res);
}

int handle_EnableChannel(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx    = (cPara.m_stPara[0].valInt == 1);
    size_t ch   = cPara.m_stPara[1].valInt;
    bool bEn    = (cPara.m_stPara[2].valInt == 1);
    res = LMS_EnableChannel(device, bTx, ch, bEn);
    return ret_cmdok(res);
}

int handle_SetAntenna(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx   = (cPara.m_stPara[0].valInt == 1);
    size_t ch  = cPara.m_stPara[1].valInt;
    size_t idx = cPara.m_stPara[2].valInt;
    res = LMS_SetAntenna(device, bTx, ch, idx);
    return ret_cmdok(res);
}

int handle_GetAntenna(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx = (cPara.m_stPara[0].valInt == 1);
    size_t ch = cPara.m_stPara[1].valInt;
    size_t idx = LMS_GetAntenna(device, bTx, ch);
    cout << "\t" << idx << endl;
    if (idx != -1) res = 0;
    return ret_cmdok(res);
}

int handle_SetLOFrequency(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx     = (cPara.m_stPara[0].valInt == 1);
    size_t ch    = cPara.m_stPara[1].valInt;
    float_type f = cPara.m_stPara[2].valFloat;
    res = LMS_SetLOFrequency(device, bTx, ch, f);
    return ret_cmdok(res);
}

int handle_GetLOFrequency(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx = (cPara.m_stPara[0].valInt == 1);
    size_t ch = cPara.m_stPara[1].valInt;
    float_type f;
    res = LMS_GetLOFrequency(device, bTx, ch, &f);
    cout << "\t" << f << endl;
    return ret_cmdok(res);
}

int handle_SetGaindB(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx    = (cPara.m_stPara[0].valInt == 1);
    size_t ch   = cPara.m_stPara[1].valInt;
    unsigned gn = cPara.m_stPara[2].valInt;
    res = LMS_SetGaindB(device, bTx, ch, gn);
    return ret_cmdok(res);
}

int handle_GetGaindB(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx    = (cPara.m_stPara[0].valInt == 1);
    size_t ch   = cPara.m_stPara[1].valInt;
    unsigned gn = 0;
    res = LMS_GetGaindB(device, bTx, ch, &gn);
    cout << "\t" << gn << endl;
    return ret_cmdok(res);
}

int handle_SetSampleRate(const CCmdParameter& cPara)
{
    int res = -1;
    float_type rate = cPara.m_stPara[0].valFloat;
    size_t over     = cPara.m_stPara[1].valInt;
    res = LMS_SetSampleRate(device, rate, over);
    return ret_cmdok(res);
}

int handle_GetSampleRate(const CCmdParameter& cPara)
{
    int res = -1;
    bool bTx = (cPara.m_stPara[0].valInt == 1);
    size_t ch = cPara.m_stPara[1].valInt;

    float_type f_host;
    float_type f_rf;
    res = LMS_GetSampleRate(device, bTx, ch, &f_host, &f_rf);
    cout << "\t" << f_host << endl;
    cout << "\t" << f_rf << endl;
    return ret_cmdok(res);
}

int handle_LoadConfig(const CCmdParameter& cPara)
{
    return 0;
}

int handle_SaveConfig(const CCmdParameter& cPara)
{
    return 0;
}

int handle_Synchronize(const CCmdParameter& cPara)
{
    return 0;
}

int handle_SetClockFreq(const CCmdParameter& cPara)
{
    int res = -1;
    size_t clkid = cPara.m_stPara[0].valInt;
    float_type f = cPara.m_stPara[1].valFloat;

/*
    if (get_para1(s) == -1) {
        cout << "\tusage  clock_frq,clkid=[MHz]" << endl;
        cout << "\twhere \tclkid 0 = chip ref clock" << endl;
        cout << "\t\tclkid 1 = LMS_CLOCK_SXR" << endl << "\t\tclkid 2 = LMS_CLOCK_SXT" << endl;
        return ret_cmdok(res);
    }*/

    LMS_SetClockFreq(device, clkid, f);
    return ret_cmdok(res);
}

int handle_GetClockFreq(const CCmdParameter& cPara)
{
    int res = -1;
    size_t clkid = cPara.m_stPara[0].valInt;
    float_type f;
    res = LMS_GetClockFreq(device, clkid, &f);
    stringstream ss;
    ss << "\t" << fixed << setprecision(6) << f / 1e6 << " MHz\n";
    cout << ss.str();
    return ret_cmdok(res);
}

int handle_VCTCXORead(const CCmdParameter& cPara)
{
    int res = -1;
    uint16_t val = 0;
    res = LMS_VCTCXORead(device, &val);
    cout << "\t" << val << endl;
    return ret_cmdok(res);
}

int handle_VCTCXOWrite(const CCmdParameter& cPara)
{
    return 0;
}


int handle_Close() {
    int res = LMS_Close(device);
    return ret_cmdok(res);
}