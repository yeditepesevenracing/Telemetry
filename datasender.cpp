#include "Client.h"
#include <iostream>

using namespace std;

int main() {
    int data[20] = {2, 4, 5, 6};
    Client client("127.0.0.1", 8080);
    if (client.connectToServer()) {
        return client.sendData(data, 20);
    }
    return 0;
}
