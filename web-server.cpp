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
#include <pthread.h>
#include "HTTPresponse.cpp"

#define THREADS 6

using namespace std;

struct bundle{
    int curr_client_sock;
    int server_sock;
    int end_server;

}bundle;

const string ACK = "200\0";
const string INTERNAL = "500\0";
char* directory;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t go = PTHREAD_COND_INITIALIZER;

void *acceptClient(void *threadid);


int main(int argc, char *argv[]){
    struct bundle server_stats;
    server_stats.end_server = 0;
    pthread_t threads[THREADS];
    //./server localhost 4000 /Documents/3D3/Project1/FilesOnServer/
    directory = argv[3];
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

    struct sockaddr_in server;
    server_stats.server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int port = atoi(argv[2]);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ipstr);
    
    cout << "Server IP : " << ipstr << endl;
    cout << "Sock FD   : " << server_stats.server_sock << endl;
    cout << "Port      : " << port << endl;
    if (bind(server_stats.server_sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        cout << "ERROR SERVER: Bind\n";
        perror("bind");
        close(server_stats.server_sock);
        return EXIT_FAILURE;
    }

    freeaddrinfo(serv_data); // all done with this structure

    //Listen for Client
    cout << "Waiting for Connections...\n";
    int rc;
    for(int i = 0; i < THREADS; i++)
    {
        rc = pthread_create( &threads[i], NULL, acceptClient,(void *)&server_stats );
        if (rc) 
        { 
			printf("ERROR return code from pthread_create(): %d\n",rc); 
			exit(-1); 
		} 
    }
    
    while( server_stats.end_server != 1 )
    {
        listen(server_stats.server_sock,1);
        struct sockaddr_storage client;
        socklen_t client_size = sizeof(client);
        server_stats.curr_client_sock = accept(server_stats.server_sock, (struct sockaddr *)&client, &client_size);
        if(server_stats.curr_client_sock < 0)
        {
            cout << "ERROR Connecting to Client Socket\n";
        }
        else
        {
            cout << "Server Connected to client" << endl;
            pthread_mutex_lock(&lock);

            pthread_cond_signal(&go);

            pthread_mutex_unlock(&lock);
        }
    }

    for(int i = 0; i < THREADS; i++) 
    { 
		pthread_join( threads[i], NULL); 
	} 
    
	return 0;
    
}

void *acceptClient(void *threadid)
{
    //cout<<"Thread created"<<endl;
    struct bundle* server_stats = (struct bundle  *)threadid;
    
    while( server_stats->end_server != 1)
    {
        pthread_cond_wait(&go, &lock);
        int clientserver_sock = server_stats->curr_client_sock;
        HTTPresponse response;
        string message, request;
        char buffer[1000];
        char buffrequest[100];
        int bytes_in = recv(clientserver_sock,buffer,sizeof(buffer), 0);
        if (bytes_in < 0)
        { 
            send(clientserver_sock, INTERNAL.c_str(), INTERNAL.size() + 1, 0);                
        }
        buffer[bytes_in] = '\0';
        response.parseresponse(buffrequest, buffer, response.file_name);
            //Search for file
        request = buffrequest;
        if (request == "GET"){
            send(clientserver_sock, ACK.c_str(), ACK.size() + 1, 0);
            message = response.getdata(directory, response.file_name, response.file_data);
        }
        else message = INTERNAL;
        //Acknowledge Client
        send(clientserver_sock, message.c_str(), message.size() , 0);
        close(clientserver_sock);
    }

    pthread_exit(NULL);
}




/*
struct sockaddr_in server;
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int port = atoi(argv[2]);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &server.sin_addr);

    if (bind(server_sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cout << "ERROR SERVER: No Bind\n";
        close(server_sock);
        return EXIT_FAILURE;
    }

    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(server.sin_family, &server.sin_addr, ipstr, sizeof(ipstr));
    cout << "Server\nName: " << argv[1] << "\nIP: " << ipstr << "\nPort: " <<
    ntohs(server.sin_port) << endl;
*/
/*
    //Communication
    char buffer[200];
    string ack, bufstring;
    int bytes_in;
    ifstream file;
        //Read in from Client
        bytes_in = recv(clientserver_sock,buffer,sizeof(buffer), 0);
        if (bytes_in < 0)
        {
            cout << "ERROR reading from socket\n";                      
            return EXIT_FAILURE;
        }
        buffer[bytes_in] = '\0';
        cout << "Message Recieved Size: " << bytes_in << endl;
        cout << "Requested File: " << buffer << endl;

        //Check for end result

        //Search for file
        file.open(buffer);
        if(!file.is_open()){
            ack = "ERROR 404 File Not Found\0";
        }

        else getline(file, ack);
        file.close();
        
        //Acknowledge Client
        send(clientserver_sock, ack.c_str(), ack.size() , 0);
    
    close(clientserver_sock);
    */






//if ((host = gethostbyname(argv[1])) == NULL) {  // get the host info
    //    cout << "ERROR Getting Host Details";
    //    return EXIT_FAILURE;
    //}
    //printf("Host name  : %s\n", host->h_name);
    //printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));