#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void check_exit(char* buffer, int* newsockfd, int* sockfd)
{
    if(strcmp(buffer, "exit") == 0)
    {    
        printf("Coonection Terminated\n");
        close(*newsockfd);
        close(*sockfd);
        exit(1);
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket");
    
    bzero((char *)&serv_addr, sizeof(serv_addr));
    
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    
    if (newsockfd < 0)
        error("ERROR on accept");
    else
		printf("You are connected now client\n");
    
    while (1)
    {

        bzero(buffer, 256);
        n = read(newsockfd, buffer, 256);
        buffer[strcspn(buffer, "\n")] = 0;
        check_exit(buffer, &newsockfd, &sockfd);

        if (n < 0)
            error("ERROR reading from socket");

        printf("Client : %s\n", buffer);
        bzero(buffer, 256);
        printf("Server: ");
        fgets(buffer, 256, stdin);
        n = write(newsockfd, buffer, strlen(buffer));
        buffer[strcspn(buffer, "\n")] = 0;
        check_exit(buffer, &newsockfd, &sockfd);
        if (n < 0)
            error("ERROR writing to socket");
    } /* end of while */


    return 0; /* we never get here */
}
