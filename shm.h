#include <inttypes.h>

typedef struct { 
	
enum state { idle, want_in, in_cs } flag[19];
   
 	int turn;
 
char* mylist[1000]; 
}sharedInfo;
