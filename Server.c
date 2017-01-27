#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
*code provided by the professor
*/

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    char password[] = "a";
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int offStatus = 1;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
            (struct sockaddr *) &cli_addr, 
            &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    
    /*
    *code for the inmpementation of the project
    *over all loop
    */
    /*
    *send a promote for password
    *check for password
    *check the incoming message for commands
    */

    n = write(newsockfd, "password: ", 10);
    bzero(buffer, 256);
    if(n<0) offStatus = 0;
    n = read(newsockfd, buffer, 1);
    printf("%s\n", buffer);
    if (strcmp(password, buffer) != 0 )
    {
        printf("%d\n", strcmp(password,buffer));
        error("Error: incorrect password");
        offStatus = 0;
    }
    n = write(newsockfd, "Welcome to this backdoor", 26);

    do {

        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");
    }while (offStatus == 1);
    
    close(newsockfd);
    close(sockfd);
    return 0; 
}
