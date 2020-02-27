// LimeTxCw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "lime/LimeSuite.h"


using namespace std;

static lms_device_t* device = NULL;
static unsigned int gaindB = 0;
static float_type  freq = 1.1;

int device_error()
{
    if (device != NULL) {
        cout << "ERROR: " << LMS_GetLastErrorMessage() << endl;
        LMS_Close(device);
    }
    exit(-1);
}

void show_usage()
{
    cout << "LimeTxCw  v0.201" << endl;
    cout << "\tread cmds     :  help , info , stat , temp, " << endl;
    cout << "\tctrl cmds     :  txon , txoff , gain [0..70], freq [MHz] " << endl;
    cout << "\tmisc cmd      :  clr " << endl;
    cout << "\texit          :  stop " << endl;
    cout << "------------------------------------------------------------ " << endl << endl;
}

void show_info()
{
    stringstream ss;   // temporary stringstream for formatted output (avoid formatting iostream )

    const lms_dev_info_t* devInfo;
    devInfo = LMS_GetDeviceInfo(device);

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


int main(int argc, char** argv)
{
    show_usage();

    // initial values
    freq = 1575.42e6;
    gaindB = 20;

    int n = LMS_GetDeviceList(nullptr);
    if (n > 0)
    {
        if (LMS_Open(&device, NULL, NULL) != 0) //open first device
            device_error();

        show_info();

        if (LMS_Init(device) != 0)
            device_error();

        if (LMS_EnableChannel(device, LMS_CH_TX, 0, true) != 0)
            device_error();

        if (LMS_SetLOFrequency(device, LMS_CH_TX, 0, freq) != 0)
            device_error();

        if (LMS_SetGaindB(device, LMS_CH_TX, 0, gaindB) != 0)
            device_error();

        if (LMS_SetAntenna(device, LMS_CH_TX, 0, LMS_PATH_AUTO) != 0)
            device_error();

        show_status();

        string cmd;
        while (getline(cin, cmd))
        {
            if      (cmd == "help") show_usage();
            else if (cmd == "info") show_info();
            else if (cmd == "stat") show_status();
            else if (cmd == "temp") {
                float_type temp;
                if (LMS_GetChipTemperature(device, 0, &temp) < 0)
                    device_error();
                cout << "\ttemperature = " << temp << endl;
            }
            else if (cmd == "txon") {
                LMS_EnableChannel(device, LMS_CH_TX, 0, true);
                show_status();
            }
            else if (cmd == "txoff") {
                LMS_EnableChannel(device, LMS_CH_TX, 0, false);
            }
            else if (cmd.find("gain") != string::npos) {
                cmd.erase(0, 4);
                if (cmd.empty() || (cmd.find_first_of("0123456789") == string::npos))
                    cout << "\tusage  gain [0..70]" << endl;
                else
                    LMS_SetGaindB(device, LMS_CH_TX, 0, stoi(cmd));
            }
            else if (cmd.find("freq") != string::npos) {
                cmd.erase(0, 4);
                if (cmd.empty() || (cmd.find_first_of("0123456789") == string::npos))
                    cout << "\tusage  freq [MHz]" << endl;
                else {
                    float_type freq1 = stod(cmd) * 1e6;
                    if (LMS_SetLOFrequency(device, LMS_CH_TX, 0, freq1) != 0)
                        device_error();
                }
            }
            else if (cmd == "clr") {
                system("CLS");
                show_usage();
            }
            else if (cmd == "stop") {
                LMS_Reset(device);
                if (LMS_Close(device) == 0)
                    cout << "Closed" << endl;
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
