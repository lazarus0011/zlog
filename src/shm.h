#ifndef __SHM_H
#define __SHM_H
#pragma once

/*********************************************************** 
Date:		20181130
Author:		zhues
Desc:		share memory manager
***********************************************************/

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdarg.h>


#define _EC_SUCCESS		0
#define _EC_FAIL		0

//get integer key
unsigned int BKDRHash(char *str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	return (hash & 0x7FFFFFFF);
}

void DebugInfo(unsigned int nDebugLevel, const char* sFormat, ...)
{

}


class ShmManager
{
public:
	ShmManager()
	{
		m_nKey = 0;
		m_sShmID = 0;
		m_sShmSize = 0;
		m_nResSize = 0;
		m_pCur = NULL;
	}
	ShmManager(char* sShmName, int nSize)
	{
		m_nKey = BKDRHash(sShmName);
		m_nShmID = 0;;	
		m_nShmSize = nSize;
		m_nResSize = 0;
		m_pCur = NULL;
	}
	~ShmManager(){};

protected:
	//get shm
	int nShmGet();

	//attach shm
	int nShmAt();

	//detach shm
	int nShmDt();

//static function
	//delete shm
	static int nShmDelByName(char* sShmName);

private:
	key_t m_nKey;
	int m_nShmID;
	int m_nShmSize;
	char* m_pCur;
	int m_nResSize;
};


#endif //__SHM_H
