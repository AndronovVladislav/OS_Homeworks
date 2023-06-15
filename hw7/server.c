#include "common.h"

int main() {
    int shmid;
    if ((shmid = shm_open(SHM_REGION, O_RDONLY, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    int *number = mmap(0, sizeof(int), PROT_READ, MAP_SHARED, shmid, 0);
    while (*number != -1) {
        printf("%d ", *number);
        sleep(1);
    }

    exit(0);
}