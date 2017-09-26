#include <inttypes.h>

typedef struct { 
	enum state { idle, want_in, in_cs } flag[19];
    	int turn;
 uint8_t mylist[1000]; }sharedInfo;
