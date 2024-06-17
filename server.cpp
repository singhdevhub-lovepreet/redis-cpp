#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int read_all(int socket_fd, char *buff, int n)
{
    while (n > 0)
    {
        int recv = read(socket_fd, buff, n);
        if (recv < 0)
        {
            cout << "Not able to read" << endl;
            return -1;
        }
        buff += recv;
        n -= recv;
    }
    return 0;
}

int write_all(int socket_fd, char *buff, int n)
{
    while (n > 0)
    {
        int sent = write(socket_fd, buff, n);
        if (sent < 0)
        {
            cout << "unable to send" << endl;
            return -1;
        }
        buff += sent;
        n -= sent;
    }
    return 0;
}

int one_request(int socket_fd)
{

    int max_message_length = 4096;
    char rbuff[4 + max_message_length];
    int read = read_all(socket_fd, rbuff, 4);
    if (read < 0)
    {
        return -1;
    }
    int len;
    memcpy(&len, rbuff, 4);
    if (len > max_message_length)
    {
        return -1;
    }
    read = read_all(socket_fd, &rbuff[4], len);
    if (read < 0)
    {
        return -1;
    }
    rbuff[4 + read] = '\0';
    char reply[] = "world";
    len = strlen(reply);
    char wbuff[4 + len];
    memcpy(wbuff, &len, 4);
    memcpy(&wbuff[4], reply, len);
    return write_all(socket_fd, wbuff, 4 + len);
}

int main(int argc, char *argv[])
{

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        cout << "Failed to create a socket";
        exit(1);
    }

    int optval = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));

    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(8080);
    char *address = "localhost";

    inet_pton(AF_INET, address, &socket_address.sin_addr.s_addr);

    if (bind(socket_fd, (const struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
    {
        std::cout << "Not able to bind to a particular address and port" << std::endl;
    }
    else
    {
        if (listen(socket_fd, SOMAXCONN) < 0)
        {
            std::cout << "Failed to listen on the socket" << std::endl;
        }
        else
        {
            while (true)
            {
                struct sockaddr_in client_address;
                socklen_t client_address_length = sizeof(client_address);
                int new_socket_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_length);
                if (new_socket_fd < 0)
                {
                    std::cout << "Failed to accept the connection" << std::endl;
                }
                while (true)
                {
                    if (one_request(new_socket_fd) < 0)
                    {
                        break;
                    }
                }
                close(new_socket_fd);
            }
        }
    }
    close(socket_fd);
    return 0;
}