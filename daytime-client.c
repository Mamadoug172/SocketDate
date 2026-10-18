#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define PORT 9999

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

void clearBuffer(char* buffer, size_t size){
    for (size_t i = 0;i<size;i++){
        buffer[i] = '\0';
    }
}



int main(void){

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET){
        perror("socket()");
        return EXIT_FAILURE;
    }

    struct hostent *hostinfo = NULL;
    SOCKADDR_IN sin = {0};
    const char *addr = "127.0.0.1";

    IN_ADDR ip_binaire;

    if (inet_pton(AF_INET, addr, &ip_binaire) <= 0){
        perror("inet_pton");
        closesocket(sock);
        return EXIT_FAILURE;
    }

    hostinfo = gethostbyaddr(&ip_binaire,sizeof(ip_binaire),AF_INET);

    if (hostinfo == NULL){
        fprintf(stderr, "Unknown host %s\n", addr);
        return EXIT_FAILURE;
    }

    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    
    // on se connecte au socket

    if (connect(sock, (SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR){
        perror("connect()");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    bool cont = true;
    char buffCont[1024];
    
    char buffer_rcv[1024];
    int n = 0;

    while (cont){

        printf("Envoyer un message au flux ? [msg]\n");
        scanf(" %1023s", buffer);

        if (send(sock, buffer, strlen(buffer),0) < 0){
            perror("send()");
            return EXIT_FAILURE;
        }

        if ((n=recv(sock, buffer_rcv, sizeof(buffer_rcv)-1,0)) <0){
            perror("recv()");
            return EXIT_FAILURE;
        }else if (n>0){
            printf("%s\n", buffer_rcv);
        }

        printf("Voulez-vous continuer ? [y,n]\n");
        scanf(" %1023[^\n]", buffCont);

        if (strcmp(buffCont, "n") == 0){
            cont = false;
        }
    }

    closesocket(sock);

    return EXIT_SUCCESS;
}
