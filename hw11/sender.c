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
    char *str;

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

    size_t max_len = BUFSIZE - 1;
    for (;;) {
        getline(&str, &max_len, stdin);
        if (send(sock, str, strlen(str), 0) != strlen(str)) {
            DieWithError("send() failed");
        }

        if (!strcmp(str, FINAL_STRING)) {
            break;
        }
    }

    close(sock);
    exit(0);
}
