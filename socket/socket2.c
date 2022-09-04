#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
int main()
{
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    char bufSend[BUFSIZ] = {0};
    char bufRecv[BUFSIZ] = {0};
    while (1)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        {
            return -1;
        }
        printf("Input a string: ");
        fgets(bufSend, BUFSIZ, stdin);
        send(sock, bufSend, strlen(bufSend), 0);
        recv(sock, bufRecv, BUFSIZ, 0);
        printf("Message form server: %s\n", bufRecv);
        memset(bufSend, 0, BUFSIZ);
        memset(bufRecv, 0, BUFSIZ);
        close(sock);
    }
    return 0;
}
