#include <iostream>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "error, missing host and/or port\r\n";
        exit(1);
    }

    int client_socket_FD = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket_FD < 0)
    {
        std::cout << "socket error\r\n";
        exit(1);
    }

    int port = atoi(argv[2]);

    struct sockaddr_in client_socket_addr;
    memset(&client_socket_addr, 0, sizeof(struct sockaddr_in));

    client_socket_addr.sin_family = AF_INET;
    client_socket_addr.sin_addr.s_addr = inet_addr(argv[1]);
    client_socket_addr.sin_port = htons(port);

    int connect_result = connect(client_socket_FD, (struct sockaddr*) &client_socket_addr, sizeof(client_socket_addr));

    if (connect_result < 0)
    {
        std::cout << "error connecting to server, error: " << connect_result << "\r\n";
        close(client_socket_FD);
        exit(1);
    }

    std::cout << "Connected to " << inet_ntoa(client_socket_addr.sin_addr) << ":" << port << "\r\n";

    char buffer[500] = {0};

    int bytes_received = recv(client_socket_FD, &buffer, 500, 0);

    if (bytes_received > 0)
    {
        std::cout << "Received data: " << buffer << "\r\n";
    }

    std::cout << "Sending data: \"USER xxx\"\r\n";
    int bytes_sent = send(client_socket_FD, "USER xxx", 9, 0);

    if(bytes_sent != 9)
    {
        std::cout << "message sending error\r\n";
    }

    std::cout << "Sending data: \"PASS xxx\"\r\n";
    bytes_sent = send(client_socket_FD, "PASS xxx", 9, 0);

    if(bytes_sent != 9)
    {
        std::cout << "message sending error\r\n";
    }

    close(client_socket_FD);

    return 0;
}
