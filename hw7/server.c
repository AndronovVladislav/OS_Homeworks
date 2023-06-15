#include "common.h"

int shmid;

void handler(int nsig) {
    close(shmid);
    exit(0);
}

int main() {
    if ((shmid = shm_open(SHM_REGION, O_CREAT | O_RDONLY, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    int *number = mmap(0, sizeof(int), PROT_READ, MAP_SHARED, shmid, 0);
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