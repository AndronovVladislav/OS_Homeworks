#ifndef UTILS_H
#define UTILS_H

#define BUFSIZE 256
#define MAXPENDING 5
#define FINAL_STRING "The end\n"

#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void DieWithError(char *errorMessage);
void HandleTCPClient(int senderSock, int receiverSock);
int CreateTCPServerSocket(unsigned short port);
int AcceptTCPConnection(int servSock);

#endif