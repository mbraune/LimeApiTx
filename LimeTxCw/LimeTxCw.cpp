// LimeTxCw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "lime/LimeSuite.h"

enum Testcases {
#   define TC(a) a,
#   include "testcases.h"
#   undef TC
    TcCount
};

char const* const testcases_str[] = {
#   define TC(a) #a,
#   include "testcases.h"
#   undef TC
    0
};


using namespace std;

static lms_device_t* device = NULL;
static unsigned int gaindB = 0;
static float_type  dFreq = 1.1;

int device_error()
{
    if (device != NULL) {
        cout << "ERROR: " << LMS_GetLastErrorMessage() << endl;
        cout << "err_1001\n";   // device error
        LMS_Close(device);
    }
    exit(-1);
}

void show_usage()
{
    cout << "LimeTxCw  v0.205" << endl;
    cout << "\tread cmds     :  help , info , stat , temp=?, " << endl;
    cout << "\tctrl cmds     :  txon , txoff , gain=[0..70], freq=[MHz] " << endl;
    cout << "\tmisc cmd      :  clr " << endl;
    cout << "\texit          :  stop " << endl;
    cout << "------------------------------------------------------------ " << endl;
    //cout << "cmd_ok\n";
}

void show_info()
{
    stringstream ss;   // temporary stringstream for formatted output (avoid formatting iostream )

    const lms_dev_info_t* devInfo;
    devInfo = LMS_GetDeviceInfo(device);

    ss << "\t" << devInfo->deviceName << "   serial " << hex << uppercase << devInfo->boardSerialNumber << endl;
    cout << ss.str();
    cout << "\thw version " << devInfo->hardwareVersion << "   fw version " << devInfo->firmwareVersion << endl;
    //cout << "cmd_ok\n";
}

void show_status()
{
    stringstream ss;     // temporary stringstream for formatted output (avoid formatting iostream )

    float_type clock;
    LMS_GetClockFreq(device, LMS_CLOCK_REF, &clock);
    ss << "\tclk_ref = " << fixed << setprecision(6) << clock/1e6 << " MHz\n";

    //LMS_GetClockFreq(device, LMS_CLOCK_SXR , &clock);
    //ss << "\tclk_sxt = " << fixed << setprecision(6) << clock / 1e6 << " MHz\n";


    float_type myfreq;
    LMS_GetLOFrequency(device, LMS_CH_TX, 0, &myfreq);
    ss << "\tfreq    = " << defaultfloat << setprecision(8) << myfreq/1e6 << " MHz  \t";

    unsigned int mygain;
    LMS_GetGaindB(device, LMS_CH_TX, 0, &mygain);
    ss << "   gain = " << mygain << " dB" << endl;

    cout << ss.str();
}

//
// convert string to Testcase enum
// if not found return none
Testcases getTestcase(string s) {
    int i; 
    for (i = 0; i < TcCount; i++) {
        if (s.find(testcases_str[Testcases(i)]) != string::npos)
            return static_cast<Testcases>(i);
    }
    return none;
}

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
void handle_syscmd(string s) {
    string ss;
    s.erase(0, 7);
    ss = "\"" + s + "\"";
    std::system(ss.c_str());
    cout << "cmd_ok\n";
}

void handle_temp(const string s){
    // only query valid
    float_type temp;
    if (LMS_GetChipTemperature(device, 0, &temp) < 0)
        device_error();
     cout << "\t" << temp << endl << "cmd_ok" << endl;
}

void handle_devid(const string s) {
    // only query valid
    stringstream ss;
    const lms_dev_info_t* devInfo;
    devInfo = LMS_GetDeviceInfo(device);
    ss << "\t" << hex << uppercase << devInfo->boardSerialNumber << endl  << "cmd_ok" << endl;
    cout << ss.str();
}

void handle_enab_chan(const string s) {
    // only write valid, 2 parameter + on/off required
    // e.g. enab_chan,1,0=1
    //      enab_chan,1,0=0
    std::size_t pos = s.find("=");
    std::string str3 = s.substr(pos+1);
    bool bCmd = stoi(str3);
    //std::string delimiter = ",";
    LMS_EnableChannel(device, LMS_CH_TX, 0, bCmd);
    cout << "cmd_ok" << endl;
}

void handle_reset() {
    LMS_Reset(device);
    cout << "cmd_ok" << endl;
}

void handle_close() {
    LMS_Close(device);
    cout << "cmd_ok" << endl;
}

// ##########################################################

int main(int argc, char** argv)
{
    show_usage();

    // initial values
    dFreq = 1575.42e6;
    gaindB = 20;

    int n = LMS_GetDeviceList(nullptr);
    if (n > 0)
    {
        // initialization, 
        // print some info
        if (LMS_Open(&device, NULL, NULL) != 0) //open first device
            device_error();

        show_info();

        if (LMS_Init(device) != 0)
            device_error();

        if (LMS_EnableChannel(device, LMS_CH_TX, 0, true) != 0)
            device_error();

        if (LMS_SetLOFrequency(device, LMS_CH_TX, 0, dFreq) != 0)
            device_error();

        if (LMS_SetGaindB(device, LMS_CH_TX, 0, gaindB) != 0)
            device_error();

        if (LMS_SetAntenna(device, LMS_CH_TX, 0, LMS_PATH_AUTO) != 0)
            device_error();

        show_status();

        cout << "cmd_ok\n";

        string cmd;
        while (getline(cin, cmd))
        {
            cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());

            // valid cmd ?
            Testcases eTC = getTestcase(cmd);
            if (eTC == none) {
                cout << "err_1000\n";
                break;
            }

            // else parse parameter
            switch (eTC) {
                case syscmd:    handle_syscmd(cmd);     break;
                case help:      show_usage();           break;
                case antenna:   show_status();          break;
                case temp:      handle_temp(cmd);       break;
                case devid:     handle_devid(cmd);      break;

                case enab_chan: handle_enab_chan(cmd);  break;
                case close:     handle_close();         break;
                case reset:     handle_reset();         break;

                default: break;
            }

            if (cmd == "txon") {
                LMS_EnableChannel(device, LMS_CH_TX, 0, true);
                show_status();
            }
            else if (cmd == "txoff") {
                LMS_EnableChannel(device, LMS_CH_TX, 0, false);
                cout << "cmd_ok\n";
            }
            else if (cmd.find("gain=") != string::npos) {
                cmd.erase(0,5);
                if (cmd.empty() || (cmd.find_first_of("0123456789") == string::npos))
                    cout << "\tusage  gain=[0..70]" << endl;
                else
                    LMS_SetGaindB(device, LMS_CH_TX, 0, stoi(cmd));
                    cout << "cmd_ok\n";
            }
            else if (cmd.find("freq=") != string::npos) {
                cmd.erase(0,5);
                if (cmd.empty() || (cmd.find_first_of("0123456789") == string::npos))
                    cout << "\tusage  freq=[MHz]" << endl;
                else {
                    float_type freq1 = stod(cmd) * 1e6;
                    if (LMS_SetLOFrequency(device, LMS_CH_TX, 0, freq1) != 0)
                        device_error();
                    else cout << "cmd_ok\n";
                }
            }

        }
    }
    else {
        fprintf(stderr, "Error probing for LimeSDR devices\n");
        device_error();
    }
    return 0;
}
