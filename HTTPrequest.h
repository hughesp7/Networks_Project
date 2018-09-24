#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sstream>
#include <string.h>
#include <iomanip>
#define MAX_SIZE 200
using namespace std;

struct HTTPrequest{
    char name[MAX_SIZE];
    char port[MAX_SIZE];
    char file[MAX_SIZE];

    HTTPrequest();
    void parseRequest(char* input, char* name, char* port, char* file);
    string get(char* name, char* port, char* file);
    
};