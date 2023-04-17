#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
void xoaXuongDongC(char *str) {
    //Lấy độ dài chuỗi str 
    size_t len = strlen(str);
   
    //Tạo vòng lặp và kiểm tra từng ký tự trong chuỗi
    int i;
    for (i = 0; i <= len; i = i + 1){
         if(str[i] == '\n') {  
          str[i] = ' '; //Thay thế ký tự xuống dòng bằng dấu cách nếu tìm thấy.
        }     
    }
}
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

    while (1)
    {
        int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listener == -1)
        {
            perror("socket() failed");
            return 1;
        }

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(atoi(argv[1]));

        if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)))
        {
            perror("bind() failed");
            return 1;
        }

        if (listen(listener, 5))
        {
            perror("listen() failed");
            return 1;
        }

        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(addr);

        int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
        printf("Client IP: %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        char hello[10], feedback[256];
        
        char buf[256];

        char *msg = "Hello client\n";
        send(client, msg, strlen(msg), 0);

        FILE *w =  fopen(argv[2], "a+"); 
        struct infor clientinfo;
        int ret = recv(client, &clientinfo, sizeof(clientinfo), 0);
        printf("mssv: %d\n",clientinfo.mssv);
        printf("\nHo va ten: ");
        puts(clientinfo.fullname);
        printf("Ngay sinh: ");
        puts(clientinfo.date);
        printf("Diem trung binh: %0.2f", clientinfo.cpa);
        
        xoaXuongDongC(clientinfo.fullname);
        xoaXuongDongC(clientinfo.date);
        
        fprintf(w, "%s %d-%02d-%02d %02d:%02d:%02d %d %s%s%0.2f\r\n", 
        inet_ntoa(clientAddr.sin_addr),
        clientinfo.tsend.tm_year + 1900, 
        clientinfo.tsend.tm_mon + 1, 
        clientinfo.tsend.tm_mday, 
        clientinfo.tsend.tm_hour, 
        clientinfo.tsend.tm_min, 
        clientinfo.tsend.tm_sec,
        clientinfo.mssv,
        clientinfo.fullname,
        clientinfo.date,
        clientinfo.cpa
        );

        fclose(w);
        while (1)
        {
            int ret = recv(client, buf, sizeof(buf), 0);
            
            if (ret <= 0)
            {
                printf("\nClient IP: %s:%d ngat ket noi.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                break;    
            }
        }

        close(client);
        close(listener);
    }
    
}