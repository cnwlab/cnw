##server

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 9000
#define BUFFER_SIZE 2048

int main()
{
    int sockfd;

    struct sockaddr_in server,client;

    socklen_t len=sizeof(client);

    char filename[100];

    char buffer[BUFFER_SIZE];

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(PORT);

    bind(sockfd,(struct sockaddr *)&server,sizeof(server));

    printf("====================================\n");
    printf(" UDP Concurrent File Server Started\n");
    printf(" Listening on Port %d\n",PORT);
    printf("====================================\n");

    while(1)
    {
        memset(filename,0,sizeof(filename));

        recvfrom(sockfd,
                 filename,
                 sizeof(filename),
                 0,
                 (struct sockaddr *)&client,
                 &len);

        if(fork()==0)
        {
            FILE *fp;

            fp=fopen(filename,"r");

            memset(buffer,0,sizeof(buffer));

            sprintf(buffer,
                    "Handled by Server PID : %d\n\n",
                    getpid());

            if(fp==NULL)
            {
                strcat(buffer,
                       "Requested File Not Found.\n");
            }
            else
            {
                char line[256];

                while(fgets(line,sizeof(line),fp)!=NULL)
                {
                    strcat(buffer,line);
                }

                fclose(fp);
            }

            sendto(sockfd,
                   buffer,
                   strlen(buffer)+1,
                   0,
                   (struct sockaddr *)&client,
                   len);

            printf("Request for %s served by PID %d\n",
                    filename,
                    getpid());

            exit(0);
        }
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
#define BUFFER_SIZE 2048

int main()
{
    int sockfd;

    struct sockaddr_in server;

    socklen_t len=sizeof(server);

    char filename[100];

    char buffer[BUFFER_SIZE];


    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd<0)
    {
        printf("Socket Creation Failed\n");
        return 0;
    }


    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    printf("Enter File Name : ");

    scanf("%s",filename);

    sendto(sockfd,
           filename,
           strlen(filename)+1,
           0,
           (struct sockaddr *)&server,
           len);

    recvfrom(sockfd,
             buffer,
             sizeof(buffer),
             0,
             NULL,
             NULL);

    printf("\n=====================================\n");
    printf("Response Received From Server\n");
    printf("=====================================\n\n");

    printf("%s\n",buffer);

    close(sockfd);

    return 0;
}
