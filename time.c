##server


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

#define PORT 9000

int main()
{
    int sockfd;

    char buffer[100];

    struct sockaddr_in server,client;

    socklen_t len=sizeof(client);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(PORT);

    bind(sockfd,(struct sockaddr *)&server,sizeof(server));

    printf("=====================================\n");
    printf(" UDP Time Server Started\n");
    printf(" Listening on Port %d\n",PORT);
    printf("=====================================\n");

    while(1)
    {
        recvfrom(sockfd,
                 buffer,
                 sizeof(buffer),
                 0,
                 (struct sockaddr *)&client,
                 &len);

        time_t currentTime;

        time(&currentTime);

        char *timeString=ctime(&currentTime);

        sendto(sockfd,
               timeString,
               strlen(timeString)+1,
               0,
               (struct sockaddr *)&client,
               len);

        printf("Time request served.\n");
    }

    close(sockfd);

    return 0;
}

##client 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 9000

int main()
{
    int sockfd;

    char buffer[100];

    struct sockaddr_in server;

    socklen_t len=sizeof(server);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    strcpy(buffer,"TIME");

    sendto(sockfd,
           buffer,
           strlen(buffer)+1,
           0,
           (struct sockaddr *)&server,
           len);

    recvfrom(sockfd,
             buffer,
             sizeof(buffer),
             0,
             NULL,
             NULL);

    printf("\nCurrent Server Time\n");
    printf("%s\n",buffer);

    close(sockfd);

    return 0;
}
