#pragma once
#include "lime/LimeSuite.h"

#define LIMETXCW_VERSION "0.405"

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

#define CF_MAX_PARA 4

const PAR_TYPE cf_ParaT[][CF_MAX_PARA] = {
    {pt_None,   pt_None,   pt_None,   pt_None},     // none
    {pt_None,   pt_None,   pt_None,   pt_None},     // About
    {pt_String, pt_None,   pt_None,   pt_None},     // Syscmd
    {pt_None,   pt_None,   pt_None,   pt_None},     // Help
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


// first number, num of parameters
// NONE_ means no equal sign with value
const std::string cf_usage[] = {
    "none",
    "print version string",
    "run syscmd - example syscmd=ls",
    "help cmd  - example help devid",
    "Devid",
    // ....
};


static const char* strCtrlFunc[] = { CONTROLFUNCS };

// global variables:
extern lms_device_t* device;

// prototypes 
void show_usage();


