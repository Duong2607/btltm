#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>



struct infor 
{
    int mssv;
    char fullname[100];
    char date[11];
    float cpa;
    struct tm tsend;
};


int main(int argc, char *argv[])
{
 
    
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int valread, sock=0;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));


    if(inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 


    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1)
    {
        perror("connect() failed");
        return 1;
    }

    char buf[256];
    char re[256];
    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0)
    {
        printf("Connection closed\n");
        return 1;
    }

    if (ret < sizeof(buf))
        buf[ret] = 0;
    printf("%d bytes received: %s\n", ret, buf);

   
        struct infor clientinfo;
        
        
        printf("Nhap MSSV: ");
        scanf("%d",&clientinfo.mssv);
        getchar();
        printf("Nhap Ho va ten: ");
        fgets(clientinfo.fullname, sizeof(clientinfo.fullname)+1, stdin);
        printf("Nhap ngay sinh: ");
        fgets(clientinfo.date, sizeof(clientinfo.date)+1, stdin);
        printf("Nhap diem trung binh: ");
        scanf("%f", &clientinfo.cpa);
        getchar();
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        clientinfo.tsend = tm;


        send(client, &clientinfo, sizeof(clientinfo), 0);

    while (1)
    {
        printf("Gui den server:");
        fgets(buf, sizeof(buf), stdin);

        send(client, buf, strlen(buf), 0);
   
        if (strncmp(buf, "exit", 4) == 0)
            break;
    }

    close(client);
}


