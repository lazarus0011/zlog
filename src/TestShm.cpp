#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include<sys/shm.h>

//#define MacShmSize	10
#define MacShmSize	1024*1024*1024*1

void ShowShmInfoByID(int nShmid)
{
	printf("shm_perm:\n");
	struct shmid_ds ShmidInfo;
	memset(&ShmidInfo, 0, sizeof(ShmidInfo));
	shmctl(nShmid, IPC_STAT, &ShmidInfo);

	printf("shm_perm:\n");
	printf("\tshm_perm.__key:%u\n", ShmidInfo.shm_perm.__key);
	printf("\tshm_perm.uid:%u\n", ShmidInfo.shm_perm.uid);
	printf("\tshm_perm.gid:%u\n", ShmidInfo.shm_perm.gid);
	printf("\tshm_perm.cuid:%u\n", ShmidInfo.shm_perm.cuid);
	printf("\tshm_perm.cgid:%u\n", ShmidInfo.shm_perm.cgid);
	printf("\tshm_perm.mode:%d\n", ShmidInfo.shm_perm.mode);
	printf("\tshm_perm.__seq:%d\n", ShmidInfo.shm_perm.__seq);
	printf("shm_segsz:%u\n", ShmidInfo.shm_segsz);
	printf("shm_atime:%u\n", ShmidInfo.shm_atime);
	printf("shm_dtime:%u\n", ShmidInfo.shm_dtime);
	printf("shm_ctime:%u\n", ShmidInfo.shm_ctime);
	printf("shm_cpid:%u\n", ShmidInfo.shm_cpid);
	printf("shm_lpid:%u\n", ShmidInfo.shm_lpid);
	printf("shm_nattch:%u\n", ShmidInfo.shm_nattch);

}

void ShowShmInfoByAddr(struct shmid_ds* pShmid_ds)
{
	printf("shm_perm:\n");
	printf("\tshm_perm.__key:%u\n", pShmid_ds->shm_perm.__key);
	printf("\tshm_perm.uid:%u\n", pShmid_ds->shm_perm.uid);
	printf("\tshm_perm.gid:%u\n", pShmid_ds->shm_perm.gid);
	printf("\tshm_perm.cuid:%u\n", pShmid_ds->shm_perm.cuid);
	printf("\tshm_perm.cgid:%u\n", pShmid_ds->shm_perm.cgid);
	printf("\tshm_perm.mode:%d\n", pShmid_ds->shm_perm.mode);
	printf("\tshm_perm.__seq:%d\n", pShmid_ds->shm_perm.__seq);
	printf("shm_segsz:%u\n", pShmid_ds->shm_segsz);
	printf("shm_atime:%u\n", pShmid_ds->shm_atime);
	printf("shm_dtime:%u\n", pShmid_ds->shm_dtime);
	printf("shm_ctime:%u\n", pShmid_ds->shm_ctime);
	printf("shm_cpid:%u\n", pShmid_ds->shm_cpid);
	printf("shm_lpid:%u\n", pShmid_ds->shm_lpid);
	printf("shm_nattch:%u\n", pShmid_ds->shm_nattch);

}

int main(int argc, char* argv[])
{
	char* sBuf = "/opt/aspire/product/zhuenshui/test/ipc/shm.cpp";
	//get key
	key_t nKey;
	nKey = ftok(sBuf, 0);
	printf("Key:%u\n", nKey);

	//get shmid
	int nShmid = shmget(nKey, MacShmSize, IPC_CREAT|IPC_EXCL|0666);
	//int nShmid = shmget(nKey, MacShmSize, IPC_CREAT);
	printf("Shmid:%d\n", nShmid);
	ShowShmInfoByID(nShmid);

	//attche shm
	char* pShm = (char*)shmat(nShmid, NULL, 0);
	if((char*)-1 == pShm)
	{
		perror("shmat failed");
		return -1;
	}
	printf("pShm:%p\n", pShm);
	ShowShmInfoByID(nShmid);

	char sExit[32] = "";
	int nTemp = 0;
	nTemp = getchar();
	shmdt((void*)pShm);
	ShowShmInfoByID(nShmid);

	shmctl(nShmid, IPC_RMID, NULL);
	ShowShmInfoByID(nShmid);

	return 0;
}
