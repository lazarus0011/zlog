#include<iostream>
#include<sys/types.h>
#include<stdarg.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#include<sys/timeb.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include<string.h>

#define MacTimeLen	        15
#define MacMilliTimeStrLen  19
#define MacProgramNameLen   64
#define MacFileNameLen      64
#define MacLogInfoLen       512 

#define nDebuglog(FMT, ...) nDebugLog(__FILE__, __LINE__, (FMT), ##__VA_ARGS__)

typedef enum
{
    LOG_DEBUG = 0x01,
    LOG_ERROR = 0x02
}enuLogPriority;


typedef struct _recDebugLogInfo
{
    unsigned int nLogFD;
    unsigned pid;
    unsigned tid;
    char sFileName[MacFileNameLen + 1];
    unsigned nLine;
    enuLogPriority nPriority;
    char sProgramName[MacProgramNameLen + 1];
    char sLogInfo[MacLogInfoLen + 1];
}recDebugLogInfo;

//get currenttimie
void vGetCurrentTime(char * pcCurTime, size_t nBufSize);
//format date
void LogTime(char* sLogTime, int nSize);
//create or open log file
int nDebugLogInit(const char* sProgramName);
//close log file
int nDebugLogExit();
//formar log record
int nDebugFormatSet(unsigned int nPriority, const char* sFormat, va_list& st);
//write log record to file
int nDebugLog(const char* sFileName, unsigned int nLine, unsigned int nPriority, const char* sFormat, ...); 

//create log record
//recDebugLogInfo rLogRec;
//int nLogPriority;
