// LimeTxCw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "lime/LimeSuite.h"
#include "ControlFunctions.h"
#include "CmdParameter.h"
#include "LimeTxCw.h"

// -----------------------------------

//------------------------------------
// global
//------------------------------------
lms_device_t* device = NULL;

// -----------------------------------
using namespace std;


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
    ss << "\tfreq    = " << fixed << setprecision(6) << myfreq/1e6 << " MHz  \t";

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
            CCmdParameter cPara(cmd);

            CtrlFunc eCF = cPara.getCtrlFunc();
            if (eCF == CF_None) {
                // list available cmds
                system("cls");
                cout << cmd << " - unknown  \t available cmds: " << endl;
                show_available_cmds();
                cout << "err_1000\n";
                continue;
            }

            if (cPara.needHelp()) {
                cout << "\t" << cPara.getUsage()  << endl;
                cout << "err_1000\n";
                continue;
            }

            if (!cPara.isValid()) {
                cout << "\tparameter error" << endl;
                cout << "\t" << cPara.getUsage() << endl;
                cout << "err_1000\n";
                continue;
            }

            switch (eCF) {
                case CF_None:                                                       break;
                case CF_About:                  handle_About();                     break;
                case CF_Syscmd:                 handle_Syscmd(cPara);               break;
                case CF_Help:                   handle_Help();                      break;
                case CF_Devid:                  handle_Devid();                     break;
                case CF_Init:                   handle_Init();                      break;
                case CF_Reset:                  handle_Reset();                     break;
                case CF_GetChipTemperature:     handle_GetChipTemperature(cPara);   break;
                case CF_EnableChannel:          handle_EnableChannel(cPara);        break;
                case CF_SetAntenna:             handle_SetAntenna(cPara);           break;
                case CF_GetAntenna:             handle_GetAntenna(cPara);           break;
                case CF_SetLOFrequency:         handle_SetLOFrequency(cPara);       break;
                case CF_GetLOFrequency:         handle_GetLOFrequency(cPara);       break;
                case CF_SetGaindB:              handle_SetGaindB(cPara);            break;
                case CF_GetGaindB:              handle_GetGaindB(cPara);            break;
                case CF_SetSampleRate:          handle_SetSampleRate(cPara);        break;
                case CF_GetSampleRate:          handle_GetSampleRate(cPara);        break;
                case CF_LoadConfig:             handle_LoadConfig(cPara);           break;
                case CF_SaveConfig:             handle_SaveConfig(cPara);           break;
                case CF_Synchronize:            handle_Synchronize(cPara);          break;
                case CF_SetClockFreq:           handle_SetClockFreq(cPara);         break;
                case CF_GetClockFreq:           handle_GetClockFreq(cPara);         break;
                case CF_VCTCXORead:             handle_VCTCXORead(cPara);           break;
                case CF_VCTCXOWrite:            handle_VCTCXOWrite(cPara);          break;
                case CF_Close:                  handle_Close();                     break;

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
