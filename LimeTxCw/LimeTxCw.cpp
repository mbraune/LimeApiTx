// LimeTxCw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "lime/LimeSuite.h"
#include "ControlFunctions.h"
#include "LimeTxCw.h"

// -----------------------------------
// -----------------------------------

// below are supported keywords for ControlFunctions
#define CONTROLFUNCS \
    etype(None),      \
    etype(About),      \
    etype(Syscmd),    \
    etype(Help),      \
    etype(Devid),           \
    etype(Init),            \
    etype(Reset),           \
    etype(GetChipTemperature),  \
    etype(EnableChannel),       \
    etype(SetAntenna),          \
    etype(GetAntenna),          \
    etype(SetLOFrequency),      \
    etype(GetLOFrequency),      \
    etype(SetGaindB),           \
    etype(GetGaindB),           \
    etype(SetSampleRate),       \
    etype(GetSampleRate),       \
    etype(LoadConfig),          \
    etype(SaveConfig),          \
    etype(Synchronize),         \
    etype(SetClockFreq),        \
    etype(GetClockFreq),        \
    etype(VCTCXORead),          \
    etype(VCTCXOWrite),         \
    etype(Close)

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

//------------------------------------
// global
//------------------------------------
lms_device_t* device = NULL;

// -----------------------------------

using namespace std;

// get enum out of command string
// do case insensitive find
CtrlFunc getCtrlFunc(std::string s)
{
    const int n = sizeof(strCtrlFunc) / sizeof(strCtrlFunc[0]);
    for (int i = 0; i < n; ++i)
    {
        //transform szCtrlFunc tolower string
        string cs = strCtrlFunc[i];
        transform(cs.begin(), cs.end(), cs.begin(), ::tolower);
        // transform search string tolower
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s.find(cs) != string::npos)
            return (CtrlFunc)i;
    }
    return CF_None;
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

// show list of available commands
// todo format left
void show_available_cmds()
{
    const int n = sizeof(strCtrlFunc) / sizeof(strCtrlFunc[0]);
    for (int i = 1; i < n; ++i)
    {
        if (i%4 == 0)
            cout << std::setw(20) << strCtrlFunc[i] << endl;
        else cout << std::setw(20) << strCtrlFunc[i];
    }
}

void show_usage()
{
    cout << "LimeTxCw " << LIMETXCW_VERSION << endl;
    cout << "\t available cmds: " << endl;
    show_available_cmds();
    cout << "-------------------------------------------------------------------------------- " << endl;
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
            //remove whitespace
            cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());

            // check for valid cmd , do case insensititve find
            CtrlFunc eCF = getCtrlFunc(cmd);
            if (eCF == CF_None) {
                // list available cmds
                system("cls");
                cout << cmd  << " - unknown  \t available cmds: " << endl;
                show_available_cmds();
                cout << "err_1000\n";
                continue;
            }

            switch (eCF) {
                case CF_None:                                                   break;
                case CF_About:                  handle_About();                 break;
                case CF_Syscmd:                 handle_Syscmd(cmd);             break;
                case CF_Help:                   handle_Help(cmd);               break;
                case CF_Devid:                  handle_Devid(cmd);              break;
                case CF_Init:                   handle_Init();                  break;
                case CF_Reset:                  handle_Reset();                 break;
                case CF_GetChipTemperature:     handle_GetChipTemperature(cmd); break;
                case CF_EnableChannel:          handle_EnableChannel(cmd);      break;
                case CF_SetAntenna:             handle_SetAntenna(cmd);         break;
                case CF_GetAntenna:             handle_GetAntenna(cmd);         break;
                case CF_SetLOFrequency:         handle_SetLOFrequency(cmd);     break;
                case CF_GetLOFrequency:         handle_GetLOFrequency(cmd);     break;
                case CF_SetGaindB:              handle_SetGaindB(cmd);          break;
                case CF_GetGaindB:              handle_GetGaindB(cmd);          break;
                case CF_SetSampleRate:          handle_SetSampleRate(cmd);      break;
                case CF_GetSampleRate:          handle_GetSampleRate(cmd);      break;
                case CF_LoadConfig:             handle_LoadConfig(cmd);         break;
                case CF_SaveConfig:             handle_SaveConfig(cmd);         break;
                case CF_Synchronize:            handle_Synchronize(cmd);        break;
                case CF_SetClockFreq:           handle_SetClockFreq(cmd);       break;
                case CF_GetClockFreq:           handle_GetClockFreq(cmd);       break;
                case CF_VCTCXORead:             handle_VCTCXORead(cmd);         break;
                case CF_VCTCXOWrite:            handle_VCTCXOWrite(cmd);        break;
                case CF_Close:                  handle_Close();                 break;

                default: break;
            }

            if (eCF == CF_Close)
                break;
        }
    }
    else {
        fprintf(stderr, "Error probing for LimeSDR devices\n");
        device_error();
    }
    return 0;
}
