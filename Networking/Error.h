/*
	Error.h
	
	Joe O'Regan
	K00203642
	17/02/2018

	Handle errors
*/


// display error message
void error(const char *msg) {
    perror(msg);
    exit(0);
}

// Display error message, and specify exit condition
void errorEx(const char *msg, int type) {
    perror(msg);
    exit(type);
}
