#include "common.h"

int shmid;
int *number;

void handler(int nsig) {
    shmctl(shmid, IPC_RMID, NULL);
    close(shmid);

    if (shmdt(number) == -1) {
        perror("shm_unlink");
    }

    exit(0);
}

int main() {
    int shm_key = ftok(SHM_REGION, 0);
    signal(SIGINT, handler);

    if ((shmid = shmget(shm_key, sizeof(int),
                        0666 | IPC_CREAT | IPC_EXCL)) < 0)  {
        if ((shmid = shmget(shm_key, sizeof(int), 0)) < 0) {
            printf("Can\'t connect to shared memory\n");
            exit(-1);
        }
        number = shmat(shmid, NULL, 0);
        printf("Connect to Shared Memory from server\n");
    } else {
        number = shmat(shmid, NULL, 0);
        printf("New Shared Memory from server\n");
    }

    while (*number != -1) {
        sleep(1);
        if (*number != -1) {
            printf("%d ", *number);
            fflush(stdout);
        }
    }

    printf("\n");
    handler(0);
}