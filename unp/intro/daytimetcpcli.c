#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 4096

int main(int argc, char* argv[]) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if(argc != 2)
        return 1;

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("socket");
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13); // daytime server
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        perror("inet_pton");
        return 1;
    }

    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        return 1;
    }

    while( (n = read(sockfd, recvline, MAXLINE)) > 0 ) {
        recvline[n] = 0;
        if(fputs(recvline, stdout) == EOF)
            perror("fputs");
    }
    if(n < 0)  {
        perror("read");
    }

    return 0;
}
