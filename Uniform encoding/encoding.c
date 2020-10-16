#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

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
    char message[1024];
    fgets(message,1024,stdin);
    message[strlen(message)-1] = '\0';
    int Chars[256]={0};
    char encoded[8*strlen(message)];
    encode(Chars,message,encoded);
    printf("Encoded as: %s\n",encoded);

    strcpy(message,"");
    decode(Chars,encoded,message);
    printf("Decoded as: %s\n",message);
}