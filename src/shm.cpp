
/*********************************************************** 
Date:		20181130
Author:		zhues
Desc:		share memory manager
***********************************************************/
#include"shm.h"

int ShmManager::nShmGet()
{
	//create shm
	m_nShmID = shmget(m_nKey, m_nShmSize, IPC_CREAT|IPC_EXCL|0664);
	if(-1 == m_nShmID)
	{
		//get shm	
		m_nShmID = shmget(m_nKey, 0, 0);
		if(-1 == m_ShmID)
		{
			perror("[ShmManager::nShmGet]:shmget");
		}
		return _EC_FAIL;
	}

	return _EC_SUCCESS;
}
int ShmManager::nShmGet()
{
	return _EC_SUCCESS;
}
int ShmManager::nShmGet()
{
	return _EC_SUCCESS;
}

private:
	key_t m_nKey;
	int m_nShmID;
	int m_nShmSize;
	char* m_pCur;
	int m_nResSize;


#endif //__SHM_H
