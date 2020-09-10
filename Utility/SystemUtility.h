#pragma once
#include <string>
#include <sstream>
#include <string>
#include <sstream>
#include <stdio.h>    
#include <iostream> 
#include "src/sio_client.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <strstream>
#include <time.h>

class SystemUtility
{
public:
    SystemUtility();
    ~SystemUtility();

    static std::string& replace_all(std::string& str, const std::string& old_value, const std::string&   new_value);
    static std::string lltoString(long long t);
    static long long GetPresentUnixTime();
    static std::string SortSignParam(std::vector<std::string> params);
    static std::string InsertParam(std::vector<std::string> &paramsList, std::string key, std::string value);
    static std::string InsertUrlParam(std::string& baseInfo, std::string key, std::string value);
    static bool GetCpuByCmd(char *lpszCpu, int len = 128);
    static std::string WString2String(const std::wstring& ws);
    static std::wstring String2WString(const std::string& s);
    static size_t url_encode(char * buffer, size_t buflen, const char * source, size_t srclen);
    static size_t url_decode(char * buffer, size_t buflen, const char * source, size_t srclen);
    static bool GetBaseBoardByCmd(char *lpszBaseBoard, int len/*=128*/);
};

