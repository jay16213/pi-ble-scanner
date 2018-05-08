#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include <netdb.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/select.h>

#define SA struct sockaddr
#define MAXLEN 1024



struct pi
{
    int socket;
    int cur_people;
    char LogFile[40];
    FILE *file;
};

int main(int argc,char**argv)
{
    int i;
    int ser_port;
    int maxfd;
    int maxindex;
    int s_lisfd;
    int s_confd;
    int check_flag;
    struct pi client[FD_SETSIZE];
    //int pi[FD_SETSIZE];
    char recevBuffer[MAXLEN];
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    fd_set rset,iniset;
    if(argc !=2)
    {
        fprintf(stderr,"Useage: ./C_socketServer <port>\n");
        exit(-1);
    }
    ser_port=atoi(argv[1]);
    s_lisfd=socket(AF_INET,SOCK_STREAM,0);
    if(s_lisfd<0)
    {
        fprintf(stderr,"Bulid listening socket error\n");
        exit(-1);
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(ser_port);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

    if(bind(s_lisfd,(SA*)&servaddr,sizeof(servaddr))==-1)
    {
        fprintf(stderr,"Bind socket error\n");
        exit(-1);
    }

    if(listen(s_lisfd,10)==-1)
    {
        fprintf(stderr,"Listen error\n");
        exit(-1);
    }

    maxfd=s_lisfd;
    maxindex=-1;
    for(i=0;i<FD_SETSIZE;++i)client[i].socket=-1;
    FD_ZERO(&iniset);
    FD_SET(s_lisfd,&iniset);

    for(;;)
    {
        int nready;
        rset=iniset;
        if((nready=select(maxfd+1,&rset,NULL,NULL,NULL))<0)
        {
            fprintf(stderr,"select wrong\n");
            exit(-1);
        }

        if(FD_ISSET(s_lisfd,&rset))
        {
            socklen_t clilen=sizeof(cliaddr);
            if((s_confd=accept(s_lisfd,(SA*)&cliaddr,&clilen))==-1)
            {
                fprintf(stderr,"accept wrong\n");
                exit(-1);
            }
            for(i=0;i<FD_SETSIZE;++i)
            {
                char *str;
                int PIid;
                time_t rawtime;
                client[i].socket=s_confd;
                printf("One pi connect to server!\n");
                read(client[i].socket,recevBuffer,MAXLEN);
                str=strtok(recevBuffer," \n");
                PIid=atoi(str);
                snprintf(client[i].LogFile,40,"%d_beacon_scannerlog.txt",PIid);
                client[i].file=fopen(client[i].LogFile,"w");
                time (&rawtime);
                fprintf(client[i].file,"Connect at %s", ctime (&rawtime));
                fclose(client[i].file);
                break;
            }
            if(i==FD_SETSIZE)
            {
                fprintf(stderr,"too many clients\n");
                exit(-1);
            }
            FD_SET(s_confd,&iniset);
            if(s_confd>maxfd)maxfd=s_confd;
            if(i>maxindex)maxindex=i;
            if(--nready<=0)continue;
        }
        int sockfd;
        for(i=0;i<=maxindex;++i)
        {
            if((sockfd=client[i].socket)<0)continue;
            if(FD_ISSET(sockfd,&rset))
            {
                ssize_t n;
                if((n=read(sockfd,recevBuffer,MAXLEN))==0)
                {
                    printf("leave\n");
                    client[i].socket=-1;
                    if(i==maxindex)
                    {
                        int j;
                        for(j=maxindex;j>=0;--j)
                        {
                            if(client[j].socket>0)
                            {
                                maxindex=j;
                            }
                        }
                        if(j==0)maxindex=-1;
                    }
                    close(sockfd);
                    //fclose(client[i].file);
                    FD_CLR(sockfd,&iniset);
                }
                else
                {
                    char *str;
                    time_t rawtime;
                    client[i].file=fopen(client[i].LogFile,"a");
                    recevBuffer[n]=0;
                    str=strtok(recevBuffer," \n");
                    str=strtok(NULL," \n");
                    time (&rawtime);
                    fprintf(client[i].file,"Current %d people at %s",atoi(str),ctime (&rawtime));
                    fclose(client[i].file);
                }
                if(--nready<=0)break;
            }

        }
        
    }

}