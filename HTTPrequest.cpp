#include "HTTPrequest.h"


HTTPrequest::HTTPrequest(){

    memset(&name[0], 0, sizeof(name));
    memset(&file[0], 0, sizeof(file));
    memset(&port[0], 0, sizeof(port));
}

void HTTPrequest::parseRequest(char* input, char* name, char* port, char* file){
        int i = 7;
        int j = 0;
        bool good = true;
        while(good){
            if (input[i] == ':'){
                i ++;
                name[j] = '\0';
                break;
            }
            else name[j] = input[i];
            i ++;
            j ++;
        }
        j = 0;
        while(good){
            if (input[i] == '/'){
                i ++;
                port[j] = '\0';
                break;
            }
            else port[j] = input[i];
            i ++;
            j ++;
        }
        j = 0;
        while(good){
            if(input[i] == '\0'){
                file[j] = '\0';
                break;
            }
            else file[j] = input[i];
            i ++;
            j ++;
        }
}

string HTTPrequest::get(char* name, char* port, char* file){
    char request[400];
    strcpy(request, "GET /");
    strcat(request, name);
    strcat(request, "/");
    strcat(request, file);
    strcat(request, " HTTP/1.0");
    //strcat(request, '\0');
    string req(request);
    return req;
}