#include "common.h"

int *number;
int shm_key, shmid;

void handler(int nsig) {
    sleep(2);
    *number = -1;

    shmctl(shmid, IPC_RMID, NULL);
    close(shmid);
    if (shmdt(number) == -1) {
        perror("shm_unlink");
    }

    exit(0);
}

int main(int argc, char** argv) {
    signal(SIGINT, handler);
    srand(time(NULL));

    if (argc != 2) {
        printf("Usage: %s <Number of random numbers>", argv[0]);
    }

    shm_key = ftok(SHM_REGION, 0);
    if ((shmid = shmget(shm_key, sizeof(int),
                        0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if ((shmid = shmget(shm_key, sizeof(int), 0)) < 0) {
            printf("Can\'t connect to shared memory\n");
            exit(-1);
        }
        number = shmat(shmid, NULL, 0);
        printf("Connect to Shared Memory from client\n");
    } else {
        number = shmat(shmid, NULL, 0);
        printf("New Shared Memory from client\n");
    }

    for (int i = 0; i < atoi(argv[1]); ++i) {
        sleep(1);
        *number = rand() % 1000;
        printf("I sent a random number %d\n", *number);
    }

    handler(0);
}