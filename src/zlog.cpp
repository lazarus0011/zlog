#include"zlog.h"
//create log record
recDebugLogInfo rLogRec;
//default priority
int nLogPriority = LOG_ERROR; 

void vGetCurrentTime(char * pcCurTime, size_t nBufSize)
{
    time_t now;
    struct tm *t_now;

    now = time(NULL);
    t_now = localtime(&now);
    snprintf(pcCurTime, nBufSize, 
            (char *)"%04d%02d%02d%02d%02d%02d", 
            t_now->tm_year+1900, 
            t_now->tm_mon+1, 
            t_now->tm_mday, 
            t_now->tm_hour,
            t_now->tm_min, 
            t_now->tm_sec);
}

//format date
void LogTime(char* sLogTime, int nSize)
{
	struct tm *ptm;
	struct timeb stTimeb;
	char szTime[MacMilliTimeStrLen];
    ftime(&stTimeb);
    ptm = localtime(&stTimeb.time);
    snprintf(sLogTime, nSize, "%d/%02d/%02d %02d:%02d:%02d.%03d",
                ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm);
}

int nSetLogFile(const char* sProgramName)
{
    char sCurrentTime[MacTimeLen + 1] = ""; 
	memset(sCurrentTime, 0, sizeof(sCurrentTime));
	vGetCurrentTime(sCurrentTime, sizeof(sCurrentTime));

	char sFilename[MacFileNameLen] = "";
	memset(sFilename, 0, sizeof(sFilename));
	sprintf(sFilename, "%s%s.log", sProgramName, sCurrentTime);

    //create log file
	int nLogFD = open(sFilename, O_CREAT|O_EXCL|O_RDWR, 0644);
	if(-1 == rLogRec.nLogFD)
	{
		perror("nDebugLogInit:open");
		return -1;
	}

    return nLogFD;
}

int nDebugLogInit(int argc, char** argv)
{
    /*
    #ifdef ERROR
        nLogPriority = LOG_ERROR;
    #elif DEBUG
        nLogPriority = LOG_DEBUG;
    #else
        nLogPriority = LOG_ERROR;
    #endif
    */
    int nRet = 0;
    memset(&rLogRec, 0, sizeof(rLogRec));
    //get program name
    char sProgramName[MacProgramNameLen + 1] = "";
    memset(sProgramName, 0, sizeof(sProgramName));
    StrimPrefix(argv[0], sProgramName, sizeof(sProgramName));
    strncpy(rLogRec.sProgramName, sProgramName, sizeof(rLogRec.sProgramName));

    //set log priority
    char sOptVal[64] = "";
    memset(sOptVal, 0, sizeof(sOptVal));
    nRet = nOptInfo(argc, argv, "D", sOptVal, sizeof(sOptVal));
    if(0 == nRet)
    {
       nLogPriority = nSetDebugPriority(sOptval) 
    }

    //create log file
	rLogRec.nLogFD = nSetLogFile(sProgramName); 
	if(-1 == rLogRec.nLogFD)
	{
		perror("nDebugLogInit:nSetLogFile");
		return -1;
	}
    //set pid 
    rLogRec.pid = getpid();
    //set tid 
    rLogRec.tid = pthread_self();
    
	return 0;
}

int nDebugLogExit()
{
    if(rLogRec.nLogFD > 0)
    {
        close(rLogRec.nLogFD);
    }
    return 0;
}

int nSetLogType(char* pLogInfo, size_t nSize, const int nPriority)
{
    switch(nPriority)
    {
        case LOG_DEBUG:
            sprintf(pLogInfo, "D ");
            break;
        default:
            sprintf(pLogInfo, "E ");
            break;
    }
    return 0;
}

int nDebugFormatSet(const char* sFormat, va_list& st)
{
    memset(rLogRec.sLogInfo, 0, sizeof(rLogRec.sLogInfo));
    char* pLogInfo = rLogRec.sLogInfo;
    size_t nSize = sizeof(rLogRec.sLogInfo);

    nSetLogType(pLogInfo, nSize, rLogRec.nPriority);

    pLogInfo += strlen(pLogInfo);
    LogTime(pLogInfo, nSize);
    
    pLogInfo += strlen(pLogInfo);
    sprintf(pLogInfo, "(%s,%u|%u,%s:%u):", 
            rLogRec.sProgramName, rLogRec.pid, rLogRec.tid, rLogRec.sFileName, rLogRec.nLine);

    pLogInfo += strlen(pLogInfo);
    vsnprintf(pLogInfo, nSize-(pLogInfo-rLogRec.sLogInfo)-1, sFormat, st);
    strcat(pLogInfo,"\n");

    return 0;
}

int nDebugLog(const char* sFileName, unsigned int nLine, unsigned int nPriority, const char* sFormat, ...)
{
    if(nPriority<nLogPriority)
    {
        return 0;
    }
	va_list st;
	va_start(st, sFormat);
    strncpy(rLogRec.sFileName, sFileName, sizeof(rLogRec.sFileName));
    rLogRec.nLine = nLine;
    rLogRec.nPriority = (enuLogPriority)nPriority;

    nDebugFormatSet(sFormat, st);
    int nRet = write(rLogRec.nLogFD, rLogRec.sLogInfo, strlen(rLogRec.sLogInfo));
    if(-1 == nRet)
    {
        perror("nDebugLog:write");
        return -1;
    }
    return 0;
}

void StrimPrefix(const char* pSrc, char* pDest, unsigned int nSize)
{
    const char* pStr = strchr(pSrc, '/');
    if(pStr)
    {
        strncpy(pDest, pStr+1, nSize);
    }
    else
    {
        strncpy(pDest, pSrc, nSize);
    }
}

int nOptInfo(int argc, char** argv, const char* sOpt, char* sOptVal, unsigned int nSize)
{
    int nRet = 0;
    int nOpt = getopt(argc, argv, sOpt);
    if(-1 != nOpt)
    {
        strncpy(sOptVal, optarg, nSize);
    }
    else
    {
       nRet = -1; 
    }
    printf("%s:%s", sOpt, sOptVal);
    return nRet;
}

int nSetDebugPriority(const char* sOptVal)
{
    int nPriority = 0;
    switch(sOptStr)
    {
        case "DEBUG":
            nPriority = LOG_DEBUG;
            break;
        case "ERROR":
        default:
            nPriority = LOG_ERROR;
            break;
    }

    return nPriority;
}

int nInit(int argc, char** argv)
{
  
    nDebugLogInit(sProgramName); 
    return 0;
}

int nExit()
{
    nDebugLogExit();
    return 0;
}

int main(int argc, char* argv[])
{
    nInit(argc, argv);

    nDebuglog(LOG_DEBUG, "main[%s] begin", argv[0]);
    sleep(1);
    nDebuglog(LOG_ERROR, "execute");
    sleep(1);
    nDebuglog(LOG_DEBUG, "main[%s] end", argv[0]);

    nExit();
    return 0;
}
