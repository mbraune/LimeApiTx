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

// return if cmd contains "=?"
bool is_query(const string cmd) {
    return (cmd.find("=?") != string::npos);
}

// extract cmd para1 , if no digit return -1
int get_digit1(const string cmd) {
    int res = -1;
    size_t pos = cmd.find(",");
    if (pos != string::npos)
        if (isdigit(cmd[pos+1]))
            res = (int)(cmd[pos+1]) - 48;

    return res;
}

// extract cmd for double Para
float_type get_doublePara(const string cmd) {
    float_type dVal = -9999.9;
    size_t pos = cmd.find("=");
    string s2 = cmd.substr(pos + 1);
    if (s2.empty() || (s2.find_first_of("0123456789") == string::npos))
        return dVal;
    else 
        return stod(s2) * 1e6;
}

// extract cmd for unsigned int Para
unsigned get_uintPara(const string cmd) {
    unsigned iVal = 0;
    size_t pos = cmd.find("=");
    string s2 = cmd.substr(pos + 1);
    if (s2.empty() || (s2.find_first_of("0123456789") == string::npos))
        return iVal;
    else
        return stoi(s2);
}


// print cmd_ok and return value
int ret_cmdok(int i) {
    cout << "cmd_ok" << endl;
    return i;
}

//############################################
// handle_cmd  functions

// execute system command , e.g.
//      syscmd=cls
//      or syscmd=ls calls system("ls")
int handle_syscmd(const std::string s) {
    int res;
    string s1,s2;
    size_t pos = s.find("=");
    s1 = s.substr(pos + 1);
    s2 = "\"" + s1+ "\"";
    res = system(s2.c_str());
    return ret_cmdok(res);
}

int handle_help()
{
    show_usage();
    return ret_cmdok(0);
}

int handle_antenna(const std::string s)
{
    return 0;
}

int handle_ant_bw(const std::string s)
{
    return 0;
}

int handle_temp(const string s) {
    int res = -1;
    if (is_query(s)) {
        float_type temp;
        res = LMS_GetChipTemperature(device, 0, &temp);
        cout << "\t" << temp << endl;
    }
    else {
        cout << "\tusage:  temp=?" << endl;
    }
    return ret_cmdok(res);
}

int handle_devid(const string s) {
    // only query valid
    stringstream ss;
    const lms_dev_info_t* devInfo;
    devInfo = LMS_GetDeviceInfo(device);
    ss << "\t" << hex << uppercase << devInfo->boardSerialNumber << endl;
    cout << ss.str();
    return ret_cmdok(0);
}

int handle_freq(const std::string s)
{
    int res=-1;
    float_type f;
    if (is_query(s)) {
        res = LMS_GetLOFrequency(device, LMS_CH_TX, 0, &f);
        cout << "\t" << f << endl;
    }
    else {
        f = get_doublePara(s);
        if (f > 0.0)
            res = LMS_SetLOFrequency(device, LMS_CH_TX, 0, f);
        else
            cout << "\tusage  freq=[MHz]" << endl;
    }
    return ret_cmdok(res);
}

int handle_gain_db(const std::string s)
{
    int res = -1;
    unsigned mygain = 0;
    if (is_query(s)) {
        res = LMS_GetGaindB(device, LMS_CH_TX, 0, &mygain);
        cout << "\t" << mygain << endl;
    }
    else {
        mygain = get_uintPara(s);
        res = LMS_SetGaindB(device, LMS_CH_TX, 0, mygain);
    }
    return ret_cmdok(res);
}

int handle_enab_chan(const string s) {
    // only write valid, 2 parameter + on/off required
    // e.g. enab_chan,1,0=1
    //      enab_chan,1,0=0
    std::size_t pos = s.find("=");
    std::string str3 = s.substr(pos + 1);
    bool bCmd = stoi(str3);
    //std::string delimiter = ",";
    int res = LMS_EnableChannel(device, LMS_CH_TX, 0, bCmd);
    return ret_cmdok(res);
}

int handle_samp_rate(const std::string s)
{
    return 0;
}

int handle_loadcnf(const std::string s)
{
    return 0;
}

int handle_savecnf(const std::string s)
{
    return 0;
}

int handle_clock_frq(const std::string s)
{
    int res = -1;
    size_t clkid =LMS_CLOCK_REF;

    if (get_digit1(s) == -1){
        cout << "\tusage  clock_frq,clkid=?    or clock_frq,clkid=[MHz]" << endl;
        cout << "\twhere \tclkid 0 = chip ref clock" << endl; 
        cout << "\t\tclkid 1 = LMS_CLOCK_SXR" << endl  << "\t\tclkid 2 = LMS_CLOCK_SXT" << endl;
        return ret_cmdok(res);
    }

    float_type clock;
    clkid = get_digit1(s);
    if (is_query(s)){
        res = LMS_GetClockFreq(device, clkid, &clock);
        stringstream ss;
        ss << "\tclk_ref = " << fixed << setprecision(6) << clock / 1e6 << " MHz\n";
        cout << ss.str();
    }
    else {
        clock = get_doublePara(s);
        if (clock > 0.0)
            LMS_SetClockFreq(device, clkid, clock);
    }
    return ret_cmdok(res);
}

int handle_reset() {
    int res = LMS_Reset(device);
    return ret_cmdok(res);
}

int handle_close() {
    int res = LMS_Close(device);
    return ret_cmdok(res);
}