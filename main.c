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
	int shmid;
	sharedInfo *shinfo;
	pid_t childpid;

	char mylist_orig[4][100] = { "hellloweowoe",
						"jsdfhn",
						"dfjnhdkjfnhdf",
						"sfhgdj"};

	//Generate key (Ref Ex15.1)




	// read strings from file






	if ((key = ftok(file,'c')) == (key_t)-1)	{
		perror("Failed to derive key");

		exit(1);
	}
	else
		printf("The generated key is: %d\n",key);

	//Create shared memory segment (Ref 15.6)
	shmid = shmget(key, sizeof(shinfo), PERM |IPC_CREAT |IPC_EXCL);
	if ((shmid == -1) && (errno != EEXIST)) /* real error */
	{
		perror("Unable to create shared memory");
		return -1;
	}
	if (shmid == -1)
	{
		printf("Shared Memory Already created");
		return -1;
	}
	else
	{
		int i;
		shinfo = (sharedInfo*)shmat(shmid,NULL,0);
		if (shinfo == (void*)-1)
			return -1;
		shinfo->turn = 9;
		shinfo->flag[0]=1;
		shinfo->flag[3]=2;
		shinfo->flag[4]=3;
		shinfo->flag[5]=4;
		for (i = 0; i < 4; ++i)
		{
			shinfo->mylist[i] = &mylist_orig[i];
			printf("%s\n" , shinfo-> mylist[i]);
			printf("%x\n" , &shinfo-> mylist[i]);
		}
	}


	//Fork child processd);

	int i;
	for (i = 1; i < 4; i++)
        {
     
        	if((childpid = fork())<=0)
        		break;
        }

                if (childpid==-1)
                {
                	printf("Failed to fork:%d",i);
                	return 1;

                }
   
                if (childpid==0)
                {
                	//Do execl/execvp using 3.4,3.6
                }
                printf("process started \n");



	return 0;
}
