#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
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

char* convertir(struct in_addr ip_addr) {
    char *str = inet_ntoa(ip_addr);
    return str;
}

void envoieMsg(SOCKET sock, char* msg){
    if (send(sock, msg, strlen(msg),0) < 0){
        perror("send()");
    }
    return;
}

void getHours(char* buff){
    time_t now = time(NULL);
    struct tm* local = localtime(&now);

    strftime(buff, sizeof(buff), "%H:%M",local);
}

void getDate(char* buff){
    time_t now = time(NULL);
    struct tm* local = localtime(&now);

    strftime(buff, sizeof(buff), "%s",local); 
}

int main(void){

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET){
        perror("socket()");
        return EXIT_FAILURE;
    }

    struct hostent *hostinfo = NULL;
    SOCKADDR_IN sin = {0};

    sin.sin_addr.s_addr = htonl(INADDR_ANY); // on accepte n'importe qu'elle adresse
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (bind(sock, (SOCKADDR*) &sin, sizeof(sin)) == SOCKET_ERROR){
        perror("bind()");
        return EXIT_FAILURE;
    }

    if (listen(sock, 5) == SOCKET_ERROR){
        perror("listen()");
        return EXIT_FAILURE;
    }

    SOCKADDR_IN csin = {0};
    SOCKET csock;
    socklen_t sinsize;

    // création du socket client établissement du lien ...

    char buffer[1021];
    int n = 0;

    char date_buff[1021];

    while (1){
        sinsize = sizeof(csin);
        csock = accept(sock, (SOCKADDR *) &csin, &sinsize);

        if (csock == INVALID_SOCKET){
            perror("accept()");
            return EXIT_FAILURE;
        }

        if ((n = recv(csock, buffer, sizeof(buffer)-1, 0)) < 0){
            perror("recv()");
            return EXIT_FAILURE;
        }

        buffer[n] = '\0';

        printf("message reçu de %s depuis le port %u\n",convertir(csin.sin_addr), ntohs(csin.sin_port));

        printf("%s\n", buffer);

        if (strcmp(buffer, "heure") == 0){
            printf("clock\n");
            getHours(date_buff);
            envoieMsg(csock, date_buff);
        }

        else if (strcmp(buffer, "date") == 0){
            printf("date\n");
            getDate(date_buff);
            envoieMsg(csock, date_buff);
        }

        else if (strcmp(buffer, "break") == 0){
            closesocket(csock);
            break;
        }
        
        closesocket(csock); // on ferme le socket client
    }

    //closesocket(csock);
    closesocket(sock);

    return EXIT_SUCCESS;
}
