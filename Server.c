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
    const char* invalidCommand = " is not a valid command\n";
    socklen_t clilen;
    char buffer[256];
<<<<<<< HEAD
    char password[] = "a";
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int offStatus = 1;
=======
    char password[] = "a\n";
    char badCommand_substring[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n, badCommandLength, badCommandLengthMinus, newStringLength;
    int offStatus = 1, runEnd = 1;
    int i_counter;
    int sockfd, newsockfd, portno;  
    
>>>>>>> 8bd42c3e9af73438506a24921762916a05d855a9
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
<<<<<<< HEAD
    *code for the inmpementation of the project
=======
    *code for the implementation of the project
>>>>>>> 8bd42c3e9af73438506a24921762916a05d855a9
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
<<<<<<< HEAD
    n = read(newsockfd, buffer, 1);
    printf("%s\n", buffer);
    if (strcmp(password, buffer) != 0 )
=======
    n = read(newsockfd, buffer, 2);
    printf("%s\n", buffer);
    if (strncmp(password, buffer, 2) != 0 )
>>>>>>> 8bd42c3e9af73438506a24921762916a05d855a9
    {
        printf("%d\n", strcmp(password,buffer));
        error("Error: incorrect password");
        offStatus = 0;
    }
<<<<<<< HEAD
    n = write(newsockfd, "Welcome to this backdoor", 26);

    do {

        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
        n = write(newsockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");
=======
    n = write(newsockfd, "Welcome to this backdoor\n", 26);
    do {
        n = write(newsockfd, ">", 1);
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        //find the command
        if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
        //n = write(newsockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");
        
        if (strncmp(buffer, "pwd\n", 4) == 0) {
            
        } else if (strncmp(buffer, "cd ", 3) == 0) {
            //if things exist, con't
        } else if (strncmp(buffer, "ls\n", 3) == 0) {
            
        } else if (strncmp(buffer, "cat ", 3) == 0) {
            //if things exist, con't
        } else if (strncmp(buffer, "help\n", 5) == 0) {
            n = write(newsockfd, "pwd \t\t returns current working directory\n", 41);
            n = write(newsockfd, "cd <dir> \t changes current working directory to <dir>\n",54);
            n = write(newsockfd, "ls \t\t lists the contents of the current working directory\n", 58);
            n = write(newsockfd, "cat <file> \t returns contents of the file\n", 42);
            n = write(newsockfd, "help \t\t prints a list of commands\n", 34);
            n = write(newsockfd, "browser \t opens up Mozilla Firefox\n", 36);
            n = write(newsockfd, "beep \t\t makes computer beep\n", 28);
            n = write(newsockfd, "off \t\t terminates the program\n", 30);
        } else if (strncmp(buffer, "browser\n", 8) == 0) {
            
        } else if (strncmp(buffer, "beep\n", 5) == 0) {
        
        } else if (strncmp(buffer, "off\n", 4) == 0) {
            n = write(newsockfd, "You killed me...\n", 17);
            offStatus = 0;
        } else {
            badCommandLength = strlen(buffer);
            badCommandLengthMinus = badCommandLength-1;
            strncpy(badCommand_substring, buffer, badCommandLengthMinus); 
            strcat(badCommand_substring, invalidCommand);
            newStringLength = strlen(badCommand_substring);
            n = write(newsockfd, badCommand_substring, newStringLength);
            memset(badCommand_substring, 0, sizeof(badCommand_substring));
        }
        
>>>>>>> 8bd42c3e9af73438506a24921762916a05d855a9
    }while (offStatus == 1);
    
    close(newsockfd);
    close(sockfd);
    return 0; 
}
