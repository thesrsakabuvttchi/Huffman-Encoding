#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <math.h>
#include <time.h>

void encode(int chars[256],char message[1024],char *encoded)
{
    int numChar=0;
    for(int i=0;i<256;i++)
        chars[i]=-1;

    for(int i=0;i<strlen(message);i++)
    {
        if(chars[(int)message[i]]==-1)
        {
            chars[(int)message[i]] = numChar;
            numChar++;
        }
    }

    int numBits=0;
    while (pow(2,numBits)<numChar)
        numBits++;

    char bits[numBits*(strlen(message)) + 1];
    for(int i=0;i<strlen(message);i++)
    {
        int conv=chars[(int)message[i]];
        for(int j=0;j<numBits;j++)
        {
            bits[numBits*i+(numBits-1-j)] = conv%2 ? '1' : '0';
            conv/=2;
        }
    }
    bits[numBits*(strlen(message))] = '\0';
    strcpy(encoded,bits);

}

int main()
{
    int sd,n,seconds;
    char buffer[1024] ="",ack;
    FILE *f1 = fopen("in.txt","r");

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd==-1)
        return(printf("Error socket not opened!\n"));

    struct sockaddr_in Server;
    Server.sin_family = AF_INET;
    Server.sin_port = htons(8080);
    Server.sin_addr.s_addr = inet_addr("0");

    if(connect(sd,(const struct sockaddr*)&Server,sizeof(Server)))
        return(printf("Unable to connect to server!\n"));


    char message[1024];
    fgets(message,1024,f1);
    printf("Message to send: %s",message);
    message[strlen(message)-1] = '\0';
    int Chars[256]={0};
    char encoded[8*strlen(message)];
    encode(Chars,message,encoded);

    write(sd,&encoded,sizeof(encoded));
    usleep(100);
    write(sd,&Chars,sizeof(Chars));
    close(sd);    
}