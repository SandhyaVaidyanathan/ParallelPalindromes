typedef struct { 
	enum state { idle, want_in, in_cs } flag[19];
    	int turn;
}sharedInfo;
