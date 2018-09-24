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
#include "HTTPrequest.cpp"

using namespace std;
const string INTERNAL = "500\0";
int main(int argc, char *argv[]) {
    //./web-client http//:localhost:4000/index.html
    //Assign Server Details
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct addrinfo server, *server_data, *p;
    memset(&server, 0, sizeof(server));
    server.ai_family = AF_INET;
    server.ai_socktype = SOCK_STREAM;
    HTTPrequest request;
    string path, filename;      //"/users/ugrad/wallnutc/Documents/3D3/Project1/FilesReceivedByClient/";
    cout<< "Enter the entire directory at which you want to store the file" <<endl;
    cout<< "Ending with what you want to name the file: Example: /users/ugrad/wallnutc/result.html" << endl;
    cout<< "Create a document with name of your choice in this directory, in this example result.html in ~/wallnutc/" << endl;
    cout<<"Enter the file name"<<endl;
    cin >> filename;
    cout<< "Enter the directory as in example (including file name)^^" <<endl;
    cin >> path;
    request.parseRequest(argv[1],request.name,request.port,request.file);
    getaddrinfo(request.name, request.port, &server, &server_data);//JUts got rid of codnition if for faiure because it was failing and exiting but wasnt relavent anyway even when i found the right return value
    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    struct sockaddr_in* temp = (struct sockaddr_in*)server_data->ai_addr;
    inet_ntop(server_data->ai_family, &(temp->sin_addr), ipstr, sizeof(ipstr));
    cout << "Server IP : " << ipstr << endl;
    cout << "Sock FD   : " << sock_fd << endl;
    cout << "Port      : " << request.port << endl;

    //Connect to Server
    for(p = server_data; p != NULL; p = p->ai_next) 
    {
    cout<<"Finding server/establishing connections..."<<endl;
    if ((sock_fd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
        perror("socket");
        continue;
    }

    if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1) {
        cout << "ERROR CLIENT: Connecting" << endl;
        close(sock_fd);
        continue;
    }

    break;
    }

    if (p == NULL) {
    cout << "ERROR CLIENT: Connecting" << endl;
    return EXIT_FAILURE;
    }
    freeaddrinfo(server_data); // all done with this structure
    string message = request.get(request.name, request.port, request.file);
    if(request.file[0] == '\0'){
        message = request.get(request.name, request.port, "index.html");
    }
    send(sock_fd, message.c_str(), message.size() + 1, 0);

    char buffer[1000];
    int bytes_in = recv(sock_fd,buffer,sizeof(buffer), 0);
    if (bytes_in < 0){
        cout << "ERROR: " << INTERNAL << endl;
        return EXIT_FAILURE;
    }
    buffer[bytes_in] = '\0';
    if (atoi(buffer) != 500 ){
        if (recv(sock_fd,buffer,sizeof(buffer), 0) < 0){
            cout << "ERROR reading from server\n";
            return EXIT_FAILURE;
        }
        if(atoi(buffer) != 404){
            char yes;
            cout << "File Contents Successfully Obtained, Display ? Y/N: ";
            cin >> yes;
            if (yes == 'Y'){
                cout<<"Response message received from server..."<<endl;
                cout << buffer;
                cout<<endl;
            } 
                char* pth = &path[0];
                char* flname = &filename[0];
                ofstream file(pth); //open in constructor
                string data(buffer);
                file << data;
                ofstream outfile (flname);
                outfile.close();
        }
        else {cout << "ERROR: " << buffer << endl;}
    }
    else {cout << "ERROR: " << buffer << endl;}
}


    /*
    //Communication
    string message;
    string stop = "XXX";
    char buffer[200];
    send(sock_fd, message.c_str(), message.size(), 0);
    cout << "Message Sent Size: " << message.size() << endl;
    //Server Response
    if (recv(sock_fd,buffer,sizeof(buffer), 0) < 0)
    {
        cout << "ERROR reading from server\n";
        return EXIT_FAILURE;
    }
    cout << "Recieved Contents: " << buffer << endl;
    close(sock_fd); 
    */
