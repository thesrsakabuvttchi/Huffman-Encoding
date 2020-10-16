#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <math.h>
#include <time.h>

struct Node
{
    char name[256];
    struct Node *left,*right;
    int count;
};

void Sort(struct Node queue[],int charNum)
{
    int min;

    for(int i=0;i<charNum;i++)
    {
        min=i;
        for(int j=i+1;j<charNum;j++)
        {
            if(queue[j].count<queue[min].count)
                min = j;
        }
        struct Node tmp;
        tmp.count = queue[i].count;
        strcpy(tmp.name,queue[i].name);
        tmp.left = queue[i].left;
        tmp.right = queue[i].right;

        queue[i].count = queue[min].count;
        strcpy(queue[i].name,queue[min].name);
        queue[i].right = queue[min].right;
        queue[i].left = queue[min].left;

        queue[min].count = tmp.count;
        strcpy(queue[min].name,tmp.name); 
        queue[min].left = tmp.left;
        queue[min].right = tmp.right;
    }
}

void dequeue(struct Node queue[],int charNum)
{
    for(int i=1;i<charNum;i++)
    {
        strcpy(queue[i-1].name,queue[i].name);
        queue[i-1].count = queue[i].count;
        queue[i-1].left = queue[i].left;
        queue[i-1].right = queue[i].right;
    }
}

void cpqueue(struct Node queue[],struct Node queue2[],int charNum)
{
    for(int i=0;i<charNum;i++)
    {
        strcpy(queue2[i].name,queue[i].name);
        queue2[i].count = queue[i].count;
        queue2[i].left = queue[i].left;
        queue2[i].right = queue[i].right;
    }
}

void Tree_traverse(struct Node *n, char codes[256][8],char prev[])
{
    char left[8],right[8];
    strcpy(left,prev);
    strcpy(right,prev);
    strcat(left,"0");
    strcat(right,"1");

    if(n->left!= NULL)
        Tree_traverse(n->left,codes,left);

    if(strlen(n->name)==1)
    {
        strcpy(codes[(int)n->name[0]],prev);
    }      

    if(n->right!= NULL)
        Tree_traverse(n->right,codes,right);

}

void encode(char message[],char encoded[],char codes[256][8])
{
    strcpy(encoded,"");
    struct Node queue[512],queue2[256];
    int charNum=0;
    int Chars[256]={0};

    for(int i=0;i<strlen(message);i++)
        Chars[(int)message[i]]++;

    for(int i=0;i<256;i++)
    {
        strcpy(codes[i],"");
    }

    for(int i=0;i<256;i++)
    {
        if(Chars[i]>0)
        {
            queue[charNum].count = Chars[i];
            queue[charNum].name[0] = (char)i;
            queue[charNum].name[1] = '\0';
            queue[charNum].left=queue[charNum].right=NULL;
            charNum++;
        }
    }

    int ct=0,charNum2=charNum;
    cpqueue(queue,queue2,charNum);

    while(charNum2>1)
    {
        ct++;
        struct Node *tmp1,*tmp2,inode;
        tmp1 = (struct Node*)malloc(sizeof(struct Node));
        tmp2 = (struct Node*)malloc(sizeof(struct Node));

        tmp1->count = queue2[0].count;
        strcpy(tmp1->name,queue2[0].name);
        tmp1->left = queue2[0].left;
        tmp1->right = queue2[0].right;

        tmp2->count = queue2[1].count;
        strcpy(tmp2->name,queue2[1].name);
        tmp2->left = queue2[1].left;
        tmp2->right = queue2[1].right;

        dequeue(queue2,charNum2--);
        dequeue(queue2,charNum2--);
    
        char num[5];
        sprintf(num,"%d",ct);
        strcat(num,"inode");
        strcpy(inode.name,num);
        inode.count = tmp1->count+tmp2->count;
        inode.left  = tmp1;
        inode.right = tmp2;

        queue2[charNum2].count = inode.count;
        queue2[charNum2].left = inode.left;
        queue2[charNum2].right = inode.right;
        strcpy(queue2[charNum2].name,inode.name);
        charNum2++;

        Sort(queue2,charNum2);

        queue[charNum].count = inode.count;
        queue[charNum].left = inode.left;
        queue[charNum].right = inode.right;
        strcpy(queue[charNum].name,inode.name);
        charNum++;
    }
    
    char prev[8]="";
    Tree_traverse(queue2,codes,prev);

    for(int i=0;i<strlen(message);i++)
    {
        strcat(encoded,codes[message[i]]);
    }
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
    printf("message to send: %s",message);
    message[strlen(message)-1] = '\0';
    char Chars[256][8];
    char encoded[8*strlen(message)];
    encode(message,encoded,Chars);
    write(sd,&encoded,sizeof(encoded));
    usleep(100);
    write(sd,&Chars,sizeof(Chars));
    close(sd);    
}


