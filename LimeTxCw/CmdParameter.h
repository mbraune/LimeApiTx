#pragma once

#include <string>
#include "LimeTxCw.h"

typedef struct _stCfPara
{
    PAR_TYPE    ptype;
    int         valInt;
    float_type  valFloat;
    std::string valString;
}stCfPara;


class CCmdParameter
{
    public:
        CCmdParameter(const std::string cmd);  // do initialization, parsing and parameter extraction

        bool     isValid(){ return m_bValid; }
        CtrlFunc getCtrlFunc() { return m_eCF; }
        void     printUsage();

        stCfPara m_stPara[CF_MAX_PARA];         // parameter values

    private:
        bool m_bValid = false;
        std::string m_sRawCmd;
        std::string m_sParaList;
        std::string usage = "";

        int         m_idx = -1;
        CtrlFunc    m_eCF = CF_None;

        PAR_TYPE    m_pt[CF_MAX_PARA];  // expected parametertypes
        size_t      m_nPara = -1;       // expected num of parameters

        void    resetData();
        int     getIndex();

        // extract paras from string
        int     getIntVal(size_t idx, /*out*/ int *val);
        int     getFloatVal(size_t idx, /*out*/ float_type* val);
        int     getStringVal(size_t idx, /*out*/ std::string* val);
};

