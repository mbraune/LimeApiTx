// ControlFunctions.cpp
//

#include <iostream>
#include <sstream>
#include "LimeTxCw.h"
#include "lime/LimeSuite.h"
#include "ControlFunctions.h"


using namespace std;

// helper
// cmd including "=?" is query
bool is_query(const string cmd) {
    return (cmd.find("=?") != string::npos);
}


//############################################
// handle_cmd  functions
//  --first check if only query is allowed, 
//  --todo parse parameter


// execute system command 
//e.g. syscmd=cls
// syscmd=ls calls system("ls")
int handle_syscmd(string s) {
    int res;
    string ss;
    s.erase(0, 7);
    ss = "\"" + s + "\"";
    res = std::system(ss.c_str());
    cout << "cmd_ok\n";
    return res;
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
    // only query valid
    int res;
    float_type temp;
    res = LMS_GetChipTemperature(device, 0, &temp);
    cout << "\t" << temp << endl << "cmd_ok" << endl;
    return res;
}

int handle_devid(const string s) {
    // only query valid
    stringstream ss;
    const lms_dev_info_t* devInfo;
    devInfo = LMS_GetDeviceInfo(device);
    ss << "\t" << hex << uppercase << devInfo->boardSerialNumber << endl << "cmd_ok" << endl;
    cout << ss.str();
    return 0;
}

int handle_freq(const std::string s)
{
    int res;
    if (is_query(s)) {
        float_type f;
        res = LMS_GetLOFrequency(device, LMS_CH_TX, 0, &f);
        cout << "\t" << f << endl;
    }
    else {
        std::size_t pos = s.find("=");
        std::string s2 = s.substr(pos + 1);
        if (s2.empty() || (s2.find_first_of("0123456789") == string::npos))
            cout << "\tusage  freq=[MHz]" << endl;
        else {
            float_type freq1 = stod(s2) * 1e6;
            res = LMS_SetLOFrequency(device, LMS_CH_TX, 0, freq1);
        }
    }
    cout << "cmd_ok\n";
    return res;
}

int handle_gain_db(const std::string s)
{
    return 0;
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
    cout << "cmd_ok" << endl;
    return res;
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
    return 0;
}

int handle_reset() {
    int res = LMS_Reset(device);
    cout << "cmd_ok" << endl;
    return res;
}

int handle_close() {
    int res = LMS_Close(device);
    cout << "cmd_ok" << endl;
    return res;
}