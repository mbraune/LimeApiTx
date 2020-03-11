// LimeTxCw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "lime/LimeSuite.h"
#include "ControlFunctions.h"

// -----------------------------------
// -----------------------------------

// below are supported keywords for ControlFunctions
#define CONTROLFUNCS \
    etype(none),      \
    etype(syscmd),    \
    etype(help),      \
    etype(antenna),   \
    etype(ant_bw),    \
    etype(temp),      \
    etype(devid),     \
    etype(freq),      \
    etype(gain_db),   \
    etype(enab_chan), \
    etype(samp_rate), \
    etype(loadcnf),   \
    etype(savecnf),   \
    etype(clock_frq), \
    etype(reset),     \
    etype(close)

#define etype(x) CF_##x

typedef enum { CONTROLFUNCS } CtrlFunc;

#undef etype
#define etype(x) #x

static const char* strCtrlFunc[] = { CONTROLFUNCS };

/*const char* enum2str(CtrlFunc cf)
{
    return strCtrlFunc[static_cast<int>(cf)];
}*/
// -----------------------------------
// -----------------------------------

//------------------------------------
// global
//------------------------------------
lms_device_t* device = NULL;


using namespace std;

// get the enum out of command string
CtrlFunc getCtrlFunc(std::string s)
{
    const int n = sizeof(strCtrlFunc) / sizeof(strCtrlFunc[0]);
    for (int i = 0; i < n; ++i)
    {
        if (s.find(strCtrlFunc[i]) != string::npos)
            return (CtrlFunc)i;
    }
    return CF_none;
}


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
    cout << "LimeTxCw  v0.220" << endl;
    cout << "\tread cmds     :  help,  devid=?,  temp=?, " << endl;
    cout << "\tctrl cmds     :  gain_db=[0..70], freq=[MHz] , antenna=" << endl;
    cout << "\tctrl cmds     :  clock_frq,clkid=[MHz] "<< endl;
    cout << "\tmisc cmd      :  loadcnf , savecnf, syscmd= " << endl;
    cout << "\texit          :  close " << endl;
    cout << "------------------------------------------------------------ " << endl;
}

void show_info()
{
    const lms_dev_info_t* devInfo;
    devInfo = LMS_GetDeviceInfo(device);

    stringstream ss;
    ss << "\t" << devInfo->deviceName << "   serial " << hex << uppercase << devInfo->boardSerialNumber << endl;
    cout << ss.str();
    cout << "\thw version " << devInfo->hardwareVersion << "   fw version " << devInfo->firmwareVersion << endl;
}

void show_status()
{
    stringstream ss;     // temporary stringstream for formatted output (avoid formatting iostream )

    float_type clock;
    LMS_GetClockFreq(device, LMS_CLOCK_REF, &clock);
    ss << "\tclk_ref = " << fixed << setprecision(6) << clock/1e6 << " MHz\n";

    float_type myfreq;
    LMS_GetLOFrequency(device, LMS_CH_TX, 0, &myfreq);
    ss << "\tfreq    = " << defaultfloat << setprecision(8) << myfreq/1e6 << " MHz  \t";

    unsigned int mygain;
    LMS_GetGaindB(device, LMS_CH_TX, 0, &mygain);
    ss << "   gain = " << mygain << " dB" << endl;

    cout << ss.str();
}


// ##########################################################

int main(int argc, char** argv)
{
    show_usage();

    // initial values
    unsigned int gaindB = 20;
    float_type  dFreq = 1575.42e6;

    int n = LMS_GetDeviceList(nullptr);
    if (n > 0)
    {
        // initialization, 
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

            // check for valid cmd
            CtrlFunc eCF = getCtrlFunc(cmd);
            if (eCF == CF_none) {
                cout << "err_1000\n";
                continue;
            }

            switch (eCF) {
                case CF_syscmd:    handle_syscmd(cmd);     break;
                case CF_help:      handle_help();          break;
                case CF_antenna:   show_status();          break;
                case CF_temp:      handle_temp(cmd);       break;
                case CF_devid:     handle_devid(cmd);      break;
                case CF_freq:      handle_freq(cmd);       break;
                case CF_gain_db:   handle_gain_db(cmd);    break;

                case CF_enab_chan: handle_enab_chan(cmd);  break;

                case CF_clock_frq: handle_clock_frq(cmd);  break;
                case CF_reset:     handle_reset();         break;
                case CF_close:     handle_close();         break;

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
            else if (cmd == "close") {
                break;
            }
        }
    }
    else {
        fprintf(stderr, "Error probing for LimeSDR devices\n");
        device_error();
    }
    return 0;
}
