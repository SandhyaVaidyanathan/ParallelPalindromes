#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#include "shm.h"
#define PERM (S_IWUSR | S_IRUSR| S_IRGRP | S_IROTH)


int main(int argc, char const *argv[])
{

	key_t key;
	char* infile;
	infile = "Input.txt";
	char* palinfile;
	palinfile = "palin.out";
	char* nopalinfile;
	nopalinfile = "nopalin.out";
	FILE *fpalin, *fnopalin;
	int shmid, i,r;
	sharedInfo *shinfo;
	pid_t childpid = getpid();
	char strFromFile[100][100];
	int itr=0;
	int totalStr = 0;
	char words[100][100];
	int max_writes = 5;
	srand(time(0));
	int n = 10;

	key = 555;
void printPalin( char* palinFilemane, char stringP[])
{
    fpalin = fopen(palinFilemane,"a");
    // If file opened successfully, then write the string to file
   	if ( fpalin )
   		{
   			fprintf(fpalin, "%ld\t",childpid );
   			fprintf(fpalin, "%d\t",shinfo->index );
			fputs(stringP,fpalin);
			fputs("\n",fpalin);

    	}
   	else
    	{
    	   	printf("Failed to open the file\n");
        }
   	fclose(fpalin);
}

void printNonPalin( char* nopalinFilename, char stringN[])
{
    fnopalin = fopen(nopalinFilename,"a");
   	if ( fnopalin )
   	{
   		fprintf(fnopalin, "%ld\t",childpid );
   		fprintf(fnopalin, "%d\t",shinfo->index );
		fputs(stringN,fnopalin);
		fputs("\n",fnopalin); 			
    }
   	else
    {
       	printf("Failed to open the file\n");
    }
	//Close the file
   	fclose(fnopalin);
}

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
        	r = rand() % 3;
        	sleep(r);
            printPalin(nopalinfile,str);
            r = rand() % 3;
        	sleep(r);
            return;
        }
    }
    r = rand() % 3;
    sleep(r);
    printNonPalin(palinfile, str);
    r = rand() % 3;
    sleep(r);

}
void process(const int i ) 
{
	
	int j;
	
	do {
		do {
			shinfo->flag[i] = want_in; // Raise my flag
			j = shinfo->turn; // Set local variable
			// wait until its my turn
 		while ( j != i )
 			j = ( shinfo->flag[j] != idle ) ?shinfo->turn : ( j + 1 ) % n;

		// Declare intention to enter critical section
 		shinfo->flag[i] = in_cs;

 // Check that no one else is in critical section
	for ( j = 0; j < n; j++ )
 		if ( ( j != i ) && ( shinfo->flag[j] == in_cs ) )
 			break;
 } while ( ( j < n ) || ( shinfo->turn != i && shinfo->flag[shinfo->turn] != idle ) );

 // Assign turn to self and enter critical section
 shinfo->turn = i;
 //critical_section();
 /* critical section code starts here, sleep settings are within isPalindrome function*/
 isPalindrome(shinfo->mylist[shinfo->index]);  
 	shinfo->index++;
 
 // Exit section
 j = (shinfo->turn + 1) % n;
 while (shinfo->flag[j] == idle)
 	j = (j + 1) % n;
 
 // Assign turn to next waiting process; change own flag to idle
 shinfo->turn = j; 
 shinfo->flag[i] = idle;
 
 //remainder_section();
 } while ( 1 );
 }

	shmid = shmget(key, 1500*sizeof(shinfo), PERM | IPC_EXCL);
	if ((shmid == -1) && (errno != EEXIST)) 
	{
		perror("Unable to read shared memory");
		return -1;
	} 

	else
	{
		shinfo = (sharedInfo*)shmat(shmid,NULL,0);
		if (shinfo == (void*)-1)
		{
			printf("Cannot attach shared memory\n");
			return -1;
		}

		for (i = 0; i < 5; i++ )
		{

			int n= atoi(argv[0]);
			int xx= atoi(argv[1]);
			process(n);
	//Detaching shared memory
	if (shmdt(shinfo) == -1) 
	{
        fprintf(stderr, "%s Error while detaching shared memory\n");
        exit(1);
    }
		}

    }
	return 0;
}
