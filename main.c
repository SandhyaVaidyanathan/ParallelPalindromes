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
	//Generate key (Ref Ex15.1)
	if ((key = ftok(file,1) == (key_t)-1))
	{
		perror("Failed to derive key");
		exit(1);
	}
	else
		printf("The generated key is: %d\n",key);

	//Create shared memory segment 
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


	return 0;
}
