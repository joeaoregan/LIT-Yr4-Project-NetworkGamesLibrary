
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
