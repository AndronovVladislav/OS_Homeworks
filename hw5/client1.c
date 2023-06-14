#include "common.h"

int main (int argc, char ** argv) {
    int fd1, fd2;
    char buf[BUFSIZE];

    if ((fd1 = open(PIPE1, O_RDONLY)) <= 0 ) {
        perror("open");
        exit(1);
    }
    printf("%s is opened for reading\n", PIPE1);

    if ((fd2 = open(PIPE2, O_WRONLY)) <= 0 ) {
        perror("open");
        exit(1);
    }
    printf("%s is opened for writing\n", PIPE2);

    int len;
    for (;;) {
        memset(buf, 0, BUFSIZE);
        if ((len = read(fd1, buf, BUFSIZE - 1)) <= 0) {
            if (len < 0) {
                perror("read");
            }
            close(fd1);
            exit(0);
        }
        printf("Incomming message: %s\n", buf);

        memset(buf, 0, BUFSIZE);
        printf("Enter message to send to client2: ");
        scanf("%s", buf);
        if (write(fd2, buf, BUFSIZE - 1) <= 0) {
            perror("write");
            close(fd2);
            exit(0);
        }
    }
}