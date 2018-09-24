#include "HTTPresponse.h"


HTTPresponse::HTTPresponse(){
    memset(&file_name[0], 0, sizeof(file_name));
    memset(&file_data[0], 0, sizeof(file_data));
}



void HTTPresponse::parseresponse(char* request, string input, char* file_name){
    int i = 0;
    int j = 0;
    bool good = true;
        while(good){
             if (input[i] == ' '){
                i++;
                request[i] = '\0';
                break;
            }
            else request[i] = input[i];
            i ++;
        }
        i = 15;
        while(good){
            if (input[i] == ' '){
                i ++;
                file_name[j] = '\0';
                break;
            }
            else file_name[j] = input[i];
            i ++;
            j ++;
        }
    cout << input  << endl;

}

string HTTPresponse::getdata(char* directory, char* file_name, char* file_data){
    string message;
    int i = 0;
    ifstream file;
    char* direc= (char *) malloc(1 + strlen(directory)+ strlen(file_name) );
    strcpy(direc, directory);
    strcat(direc, file_name);
    cout<< direc <<endl;
    file.open(direc);
    if(file){
        while(!file.eof()){
            file.get(file_data[i]);
            i++;
        }
        file_data[i] = '\0';
        message = file_data;
    }
    else message = "404\0";
    return message;
}