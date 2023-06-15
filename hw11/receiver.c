#include "Other/utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
       fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n",
               argv[0]);
       exit(1);
    }

    int sock;
    struct sockaddr_in echoServAddr;
    int bytesRcvd, totalBytesRcvd;
    char *servIP = argv[1];
    int echoServPort = atoi(argv[2]);
    char str[BUFSIZE];

    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        DieWithError("socket() failed");
    }

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family      = AF_INET;
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);
    echoServAddr.sin_port        = htons(echoServPort);

    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
        DieWithError("connect() failed");
    }

    for (;;) {
        memset(str, 0, BUFSIZE);

        if ((bytesRcvd = recv(sock, str, BUFSIZE, 0)) < 0) {
            DieWithError("recv() failed or connection closed prematurely");
        }
        printf("%s", str);

        if (!strcmp(str, FINAL_STRING)) {
            break;
        }
    }

    close(sock);
    exit(0);
}
