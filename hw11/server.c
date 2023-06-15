#include "Other/utils.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    char str[BUFSIZE];
    int echoServPort = atoi(argv[1]);

    int servSock = CreateTCPServerSocket(echoServPort);
    int senderSock = AcceptTCPConnection(servSock);
    int receiverSock = AcceptTCPConnection(servSock);

    for (;;) {
        memset(str, 0, BUFSIZE);

        if (recv(senderSock, str, BUFSIZE, 0) < 0) {
            DieWithError("recv() failed");
        }

        if (send(receiverSock, str, strlen(str), 0) != strlen(str)) {
            DieWithError("send() failed");
        }

        if (!strcmp(str, FINAL_STRING)) {
            break;
        }
    }
    
    close(senderSock);
    close(receiverSock);
    exit(0);
}
