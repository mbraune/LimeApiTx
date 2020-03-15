// ControlFunctions.cpp
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "LimeTxCw.h"
#include "lime/LimeSuite.h"
#include "ControlFunctions.h"


using namespace std;

// helper functions, used only local
//

// extract cmd para1 , if no digit return -1
int get_para1(const string cmd) {
    int res = -1;
    size_t pos = cmd.find(",");
    if (pos != string::npos)
        if (isdigit(cmd[pos+1]))
            res = (int)(cmd[pos+1]) - 48;

    return res;
}

int get_para2(const string cmd) {
    int res = -1;
    size_t pos = cmd.find(",");
    pos = cmd.find(",", pos + 1);
    if (pos != string::npos)
        if (isdigit(cmd[pos + 1]))
            res = (int)(cmd[pos + 1]) - 48;

    return res;
}

// extract cmd for double Para behind '='
float_type get_doubleVal(const string cmd) {
    float_type dVal = -9999.9;
    size_t pos = cmd.find("=");
    string s2 = cmd.substr(pos + 1);
    if (s2.empty() || (s2.find_first_of("0123456789") == string::npos))
        return dVal;
    else 
        return stod(s2) * 1e6;
}

// extract cmd for unsigned int Para behind '='
int get_intVal(const string cmd) {
    int iVal = -1;
    size_t pos = cmd.find("=");
    if (pos != string::npos) {
        string s2 = cmd.substr(pos + 1);
        if (!s2.empty() && (s2.find_first_of("0123456789") != string::npos))
            return stoi(s2);
    }
    return iVal;
}


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
int handle_Syscmd(const std::string s) {
    int res;
    string s1,s2;
    size_t pos = s.find("=");
    s1 = s.substr(pos + 1);
    s2 = "\"" + s1+ "\"";
    res = system(s2.c_str());
    return ret_cmdok(res);
}

int handle_About()
{
    cout << "\tversion " << LIMETXCW_VERSION << endl;
    return ret_cmdok(0);
}

int handle_Help(const std::string s)
{
    // if s empty
    if (s.empty())
        show_usage();
    else
        cout << "\thelp cmd todo \n";
    return ret_cmdok(0);
}

int handle_Devid(const std::string s)
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

int handle_GetChipTemperature(const std::string s)
{
    int res = -1;
    float_type temp;
    res = LMS_GetChipTemperature(device, 0, &temp);
    cout << "\t" << temp << endl;
    return ret_cmdok(res);
}

int handle_EnableChannel(const std::string s)
{
    int res = -1;
    int enab = get_intVal(s);
    if(enab != -1)
        res = LMS_EnableChannel(device, LMS_CH_TX, 0, enab);
    return ret_cmdok(res);
}

int handle_SetAntenna(const std::string s)
{
    return 0;
}

int handle_GetAntenna(const std::string s)
{
    return 0;
}

int handle_SetLOFrequency(const std::string s)
{
    int res = -1;
    float_type f;

    f = get_doubleVal(s);
    if (f > 0.0)
        res = LMS_SetLOFrequency(device, LMS_CH_TX, 0, f);
    else
        cout << "\tusage  SetLOFrequency,bTx,ch=[MHz]" << endl;

    return ret_cmdok(res);
}

int handle_GetLOFrequency(const std::string s)
{
    int res = -1;
    float_type f;
    res = LMS_GetLOFrequency(device, LMS_CH_TX, 0, &f);
    cout << "\t" << f << endl;
    return ret_cmdok(res);
}

int handle_SetGaindB(const std::string s)
{
    int res = -1;
    unsigned mygain = get_intVal(s);
    res = LMS_SetGaindB(device, LMS_CH_TX, 0, mygain);
    return ret_cmdok(res);
}

int handle_GetGaindB(const std::string s)
{
    int res = -1;
    unsigned mygain = 0;
    res = LMS_GetGaindB(device, LMS_CH_TX, 0, &mygain);
    cout << "\t" << mygain << endl;
    return ret_cmdok(res);
}

int handle_SetSampleRate(const std::string s)
{
    return 0;
}

int handle_GetSampleRate(const std::string s)
{
    return 0;
}

int handle_LoadConfig(const std::string s)
{
    return 0;
}

int handle_SaveConfig(const std::string s)
{
    return 0;
}

int handle_Synchronize(const std::string s)
{
    return 0;
}

int handle_SetClockFreq(const std::string s)
{
    int res = -1;
    size_t clkid = LMS_CLOCK_REF;

    if (get_para1(s) == -1) {
        cout << "\tusage  clock_frq,clkid=[MHz]" << endl;
        cout << "\twhere \tclkid 0 = chip ref clock" << endl;
        cout << "\t\tclkid 1 = LMS_CLOCK_SXR" << endl << "\t\tclkid 2 = LMS_CLOCK_SXT" << endl;
        return ret_cmdok(res);
    }

    float_type clock;
    clkid = get_para1(s);
    clock = get_doubleVal(s);
    if (clock > 0.0)
        LMS_SetClockFreq(device, clkid, clock);
    return ret_cmdok(res);
}

int handle_GetClockFreq(const std::string s)
{
    int res = -1;
    size_t clkid = LMS_CLOCK_REF;

    if (get_para1(s) == -1) {
        cout << "\tusage  clock_frq,clkid=?" << endl;
        cout << "\twhere \tclkid 0 = LMS_CLOCK_REF" << endl;
        cout << "\t\tclkid 1 = LMS_CLOCK_SXR" << endl << "\t\tclkid 2 = LMS_CLOCK_SXT" << endl;
        return ret_cmdok(res);
    }

    float_type clock;
    clkid = get_para1(s);

    res = LMS_GetClockFreq(device, clkid, &clock);
    stringstream ss;
    ss << "\t" << fixed << setprecision(6) << clock / 1e6 << " MHz\n";
    cout << ss.str();
    return ret_cmdok(res);
}

int handle_VCTCXORead(const std::string s)
{
    int res = -1;
    uint16_t val = 0;
    res = LMS_VCTCXORead(device, &val);
    cout << "\t" << val << endl;
    return ret_cmdok(res);
}

int handle_VCTCXOWrite(const std::string s)
{
    return 0;
}


int handle_Close() {
    int res = LMS_Close(device);
    return ret_cmdok(res);
}