#include <iostream>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "error, missing port\r\n";
        exit(1);
    }

    int server_socket_FD = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket_FD < 0)
    {
        std::cout << "socket error\r\n";
        exit(1);
    }

    int port = atoi(argv[1]);

    struct sockaddr_in server_socket_addr;
    memset(&server_socket_addr, 0, sizeof(struct sockaddr_in));

    server_socket_addr.sin_family = AF_INET;
    server_socket_addr.sin_addr.s_addr = INADDR_ANY;
    server_socket_addr.sin_port = htons(port);

    int bind_result = bind(server_socket_FD, (struct sockaddr *)&server_socket_addr, sizeof server_socket_addr);

    if (bind_result < 0)
    {
        std::cout << "socket binding error\r\n";
        close(server_socket_FD);
        exit(1);
    }

    int listening_status = listen(server_socket_FD, 1);

    if (listening_status != 0)
    {
        std::cout << "error, can't listen on port " << port << "\r\n";
        close(server_socket_FD);
        exit(1);
    }

    std::cout << "listening for incoming connections on port " << port << "\r\n";

    struct sockaddr_in client_socket_addr;
    memset(&client_socket_addr, 0, sizeof(struct sockaddr_in));

    socklen_t client_socket_addr_size = sizeof(struct sockaddr_in);

    int client_socket_FD = accept(server_socket_FD, (struct sockaddr *)&client_socket_addr, &client_socket_addr_size);

    if (client_socket_FD < 0)
    {
        std::cout << "connection error\r\n";
        close(server_socket_FD);
        exit(1);
    }

    std::cout << "new incoming connection from " << inet_ntoa(client_socket_addr.sin_addr) << ", client port: " << ntohs(client_socket_addr.sin_port) << "\r\n";

    int bytes_sent = send(client_socket_FD, "Server hello", 13, 0);

    if (bytes_sent != 13)
    {
        std::cout << "sending a message to peer failed\r\n";
        close(server_socket_FD);
        exit(1);
    }

    char buffer[500] = {0};

    for(int messages = 0; messages < 2; ++messages)
    {
        memset(buffer, 0, sizeof(buffer));

        int bytes_received = recv(client_socket_FD, &buffer, 500, 0);

        if (bytes_received > 0)
        {
            std::cout << "Received data: " << buffer << "\r\n";
        }
    }

    std::cout << "server is shuting down...\r\n";

    close(server_socket_FD);

    return 0;
}
