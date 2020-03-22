#pragma once
#include "lime/LimeSuite.h"

#define LIMETXCW_VERSION "0.500"

typedef enum _PAR_TYPE
{
    pt_None,
    pt_Int,
    pt_Float,
    pt_String
} PAR_TYPE;

//###############################################################

// below are supported keywords for ControlFunctions
#define CONTROLFUNCS \
    etype(None),      \
    etype(About),      \
    etype(Help),      \
    etype(Syscmd),    \
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

#define CF_MAX_PARA 4

const PAR_TYPE cf_ParaT[][CF_MAX_PARA] = {
    {pt_None,   pt_None,   pt_None,   pt_None},     // none
    {pt_None,   pt_None,   pt_None,   pt_None},     // About
    {pt_None,   pt_None,   pt_None,   pt_None},     // Help
    {pt_String, pt_None,   pt_None,   pt_None},     // Syscmd
    {pt_None,   pt_None,   pt_None,   pt_None},     // Devid
    {pt_None,   pt_None,   pt_None,   pt_None},     // Init
    {pt_None,   pt_None,   pt_None,   pt_None},     // Reset
    {pt_Int,    pt_None,   pt_None,   pt_None},     // GetChipTemperature
    {pt_Int,    pt_Int,    pt_Int,    pt_None},     // EnableChannel
    {pt_Int,    pt_Int,    pt_Int,    pt_None},     // SetAntenna
    {pt_Int,    pt_Int,    pt_None,   pt_None},     // GetAntenna
    {pt_Int,    pt_Int,    pt_Float,  pt_None},     // SetLOFrequency
    {pt_Int,    pt_Int,    pt_None,   pt_None},     // GetLOFrequency
    {pt_Int,    pt_Int,    pt_Int,    pt_None},     // SetGaindB
    {pt_Int,    pt_Int,    pt_None,   pt_None},     // GetGaindB
    {pt_Float,  pt_Int,    pt_None,   pt_None},     // SetSampleRate
    {pt_Int,    pt_Int,    pt_None,   pt_None},     // GetSampleRate
    {pt_String, pt_None,   pt_None,   pt_None},     // LoadConfig
    {pt_String, pt_None,   pt_None,   pt_None},     // SaveConfig
    {pt_Int,    pt_None,   pt_None,   pt_None},     // Synchronize
    {pt_Int,    pt_Float,  pt_None,   pt_None},     // SetClockFreq
    {pt_Int,    pt_None,   pt_None,   pt_None},     // GetClockFreq
    {pt_None,   pt_None,   pt_None,   pt_None},     // VCTCXORead
    {pt_Int,    pt_None,   pt_None,   pt_None},     // VCTCXOWrite
    {pt_None,   pt_None,   pt_None,   pt_None},     // Close
};


const std::string cf_usage[] = {
    "none",
    "About()  , print version string",
    "Help()",
    "Syscmd(<string> )run system command         \n\texample: Syscmd(dir)" ,
    "Devid()" ,
    "Init()" ,
    "Reset()" ,
    "GetChipTemperature(0)    return temp value of chipindex 0" ,
    "EnableChannel(dir_tx, ch_idx, bool enable)  \n\texample: EnableChannel(tx,0,1)" ,
    "SetAntenna(dir_tx, ch_idx, antenna_idx)     \n\texample: SetAntenna(tx,0,0) ",
    "GetAntenna(dir_tx, ch_idx)  returns antenna idx   \n\texample: GetAntenna(tx,0) ",
    "SetLOFrequency(dir_tx, ch_idx, f[MHz])      \n\texample: SetLOFrequency(tx, 0, 1575.42)",
    "GetLOFrequency(dir_tx, ch_idx) return f[MHz]\n\texample: GetLOFrequency(tx, 0)",
    "SetGaindB(dir_tx, ch_idx, gain[dB])         \n\texample: SetGaindB(tx, 0, 30)",
    "GetGaindB(dir_tx, ch_idx) return gain[dB]   \n\texample: GetGaindB(tx, 0)",
    "SetSampleRate(float_rate[MHz], oversamp)    \n\texample: SetSampleRate(20, 0)",
    "GetSampleRate(dir_tx, ch_idx)               \n\texample: GetSampleRate(tx, 0)",
    "LoadConfig",
    "SaveConfig(filename)                        \n\texample: SaveConfig(myconf.ini)",
    "Synchronize(bool toChip)                    \n\texample: Synchronize(1)",
    "SetClockFreq(int clkid, f[MHz]              \n\texample: SetClockFreq(2, 40)",
    "GetClockFreq(int clkid)   return f[MHz]     \n\texample: GetClockFreq(2)",
    "VCTCXORead()",
    "VCTCXOWrite(int dacvalue)                   \n\texample: VCTCXOWrite(65550)",
    "Close()"
};


static const char* strCtrlFunc[] = { CONTROLFUNCS };

// global variables:
extern lms_device_t* device;

// prototypes 
void show_usage();


