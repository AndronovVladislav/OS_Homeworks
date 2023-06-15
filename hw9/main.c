#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE             256
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_RESET    "\x1b[0m"

#define PRINT_GREEN(X) printf("%s%s%s ", ANSI_COLOR_GREEN, X, ANSI_COLOR_RESET)
#define PRINT_RED(X) printf("%s%s%s ", ANSI_COLOR_RED, X, ANSI_COLOR_RESET)

int semid = -1;
void (*prev)(int);

void handler(int nsig) {
    for (int i = 0; i < 2; ++i) {
        semctl(semid, i, IPC_RMID);
    }
    exit(0);
}

int main(int argc, char **argv) {
    prev = signal(SIGINT, handler);
    int fd[2];
    pid_t chpid;
    char buffer[BUFSIZE];

    int shm_key = ftok("main.c", 0), semid;
    if ((semid = semget(shm_key, 2, 0666 | IPC_CREAT | IPC_EXCL)) < 0){
        if ((semid = semget(shm_key, 2, 0)) < 0) {
            printf("Can\'t connect to semaphore\n");
            exit(-1);
        }
    } else {
        for (int i = 0; i < 2; ++i) {
            semctl(semid, i, SETVAL, 0);
        }
    }

    pipe(fd);
    if ((chpid = fork()) == -1) {
        printf("I can't create first child :c\n");
        exit(-1);
    } else if (chpid == 0) {
        signal(SIGINT, prev);
        struct sembuf post = { 0, 1, 0 };
        struct sembuf wait = { 1, -1, 0 };

        for (int i = 0; i < 10; ++i) {
            semop(semid, &wait, 1);

            size_t n_bytes = read(fd[0], buffer, BUFSIZE - 1);
            buffer[n_bytes] = '\0';
            PRINT_RED("CHILD: ");
            printf("received message №%d: %s\n", i + 1, buffer);

            // printf("Enter message: ");
            // memset(buffer, 0, BUFSIZE);
            // scanf("%s", buffer);
            sprintf(buffer, "i'm message №%d\n", i);
            write(fd[1], buffer, strlen(buffer));

            semop(semid, &post, 1);
        }
    } else {
        struct sembuf post = { 1, 1, 0 };
        struct sembuf wait = { 0, -1, 0 };

        for (int i = 0; i < 10; ++i) {
            // printf("Enter message: ");
            // memset(buffer, 0, BUFSIZE);
            // scanf("%s", buffer);
            sprintf(buffer, "i'm message №%d\n", i);
            write(fd[1], buffer, strlen(buffer));

            semop(semid, &post, 1);
            semop(semid, &wait, 1);

            size_t n_bytes = read(fd[0], buffer, BUFSIZE - 1);
            buffer[n_bytes] = '\0';
            PRINT_GREEN("PARENT: ");
            printf("received message №%d: %s\n", i + 1, buffer);
        }
    }
    exit(0);
}