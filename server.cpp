#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <csignal>

bool keepRunning = true;

void signalHandler(int signum) { // Ctrl+c yapana kadar server açık kalrı
    keepRunning = false;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    const int PORT = 8080;

    std::signal(SIGINT, signalHandler);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (keepRunning) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, 1024);
        std::cout << "Received: " << buffer << std::endl;

        // Optional: Send a response
        const char *response = "Hello from server";
        send(new_socket, response, strlen(response), 0);

        close(new_socket);
    }

    close(server_fd);
    std::cout << "Server stopped" << std::endl;

    return 0;
}