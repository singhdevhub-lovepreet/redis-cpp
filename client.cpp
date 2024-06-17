#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char* argv[]){

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    std::string address = "localhost";
    int port = 8080;
    
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr.s_addr);

    if (connect(socket_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cout<< "Failed to connect to the server" <<std::endl;
        exit(1);
    }
    char* msg = "Hello from client";
    int isSent = send(socket_fd, msg, strlen(msg), 0);

    if(isSent != -1){
        char buffer[1000];
        int result = recv(socket_fd, buffer, 1000, 0);
        
        if(result>0){
            buffer[result] = '\0';
            std::cout<<buffer<<std::endl;
        }

    }
    close(socket_fd);
    return 0;
}