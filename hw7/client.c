#include "common.h"

int *number;
int shmid;

void init_shm() {
    if ((shmid = shm_open(SHM_REGION, O_CREAT | O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open");
        exit(-1);
    }

    if (ftruncate(shmid, sizeof(int)) == -1) {
        perror("ftruncate");
        exit(-1);
    }
}

void handler(int nsig) {
    sleep(2);
    *number = -1;

    close(shmid);
    if (shm_unlink(SHM_REGION) == -1) {
        perror("shm_unlink");
    }

    exit(0);
}

int main(int argc, char** argv) {
    signal(SIGINT, handler);
    if (argc != 2) {
        printf("Usage: %s <Number of random numbers>", argv[0]);
    }
    init_shm();

    srand(time(NULL));
    number = mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    for (int i = 0; i < atoi(argv[1]); ++i) {
        sleep(1);
        *number = random() % 1000;
        printf("I sent a random number %d\n", *number);
    }

    handler(0);
}