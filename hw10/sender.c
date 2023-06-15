#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handleSIGUSR1(int nsig) {
}

int main() {
    int receiverPID, number, sig;
    printf("My PID: %d\n", getpid());

    printf("Enter receiver PID: ");
    scanf("%d", &receiverPID);

    printf("Enter number: ");
    scanf("%d", &number);

    signal(SIGUSR1, handleSIGUSR1);

    sigset_t set;
    siginfo_t info;
    struct timespec timeout;
    timeout.tv_sec = 1;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    for (int i = 31; i > -1; --i) {
        // printf("%d", ((number & (1 << i)) >> i));
        fflush(stdout);
        do {
            kill(receiverPID, number & (1 << i) ? SIGUSR1 : SIGUSR2);
        } while (sigtimedwait(&set, &info, &timeout) == -1);
    }

    kill(receiverPID, SIGINT);
    return 0;
}