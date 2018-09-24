#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <pthread>
#include "HTTPresponse.cpp"
 
#define THREADS 6
 
using namespace std;
 
 
void *acceptClient(void *threadid);
 
int main(int argc, char *argv[]){
    string ACK = "200 OK";
    string INTERNAL = "500 ERROR";
    pthread_t threads[THREADS];
    //./web localhost 4000 /Documents/3D3/Project1/FilesOnServer/
    const char* tmp = argv[3];
    //Assign and Bind Server Socket Details
    struct addrinfo serv_temp, *serv_data;
    memset(&serv_temp, 0, sizeof(serv_temp));
    serv_temp.ai_family = AF_INET;
    serv_temp.ai_socktype = SOCK_STREAM;
    if ((getaddrinfo(argv[1], argv[2], &serv_temp, &serv_data)) != 0) {
      //  cout << "ERROR SERVER: Get Addr Info" << gai_sterror(status) << endl;
        perror("getaddrinfo");
        return EXIT_FAILURE;
    }
    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    struct sockaddr_in* temp = (struct sockaddr_in*)serv_data->ai_addr;
    inet_ntop(serv_data->ai_family, &(temp->sin_addr), ipstr, sizeof(ipstr));
 
    cout << ipstr  << " " << serv_data->ai_addr <<  endl;
    struct sockaddr_in server;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int port = atoi(argv[2]);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ipstr);
    
    cout << ipstr << " " << sock_fd << " " << port << endl;
    cout << argv[1] << " " << argv[2] << " " << argv[3] << endl;
    if (bind(sock_fd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        cout << "ERROR SERVER: Bind\n";
        perror("bind");
        close(sock_fd);
        return EXIT_FAILURE;
    }
 
    freeaddrinfo(serv_data); // all done with this structure
 
    //Listen for Client
    cout << "Waiting for Connections...\n";
    listen(sock_fd,1);
     
    //Accept Client Connecting
    struct sockaddr_storage client;
    socklen_t client_size = sizeof(client);
    int clientsock_fd = accept(sock_fd, (struct sockaddr *)&client, &client_size);
    if(clientsock_fd < 0){
        cout << "ERROR Connecting to Client Socket\n";
        return EXIT_FAILURE;
    }
    cout << "Server Connected" << endl;
 
 
 
 
    
    char* tempdir = (char*)"/users/ugrad/wallnutc";
    char* directory = (char *) malloc(1 + strlen(tmp)+ strlen(tempdir) );
    strcpy(directory, tempdir);
    strcat(directory, tmp);
    HTTPresponse response;
    string message;
    char buffer[1000];
    int bytes_in = recv(clientsock_fd,buffer,sizeof(buffer), 0);
        if (bytes_in < 0)
        { 
            send(clientsock_fd, INTERNAL.c_str(), INTERNAL.size() + 1, 0);                
            return EXIT_FAILURE;
        }
        else
        {
          string ACK = "200 OK";
          send(clientsock_fd, ACK.c_str(), ACK.size() + 1, 0);   
        }
    response.parseresponse(buffer, response.file_name);
    
    //Search for file
    message = response.getdata(directory, response.file_name, response.file_data);
    
    //Acknowledge Client
    send(clientsock_fd, message.c_str(), message.size() , 0);
    close(clientsock_fd);
 
    return 0;
}
 
void *acceptClient(void *threadid)
{
    struct sockaddr_storage client;
    socklen_t client_size = sizeof(client);
    int clientsock_fd = accept(sock_fd, (struct sockaddr *)&client, &client_size);
    if(clientsock_fd < 0){
        cout << "ERROR Connecting to Client Socket\n";
        return EXIT_FAILURE;
    }
    cout << "Server Connected" << endl;
 
 
 
 
    
    char* tempdir = (char*)"/users/ugrad/wallnutc";
    char* directory = (char *) malloc(1 + strlen(tmp)+ strlen(tempdir) );
    strcpy(directory, tempdir);
    strcat(directory, tmp);
    HTTPresponse response;
    string message;
    char buffer[1000];
    int bytes_in = recv(clientsock_fd,buffer,sizeof(buffer), 0);
        if (bytes_in < 0)
        { 
            send(clientsock_fd, INTERNAL.c_str(), INTERNAL.size() + 1, 0);                
            return EXIT_FAILURE;
        }
        else
        {
          string ACK = "200 OK";
          send(clientsock_fd, ACK.c_str(), ACK.size() + 1, 0);   
        }
    response.parseresponse(buffer, response.file_name);
    
    //Search for file
    message = response.getdata(directory, response.file_name, response.file_data);
    
    //Acknowledge Client
    send(clientsock_fd, message.c_str(), message.size() , 0);
    close(clientsock_fd);
 
 
} 