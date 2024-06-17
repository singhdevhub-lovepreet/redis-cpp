#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int read_all(int socket_fd, char *buff, int n){
    while(n>0){
        int recv = read(socket_fd, buff, n);
        if(recv < 0){
            cout<<"Not able to read"<<endl;
            return -1;
        }
        buff+=recv;
        n-=recv;
    }
    return 0;
}

int write_all(int socket_fd, char *buff, int n){
    while(n>0){
        int sent = write(socket_fd, buff, n);
        if(sent < 0){
            cout<<"unable to send"<<endl;
            return -1;
        }
        buff+=sent;
        n-=sent;
    }
    return 0;
}

int query(int socket_fd, char *text){
    int max_message_length = 4096;
    int len = strlen(text);
    char wbuff[4+max_message_length];
    memcpy(wbuff, &len, 4);
    memcpy(&wbuff[4], text, len);
    write_all(socket_fd, wbuff, 4+len);

    char rbuff[4+max_message_length+1];
    read_all(socket_fd, rbuff, 4);
    int len_recieved;
    memcpy(&len_recieved, rbuff, 4);
    read_all(socket_fd, &rbuff[4], len_recieved);
    rbuff[4 + len_recieved] = '\0';
    
    printf("server says: %s\n", &rbuff[4]);
    return 0;

}

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

    query(socket_fd, "hello1");
    query(socket_fd, "hello2"); 
    query(socket_fd, "hello3"); 
    
    close(socket_fd);
    return 0;
}