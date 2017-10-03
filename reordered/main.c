#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "shm.h"
#define PERM (S_IWUSR | S_IRUSR| S_IRGRP | S_IROTH)


int main(int argc, char const *argv[])
{
	int noOfChildProcess = 10; //no. of child processes
	key_t key;
	char* infile;
	infile = "Input.txt";
	char* palinfile;
	palinfile = "palin.out";
	char* nopalinfile;
	nopalinfile = "nopalin.out";
	FILE *fpalin, *fnopalin;
	int shmid;
	sharedInfo *shinfo;
	pid_t childpid[4];
	char strFromFile[100][100];
	int itr=0;
	int totalStr = 0;
	char words[100][100];
	char *arg1, *arg2, *arg3;
	arg1 = (char*)malloc(40);
	arg2 = (char*)malloc(40);
	arg3 = (char*)malloc(40);

	FILE *fp = fopen(infile, "r");

	key = 555;
	//Create shared memory segment (Ref 15.6)
	shmid = shmget(key, 2200*sizeof(shinfo), PERM |IPC_CREAT |IPC_EXCL);
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
		// read strings from file
		shinfo->index = 0;
		shinfo->turn = 0;
    	if(fp==NULL)
        	perror("File does not exist");
    	else
    	{
    
    		while(fgets(strFromFile[itr], 100, fp))
    		{
        	//remove \n from gets
        	strFromFile[itr][strlen(strFromFile[itr]) - 1] = '\0';
        	itr++;
    		}
        totalStr = itr;
		printf("outside\n");
    	for(itr = 0; itr < totalStr; itr++)
    		{ 
        	strcpy(shinfo->mylist[itr],strFromFile[itr]);
    		}
    		itr = 0;
    	}
    	//Interrupts should be handled here

    	//call child process -palin
    	int id;
		for (id = 1; id < noOfChildProcess; id++)
        {
        	if((childpid[id-1] = fork())<=0)
        		break;
        }
        if (childpid[id-1]==-1)
        {
          	printf("Failed to fork:%d",i);
             	return 1;
        }
        if (childpid[id-1]==0)
        {
        	//Do execl/execvp using 3.4,3.6
  
        	sprintf(arg1, "%d", fp);
        	printf("%d fp is\n", fp);
			sprintf(arg2, "%d", id);
			printf("%d id is\n", id);
			sprintf(arg1, "%d", shmid);
        	execl("palin", arg1, arg2,arg3, NULL);

        }

        wait(NULL);

	}	
	return 0;
}
