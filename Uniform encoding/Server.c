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

void decode(int Chars[256],char encoded[],char message[])
{
    int max=-1,numBits=0,tmp;
    for(int i=0;i<256;i++)
        if(Chars[i]>max)
            max=Chars[i];

    char lookup[max+1];
    tmp=max;
    while(tmp>0)
    {
        tmp/=2;
        numBits++;
    }

    for(int i=0;i<256;i++)
        if(Chars[i]>-1)
            lookup[Chars[i]]=(char)i;

    printf("%d\n",numBits);

        for(int i=0;i<strlen(encoded)/numBits;i++)
    {
        int num=0;
        for(int j=numBits*i;j<numBits*(i+1);j++)
        {
            num*=2;
            num+= encoded[j]=='0'? 0 : 1;
        }
        message[i] = lookup[num];
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
    int Chars[256];

    read(conn_sd,&encoded,sizeof(encoded));
    read(conn_sd,&Chars,sizeof(Chars));

    decode(Chars,encoded,buffer);

    printf("ENCODED MESSAGE: %s\nDECODED MESSAGE: %s\n",encoded,buffer);

    close(sd);
    close(conn_sd);
}
