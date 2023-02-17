#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(void)
{
    int port;
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    cout << "Enter port number:" << endl;
    cin >> port;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    char server_response[256];
    recv(client_socket, &server_response, sizeof(server_response), 0);
    cout << "The server sent the data: " << server_response << endl;
    close(client_socket);
    return 0;
}