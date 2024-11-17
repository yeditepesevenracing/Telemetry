#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

using namespace std;

class Client {
public:
    Client(const string& address, int port);
    ~Client();
    bool connectToServer();
    bool sendData(const int* dataArray, size_t size) const; // size_t is to send the size of the array

private:
    int sock;
    struct sockaddr_in serv_addr;
};

inline Client::Client(const string& address, int port) : serv_addr() {
    sock=0;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr) <=0) {
        cerr << "Invalid adress" << endl;
    }
}

inline Client::~Client() {
    close(sock);
}

inline bool Client::connectToServer() {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
        return false;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return false;
    }
    return true;
}
inline bool Client::sendData(const int *dataArray, size_t size) const {
    if (send(sock, dataArray, size * sizeof(int), 0) < 0) {
        cerr << "Send data failed" << endl;
        return false;
    }
    return true;
 }

