#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int currentBit = 31;
int negative;
int receivedNumber = 0;
int senderPID;

void handleSIGUSR1(int nsig) {
    receivedNumber = receivedNumber | (1 << currentBit);

    // printf("%d %d\n", currentBit, receivedNumber);
    currentBit = currentBit - 1;

    kill(senderPID, SIGUSR1);
}

void handleSIGUSR2(int nsig) {
//     printf("%d %d\n", currentBit, receivedNumber);
    currentBit = currentBit - 1;

    kill(senderPID, SIGUSR1);
}

void handleSIGINT(int nsig) {
    printf("Final number: %d\n", receivedNumber);
    exit(0);
}

int main() {
    printf("My PID: %d\n", getpid());

    printf("Enter sender PID: ");
    scanf("%d", &senderPID);

    signal(SIGUSR1, handleSIGUSR1);
    signal(SIGUSR2, handleSIGUSR2);
    signal(SIGINT, handleSIGINT);

    for (;;);
    return 0;
}