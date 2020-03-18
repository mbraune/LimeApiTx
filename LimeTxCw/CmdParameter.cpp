#include "CmdParameter.h"
#include "LimeTxCw.h"
#include <algorithm>
#include <vector>
#include <sstream>

// constructor
// perform initialization, parsing and parameter extraction
CCmdParameter::CCmdParameter(const std::string cmd)
{
    m_sRawCmd = cmd;
    //remove whitespace
    m_sRawCmd.erase(std::remove(m_sRawCmd.begin(), m_sRawCmd.end(), ' '), m_sRawCmd.end());

    // extract parastring
    size_t pos1 = m_sRawCmd.find("(");
    size_t pos2 = m_sRawCmd.find(")");
    if ((pos1 != std::string::npos) && (pos2 != std::string::npos))
        m_sParaList = m_sRawCmd.substr(pos1 + 1, pos2 - (pos1 + 1));
    else
        m_sParaList = "";


    resetData();

    m_idx = getIndex();
    if (m_idx >= 0) {
        m_bValid = true;
        m_eCF = (CtrlFunc)m_idx;

        // get expected number of parameters and parameter types
        m_nPara = 0;
        for (int i = 0; i < CF_MAX_PARA; i++) {
            m_pt[i] = cf_ParaT[m_idx][i];
            m_nPara += (m_pt[i] != pt_None);
        }
    }

    // fill m_stData if parameter available
    for (int i = 0; i < m_nPara; i++) {
        m_stPara[i].ptype = m_pt[i];

        int res = -1; 
        switch (m_pt[i]) {
            case pt_Int:
                res = getIntVal(i, &m_stPara[i].valInt);
                break;
            case pt_Float:
                res = getFloatVal(i, &m_stPara[i].valFloat);
                break;
            case pt_String:
                res = getStringVal(i, &m_stPara[i].valString);
                break;
        }
        m_bValid = (res == 0);

        if (!m_bValid) break;
    }

}

void CCmdParameter::printUsage()
{
}


// reset m_stPara
void CCmdParameter::resetData()
{
    for (int i = 0; i < CF_MAX_PARA; i++) {
        m_stPara[i].ptype       = pt_None;
        m_stPara[i].valInt      = -1;
        m_stPara[i].valFloat    = -9999.9;
        m_stPara[i].valString   = "nix";
    }
}

//
int CCmdParameter::getIndex()
{
    const int n = sizeof(strCtrlFunc) / sizeof(strCtrlFunc[0]);
    for (int i = 0; i < n; ++i)
    {
        //transform szCtrlFunc tolower string
        std::string cs = strCtrlFunc[i];
        transform(cs.begin(), cs.end(), cs.begin(), ::tolower);
        // transform search string tolower
        transform(m_sRawCmd.begin(), m_sRawCmd.end(), m_sRawCmd.begin(), ::tolower);
        if (m_sRawCmd.find(cs) != std::string::npos)
            return i;
    }
    return -1;
}

int CCmdParameter::getIntVal(size_t idx, /*out*/  int *val)
{
    int res = -1;

    std::string token;
    std::istringstream tokenStream(m_sParaList);
    int nCnt = 0;
    while (std::getline(tokenStream, token, ','))
    {
        if (nCnt == idx) {
            // check if "TX" or "RX"
            *val = stoi(token);
            res = 0;
        }
        nCnt++;
    }

    return res;
}

int CCmdParameter::getFloatVal(size_t idx, /*out*/ float_type *val)
{
    int res = -1;

    std::string token;
    std::istringstream tokenStream(m_sParaList);
    int nCnt = 0;
    while (std::getline(tokenStream, token, ','))
    {
        if (nCnt == idx) {
            *val = stof(token);
            res = 0;
        }
        nCnt++;
    }

    return res;
}

int CCmdParameter::getStringVal(size_t idx, /*out*/ std::string* val)
{
    int res = -1;

    std::string token;
    std::istringstream tokenStream(m_sParaList);
    int nCnt = 0;
    while (std::getline(tokenStream, token, ','))
    {
        if (nCnt == idx) {
            *val = token;
            res = 0;
        }
        nCnt++;
    }

    return res;
}

