#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
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
    FILE *f = fopen(argv[2], "a+");
    FILE *w =  fopen(argv[3], "a+"); 
    fseek(f, SEEK_SET, 0);
    fread(hello, sizeof(hello), 1, f);
    send(client, hello, strlen(hello),0);
    
    char buf[256];

    while (1)
    {
       
        
        int ret = recv(client, buf, sizeof(buf), 0);
         if (ret < sizeof(buf))
            buf[ret] = 0;
        
        
        if (ret <= 0)
        {
            printf("Ket noi bi dong.\n");
            break;    
        }
        
         if (strncmp(buf, "exit", 4) != 0) {
            fwrite(buf, 1, ret, w);
         }
            
    

    }
    
    close(client);
    close(listener);

    
}