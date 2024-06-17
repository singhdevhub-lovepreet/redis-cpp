#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;


int main(int argc, char *argv[]){

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd<0){
        cout<<"Failed to create a socket";
        exit(1);
    }    

    int optval = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));

    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(8080);
    char* address = "localhost";

    inet_pton(AF_INET, address, &socket_address.sin_addr.s_addr);

    if(bind(socket_fd, (const struct sockaddr *)&socket_address, sizeof(socket_address))<0){
        std::cout<<"Not able to bind to a particular address and port"<<std::endl;
    }else{
        if (listen(socket_fd, SOMAXCONN) < 0) {
            std::cout << "Failed to listen on the socket" << std::endl;
        } else {
            struct sockaddr_in client_address;
            socklen_t client_address_length = sizeof(client_address);
            int new_socket_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_length);
            if (new_socket_fd < 0) {
                std::cout << "Failed to accept the connection" << std::endl;
            }

            char* message = "Hello from server";
            int bytesSent = send(new_socket_fd, message, strlen(message),0);
            if(bytesSent < 0){
                std::cout<<"Can not send from the server"<<std::endl;
            }
            close(new_socket_fd);
        }
    }
    close(socket_fd);
    return 0;
}