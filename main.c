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

	key_t key;
	char* infile;
	infile = "Input.txt";
	int shmid;
	sharedInfo *shinfo;
	pid_t childpid[4];

	char* mylist_orig[4] = { "hellloweowoe",
						"jsdfhn",
						"dfjnhdkjfnhdf",
						"sfhgdj"};
	char strFromFile[100][100];
	int itr=0;
	int totalStr = 0;
	char words[100][100];


void isPalindrome(char str[])
{
    // Start from leftmost and rightmost corners of str
    int first = 0;
    int last = strlen(str) - 1;
 
    // Keep comparing characters while they are same
    while (last > first)
    {
        if (str[first++] != str[last--])
        {
            printf("%s is Not Palindrome \n", str);
            return;
        }
    }
    printf("%s is palindrome\n", str);
}
// read strings from file
FILE *fp = fopen(infile, "r");
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

    for(itr = 0; itr < totalStr; itr++)
    {
        printf("%s\n", strFromFile[itr]);
       // strcpy(words[itr],strFromFile[itr]);
        isPalindrome(strFromFile[itr]);
         	
    }
    }



	//Generate key (Ref Ex15.1)
	if ((key = ftok(infile,'r')) == (key_t)-1)	{
		perror("Failed to derive key");

		exit(1);
	}
	else
		printf("The generated key is: %d\n",key);

	//Create shared memory segment (Ref 15.6)
	shmid = shmget(key, 100*sizeof(shinfo), PERM |IPC_CREAT |IPC_EXCL);
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
			//shinfo->mylist[i] = mylist_orig[i];
		shinfo->mylist[i] = strFromFile[i];
			printf("main add %x\n" , shinfo-> mylist[i]);
			printf("main %s\n" , shinfo-> mylist[i]);
		}
	}


	//Fork child process
	char *arg1, *arg2;
	arg1 = (char*)malloc(40);
	arg2 = (char*)malloc(40);

	int i;
	for (i = 1; i < 4; i++)
        {
        	if((childpid[i-1] = fork())<=0)
        		break;
        }
        if (childpid[i-1]==-1)
        {
          	printf("Failed to fork:%d",i);
             	return 1;
        }
        if (childpid[i-1]==0)
        {
        	//Do execl/execvp using 3.4,3.6
  
        	sprintf(arg1, "%d", fp);
			sprintf(arg2, "%d", i);
        	execl("palin", arg1-'0', arg2-'0', NULL);

        }
        printf("process started \n");
        wait(NULL);
	return 0;
}
