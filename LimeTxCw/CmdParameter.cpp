#include "CmdParameter.h"
#include "LimeTxCw.h"
#include <algorithm>
#include <vector>
#include <sstream>

// constructor
// perform initialization, parsing and parameter extraction
CCmdParameter::CCmdParameter(const std::string cmd)
{
    m_sRawCmd   = cmd;
    m_sParaList = getParaString(cmd);

    // initialize m_stPara
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

// extract parameter part between ( and )
// transform string tolower and remove whitespace
std::string CCmdParameter::getParaString(const std::string s)
{
    std::string sp;
    size_t pos1 = s.find("(");
    size_t pos2 = s.find(")");
    if ((pos1 != std::string::npos) && (pos2 != std::string::npos)) {
        sp = s.substr(pos1 + 1, pos2 - (pos1 + 1));
        transform(sp.begin(), sp.end(), sp.begin(), ::tolower);
        sp.erase(std::remove(sp.begin(), sp.end(), ' '), sp.end());         //remove whitespace
    }
    return sp;
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
            res = 0;
            if (token == "tx")      *val = 1;
            else if (token == "rx") *val = 0;
            else {
                try {
                    *val = stoi(token);
                }
                catch (const std::invalid_argument ) {
                    *val = -1;
                    res = -1;
                }
                catch (const std::exception )
                {
                    *val = -1;
                    res = -1;
                }
            }
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
            res = 0;
            try {
                *val = stof(token);
            }
            catch (const std::invalid_argument) {
                *val = -0.99;
                res = -1;
            }
            catch (const std::exception)
            {
                *val = -0.99;
                res = -1;
            }
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
