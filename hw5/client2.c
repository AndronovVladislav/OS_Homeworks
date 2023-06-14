#include "common.h"

int main (int argc, char ** argv) {
    int fd1, fd2;
    char buf[BUFSIZE];

    remove(PIPE1);
    remove(PIPE2);

    if (mkfifo(PIPE1, 0777)) {
        perror("mkfifo");
        exit(1);
    }
    printf("%s is created\n", PIPE1);

    if (mkfifo(PIPE2, 0777)) {
        perror("mkfifo");
        exit(1);
    }
    printf("%s is created\n", PIPE2);

    if ((fd1 = open(PIPE1, O_WRONLY)) <= 0 ) {
        perror("open");
        exit(1);
    }
    printf("%s is opened for writing\n", PIPE1);

    if ((fd2 = open(PIPE2, O_RDONLY)) <= 0 ) {
        perror("open");
        exit(1);
    }
    printf("%s is opened for reading\n", PIPE2);

    int len;
    for (;;) {
        memset(buf, 0, BUFSIZE);
        printf("Enter message to send to client2: ");
        scanf("%s", buf);

        if (write(fd1, buf, BUFSIZE - 1) <= 0) {
            perror("write");
            close(fd1);
            remove(PIPE2);
            exit(0);
        }

        memset(buf, 0, BUFSIZE);
        if ((len = read(fd1, buf, BUFSIZE - 1)) <= 0) {
            if (len < 0) {
                perror("read");
            }
            close(fd2);
            remove(PIPE1);
            exit(0);
        }
        printf("Incomming message: %s\n", buf);
    }
}