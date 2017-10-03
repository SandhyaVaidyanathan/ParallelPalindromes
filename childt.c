#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/stat.h>

#include "shm.h"
#define PERM (S_IWUSR | S_IRUSR| S_IRGRP | S_IROTH)


int main(int argc, char const *argv[])
{

	key_t key;
	char* file;
	file = "Input.txt";
	int shmid, i;
	sharedInfo *shinfo;
	pid_t childpid;
	//Generate key (Ref Ex15.1)
	if ((key = ftok(file,'r')) == (key_t)-1)	{
		perror("Failed to derive key");

		exit(1);
	}
	else
		printf("The generated key is: %d\n",key);
	key = 1914778636;
	//Create shared memory segment (Ref 15.6)
	shmid = shmget(key, 100*sizeof(shinfo), PERM | IPC_EXCL);
	if ((shmid == -1) && (errno != EEXIST)) /* real error */
	{
		perror("Unable to read shared memory");
		return -1;
	}
	if (shmid == -1)
	{
		printf("Shared Memory Already created");
		return -1;
	}
	else
	{
		shinfo = (sharedInfo*)shmat(shmid,NULL,0);
		if (shinfo == (void*)-1)
			return -1;
		for (i = 0; i < 19; i++)
		{
			printf("flag[%d] = %d\t\n", i, shinfo->flag[i]);
		}
		for ( i = 0; i < 4; i++)
		{
			printf("%x\n" , shinfo->mylist[i]);
			printf("%s child \n" , shinfo->mylist[i]);

		}
		// do plaindrome and write to file
	}


	//Fork child processd);




	return 0;
}
