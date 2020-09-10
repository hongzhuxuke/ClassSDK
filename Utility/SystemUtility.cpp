#include "SystemUtility.h"
#include "Md5.h"
#include <time.h>
#include <windows.h>

SystemUtility::SystemUtility()
{
}


SystemUtility::~SystemUtility()
{
}

std::string& SystemUtility::replace_all(std::string& str, const std::string& old_value, const std::string&   new_value)
{
    while (true) {
        std::string::size_type   pos(0);
        if ((pos = str.find(old_value)) != std::string::npos)
            str.replace(pos, old_value.length(), new_value);
        else
            break;
    }
    return str;
}

string SystemUtility::lltoString(long long t)
{
    std::string result;
    std::strstream ss;
    ss << t;
    ss >> result;
    return result;
}

long long SystemUtility::GetPresentUnixTime() {
    //unixʱ���Ϊ1900������������ ����ȡϵͳ��ǰʱ���Unixʱ��� 
    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);
    char buffer[50];
    sprintf(buffer, "%d-%d-%d %d:%d:%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    std::string strTime(buffer);

    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    sscanf(buffer, "%d-%d-%d %d:%d:%d", 
       &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec);

    tm.tm_year -= 1900;
    tm.tm_mon--;
    time_t  lt = mktime(&tm); //������ȡ(�뼶)��ֵ������Ϳ���ֱ�ӷ���//��һ��llt����ת����(���뼶)��Unixʱ���
    long long unixTime = lt * 1000 + sysTime.wMilliseconds;
    return unixTime;
}

std::string SystemUtility::SortSignParam(std::vector<std::string> params) {
    string words[100];
    int num = params.size();
    for (int i = 0; i < params.size(); i++) {
        words[i] = params.at(i);
    }
    sort(words, words + num);
    std::string param;
    for (int i = 0; i < params.size(); i++) {
        param += words[i];
    }
    return param;
}

std::string SystemUtility::InsertParam(std::vector<std::string> &paramsList, std::string key, std::string value) {
    std::string param;
    param += key;
    param += value;
    paramsList.push_back(param);
    return param;
}

std::string SystemUtility::InsertUrlParam(std::string& baseInfo, std::string key, std::string value) {
    baseInfo += key + "=" + value;
    return  baseInfo;
}

bool SystemUtility::GetCpuByCmd(char *lpszCpu, int len /*= 128*/) {
    const long MAX_COMMAND_SIZE = 10000; // ��������������С	
    WCHAR szFetCmd[] = L"wmic cpu get processorid"; // ��ȡCPU���к�������	
    const string strEnSearch = "ProcessorId"; // CPU���кŵ�ǰ����Ϣ

    BOOL   bret = FALSE;
    HANDLE hReadPipe = NULL; //��ȡ�ܵ�
    HANDLE hWritePipe = NULL; //д��ܵ�	
    PROCESS_INFORMATION pi;   //������Ϣ	
    STARTUPINFO			si;	  //���������д�����Ϣ
    SECURITY_ATTRIBUTES sa;   //��ȫ����

    char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
    string			strBuffer;
    unsigned long	count = 0;
    long			ipos = 0;

    memset(&pi, 0, sizeof(pi));
    memset(&si, 0, sizeof(si));
    memset(&sa, 0, sizeof(sa));

    pi.hProcess = NULL;
    pi.hThread = NULL;
    si.cb = sizeof(STARTUPINFO);
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    //1.0 �����ܵ�
    bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
    if (!bret)
    {
        goto END;
    }

    //2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
    GetStartupInfo(&si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.wShowWindow = SW_HIDE; //���������д���
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    //3.0 ������ȡ�����еĽ���
    bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    if (!bret)
    {
        goto END;
    }

    //4.0 ��ȡ���ص�����
    WaitForSingleObject(pi.hProcess, 3000/*INFINITE*/);
    bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
    if (!bret)
    {
        goto END;
    }

    //5.0 ����CPU���к�
    bret = FALSE;
    strBuffer = szBuffer;
    ipos = strBuffer.find(strEnSearch);

    if (ipos < 0) // û���ҵ�
    {
        goto END;
    }
    else
    {
        strBuffer = strBuffer.substr(ipos + strEnSearch.length());
    }

    memset(szBuffer, 0x00, sizeof(szBuffer));
    strcpy_s(szBuffer, strBuffer.c_str());

    //ȥ���м�Ŀո� \r \n
    int j = 0;
    for (int i = 0; i < strlen(szBuffer); i++)
    {
        if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r')
        {
            lpszCpu[j] = szBuffer[i];
            j++;
        }
    }

    bret = TRUE;

END:
    //�ر����еľ��
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return(bret);
}


std::string SystemUtility::WString2String(const std::wstring& ws)
{
    std::string strLocale = setlocale(LC_ALL, "");
    const wchar_t* wchSrc = ws.c_str();
    size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
    char *chDest = new char[nDestSize];
    memset(chDest, 0, nDestSize);
    wcstombs(chDest, wchSrc, nDestSize);
    std::string strResult = chDest;
    delete[]chDest;
    setlocale(LC_ALL, strLocale.c_str());
    return strResult;
}
// string => wstring
std::wstring SystemUtility::String2WString(const std::string& str)
{
    int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t *wide = new wchar_t[num];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
    std::wstring w_str(wide);
    delete[] wide;
    return w_str;
}

static const char HEX[] = "0123456789abcdef";

char hex_encode(unsigned char val) {
    return (val < 16) ? HEX[val] : '!';
}
const unsigned char URL_UNSAFE = 0x1; // 0-33 "#$%&+,/:;<=>?@[\]^`{|} 127
const unsigned char ASCII_CLASS[128] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,3,1,1,1,3,2,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,3,1,3,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,
};

bool hex_decode(char ch, unsigned char* val) {
    if ((ch >= '0') && (ch <= '9')) {
        *val = ch - '0';
    }
    else if ((ch >= 'A') && (ch <= 'Z')) {
        *val = (ch - 'A') + 10;
    }
    else if ((ch >= 'a') && (ch <= 'z')) {
        *val = (ch - 'a') + 10;
    }
    else {
        return false;
    }
    return true;
}

size_t SystemUtility::url_encode(char * buffer, size_t buflen,
    const char * source, size_t srclen) {
    if (NULL == buffer)
        return srclen * 3 + 1;
    if (buflen <= 0)
        return 0;

    size_t srcpos = 0, bufpos = 0;
    while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
        unsigned char ch = source[srcpos++];
        if ((ch < 128) && (ASCII_CLASS[ch] & URL_UNSAFE)) {
            if (bufpos + 3 >= buflen) {
                break;
            }
            buffer[bufpos + 0] = '%';
            buffer[bufpos + 1] = hex_encode((ch >> 4) & 0xF);
            buffer[bufpos + 2] = hex_encode((ch) & 0xF);
            bufpos += 3;
        }
        else {
            buffer[bufpos++] = ch;
        }
    }
    buffer[bufpos] = '\0';
    return bufpos;
}


size_t SystemUtility::url_decode(char * buffer, size_t buflen,
    const char * source, size_t srclen) {
    if (NULL == buffer)
        return srclen + 1;
    if (buflen <= 0)
        return 0;

    unsigned char h1, h2;
    size_t srcpos = 0, bufpos = 0;
    while ((srcpos < srclen) && (bufpos + 1 < buflen)) {
        unsigned char ch = source[srcpos++];
        if (ch == '+') {
            buffer[bufpos++] = ' ';
        }
        else if ((ch == '%')
            && (srcpos + 1 < srclen)
            && hex_decode(source[srcpos], &h1)
            && hex_decode(source[srcpos + 1], &h2))
        {
            buffer[bufpos++] = (h1 << 4) | h2;
            srcpos += 2;
        }
        else {
            buffer[bufpos++] = ch;
        }
    }
    buffer[bufpos] = '\0';
    return bufpos;
}

//--------------------------------------------------------------
//						�������к� -- ��ȡ����ʱΪ None
//--------------------------------------------------------------
bool SystemUtility::GetBaseBoardByCmd(char *lpszBaseBoard, int len/*=128*/)
{
   const long MAX_COMMAND_SIZE = 10000; // ��������������С	
   WCHAR szFetCmd[] = L"wmic csproduct get UUID"; // ��ȡ�������к�������	
   const string strEnSearch = "UUID"; // �������кŵ�ǰ����Ϣ

   bool   bret = false;
   HANDLE hReadPipe = NULL; //��ȡ�ܵ�
   HANDLE hWritePipe = NULL; //д��ܵ�	
   PROCESS_INFORMATION pi;   //������Ϣ	
   STARTUPINFO			si;	  //���������д�����Ϣ
   SECURITY_ATTRIBUTES sa;   //��ȫ����

   char			szBuffer[MAX_COMMAND_SIZE + 1] = { 0 }; // ���������н�������������
   string			strBuffer;
   unsigned long	count = 0;
   long			ipos = 0;

   memset(&pi, 0, sizeof(pi));
   memset(&si, 0, sizeof(si));
   memset(&sa, 0, sizeof(sa));

   pi.hProcess = NULL;
   pi.hThread = NULL;
   si.cb = sizeof(STARTUPINFO);
   sa.nLength = sizeof(SECURITY_ATTRIBUTES);
   sa.lpSecurityDescriptor = NULL;
   sa.bInheritHandle = TRUE;

   //1.0 �����ܵ�
   bret = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
   if (!bret)
   {
      goto END;
   }

   //2.0 ���������д��ڵ���ϢΪָ���Ķ�д�ܵ�
   GetStartupInfo(&si);
   si.hStdError = hWritePipe;
   si.hStdOutput = hWritePipe;
   si.wShowWindow = SW_HIDE; //���������д���
   si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

   //3.0 ������ȡ�����еĽ���
   bret = CreateProcess(NULL, szFetCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
   if (!bret)
   {
      goto END;
   }

   //4.0 ��ȡ���ص�����
   WaitForSingleObject(pi.hProcess, 5000/*INFINITE*/);
   bret = ReadFile(hReadPipe, szBuffer, MAX_COMMAND_SIZE, &count, 0);
   if (!bret)
   {
      goto END;
   }

   //5.0 �����������к�
   bret = false;
   strBuffer = szBuffer;
   ipos = strBuffer.find(strEnSearch);

   if (ipos < 0) // û���ҵ�
   {
      goto END;
   }
   else
   {
      strBuffer = strBuffer.substr(ipos + strEnSearch.length());
   }

   memset(szBuffer, 0x00, sizeof(szBuffer));
   strcpy_s(szBuffer, strBuffer.c_str());

   //ȥ���м�Ŀո� \r \n
   int j = 0;
   for (int i = 0; i < strlen(szBuffer); i++)
   {
      if (szBuffer[i] != ' ' && szBuffer[i] != '\n' && szBuffer[i] != '\r')
      {
         lpszBaseBoard[j] = szBuffer[i];
         j++;
      }
   }

   bret = true;

END:
   //�ر����еľ��
   CloseHandle(hWritePipe);
   CloseHandle(hReadPipe);
   CloseHandle(pi.hProcess);
   CloseHandle(pi.hThread);

   return(bret);
}