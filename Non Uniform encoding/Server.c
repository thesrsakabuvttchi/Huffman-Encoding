#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/time.h> 
#include <string.h> 
#include <math.h>
#include<time.h>


void decode(char encoded[],char message[],char codes[256][8])
{
    strcpy(message,"\0");
    char tmp[strlen(encoded)];
    strcpy(tmp,"\0");
    printf("\n");
    for(int i=0;i<strlen(encoded);i++)
    {
        strncat(tmp,&encoded[i],1);
        for(int j=0;j<256;j++)
        {
            if(strcmp(codes[j],tmp)==0)
            {
                char ch[2] = {(char)j,'\0'};
                strcat(message,ch);
                strcpy(tmp,"");
                break;
            }
        }
    }
}

int main()
{
    int sd,conn_sd,num_pack=0;
    struct timeval start,end;
    double tmp=0.0;
    char buffer[1024] = "",encoded[8*1024];  
    FILE *f1;

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd==-1)
        return(printf("Socket creation unsucessfull\n"));

    
    struct sockaddr_in Server,Client;
    memset(&Server,0,sizeof(Server));

    Server.sin_family = AF_INET;
    Server.sin_port = htons(8080);
    Server.sin_addr.s_addr = inet_addr("0");

    if(bind(sd,(const struct sockaddr*)&Server,sizeof(Server)))
        return(printf("Could not bind!\n"));

    if(listen(sd,5))
        return(printf("Too many connections!!!"));

    socklen_t cli_len = sizeof(Client);
    conn_sd = accept(sd,(struct sockaddr*)&Client,&cli_len);

    if(conn_sd < 1)
        return(printf("Error, can't accept connection!"));

    int n;
    char Chars[256][8];

    read(conn_sd,&encoded,sizeof(encoded));
    read(conn_sd,&Chars,sizeof(Chars));

    decode(encoded,buffer,Chars);

    printf("ENCODED MESSAGE: %s\nDECODED MESSAGE: %s\n",encoded,buffer);

    close(sd);
    close(conn_sd);
}