#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 256

void DieWithError(char *errorMessage);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in broadcastAddr;
    char *broadcastIP;
    unsigned short broadcastPort;
    char sendString[BUFSIZE];
    int broadcastPermission;
    unsigned int sendStringLen;

    if (argc < 3) {
        fprintf(stderr,"Usage:  %s <IP Address> <Port>\n", argv[0]);
        exit(1);
    }

    broadcastIP = argv[1];
    broadcastPort = atoi(argv[2]);

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
          sizeof(broadcastPermission)) < 0)
        DieWithError("setsockopt() failed");

    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);
    broadcastAddr.sin_port = htons(broadcastPort);

    for (;;) {
        memset(sendString, 0, BUFSIZE);
        printf("Enter message: ");
        scanf("%s", sendString);
        sendStringLen = strlen(sendString);

        if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) 
            &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
            DieWithError("sendto() sent a different number of bytes than expected");
    }
}
