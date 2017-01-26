#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>

int main()
{
    int portnumber = 5000;
    char buffer[1024];
    int listen_fd, comm_fd;
    struct sokaddr_in servaddr, cliaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(portnumber);

    bzero(&servaddr, sizeof(servaddr *) &servaddr, sizeof(servaddr));

    listen(listen_fd, 5);

    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    while(1)
    {
        bzero(buffer, 1024);
        read(comm_fd, buffer, 1024);
        write(comm_fd, buffer, strlen(buffer)+1)
    }

}