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
	int option = 0;
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
	char *arg1, *arg2, *arg3, *arg4;
	arg1 = (char*)malloc(40);
	arg2 = (char*)malloc(40);
	arg3 = (char*)malloc(40);
	arg4 = (char*)malloc(40);
	int seconds = 60;
	int max_writes=5;

/*if (argc < 2){ // check for valid number of command-line arguments 
  fprintf(stderr, " %s: Error : Try Executable -h for help \n",argv[0]);
  return 1;
}


	while ((option = getopt(argc, argv,"hf:s:n:w")) != -1) {
  switch (option) {
   case 'h' :
           printf("Usage executable -f {filename} -n {no . of processes} -s {no. of seconds} -w{max_writes}\n");
           return 1;
           break;
   case 'n' : noOfChildProcess = atoi(optarg);
           break;
   case 's' : seconds = atoi(optarg);
           break;
    case 'f': 
    		infile = optarg;
    		break;
    case 'w':
    		max_writes = atoi(optarg);
    		break;
   default:
   //displaying error message as per the required format with executable name
           fprintf(stderr, "%s: ",argv[0]);
           perror(" Error: Invalid option");
           abort();
 }
} */

void interruptHandler(int SIG)
{
  signal(SIGQUIT, SIG_IGN);
  signal(SIGINT, SIG_IGN);

  if(SIG == SIGINT)
   {
    fprintf(stderr, "\nCTRL-C encoutered, killing processes\n");
  	}

  if(SIG == SIGALRM) 
  {
    fprintf(stderr, "Master has timed out. killing processes\n");
  }
kill(-getpgrp(), SIGQUIT);
}


	FILE *fp = fopen(infile, "r");

	key = 555;
	//Create shared memory segment (Ref 15.6)
	shmid = shmget(key, 1500*sizeof(shinfo), PERM |IPC_CREAT |IPC_EXCL);
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
    
    		while(fgets(strFromFile[itr], 100, fp)!=NULL)
    		{
        	//remove \n from gets
        	strFromFile[itr][strlen(strFromFile[itr]) - 1] = '\0';
        	itr++;
    		}
        totalStr = itr;

    	for(itr = 0; itr < totalStr; itr++)
    		{ 
        	strcpy(shinfo->mylist[itr],strFromFile[itr]);
    		}
    		itr = 0;
    	}
    	//Interrupts should be handled here
    signal(SIGINT, interruptHandler); 
	signal(SIGALRM, interruptHandler);
	alarm(seconds);


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
  			sprintf(arg1, "%d", id);
			printf("%d process is\n", id);
        	int a;
          for(a=0;a<totalStr;a++)
          {
            sprintf(arg2, "%d", a);
          }
       /* 		sprintf(arg2, "%d", (id-1)*max_writes);
        		sprintf(arg3, "%d", max_writes);
        		sprintf(arg4, "%d", totalStr);
           							
				execl("palin", arg1, arg2,arg3,arg4, NULL); */
  execl("palin", arg1, arg2, NULL);
        }

        wait(NULL);

        fclose(fp);

	}	
	return 0;
}
