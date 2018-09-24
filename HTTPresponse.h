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
#include <fstream>
#define MAX_SIZE 200
using namespace std;

struct HTTPresponse{
    char file_data[2000];
    char file_name[MAX_SIZE];

    HTTPresponse();
    void parseresponse(char* request, string input, char* file_name);
    string get(char* name, char* port, char* file);
    string getdata(char* directory, char* file_name, char* file_data);
};