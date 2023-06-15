#include "utils.h"

int AcceptTCPConnection(int servSock) {
    int clntSock;
    struct sockaddr_in echoClntAddr;
    unsigned int clntLen = sizeof(echoClntAddr);

    if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr,
           &clntLen)) < 0) {
        DieWithError("accept() failed");
    }

    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
    return clntSock;
}
