#include "common.h"

void init_shm() {
    int shmid;
    if ((shmid = shm_open(SHM_REGION, O_CREAT | O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    if (ftruncate(shmid, sizeof(int)) == -1) {
        perror("ftruncate");
        exit(-1);
    }

    close(shmid);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <Number of numbers>", argv[0]);
    }
    init_shm();

    int shmid;
    if ((shmid = shm_open(SHM_REGION, O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    int *number = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    int rand;
    for (int i = 0; i < atoi(argv[1]); ++i) {
        *number = random() % 1000;
        printf("I sent a random number %d\n", *number);
        sleep(1);
    }

    close(shmid);
    exit(0);
}