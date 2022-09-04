
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>

#define GET_REQUEST "GET / HTTP/1.0\r\n\r\n"

char *getinfo(){
	printf("输入网址：");
	char *str;
	char str2[100];
	scanf("%s",str2);

	str=str2;
	printf("%s\n",str); return str;
}
int  socketf(){
	struct addrinfo *res;
	struct addrinfo hints;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = PF_UNSPEC;	
	hints.ai_socktype = SOCK_STREAM;
	if(getaddrinfo(getinfo(),"80",&hints,&res)==-1)
		return -1;
	int s = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	if(connect(s,res->ai_addr,res->ai_addrlen)==-1)
		return -1;
	return s;
}
void printftest(){

	printf("test\n");
}
int sendhttp(){
	char server_use_buf[BUFSIZ];
	printftest();
        int sd=socketf();	
	int len =sprintf (server_use_buf,GET_REQUEST);
	write(sd,GET_REQUEST,len);
	
return sd;
}

int getweb(){
int ad=	sendhttp();
char server_use_buf1[BUFSIZ];
int len1 = sizeof(server_use_buf1)-1;
read(ad,server_use_buf1,len1);
printf("%s\n",server_use_buf1);
return 0;
}

int main(){
//	char *str1=getinfo();
 //      printf("%s",str1);
//socketf();
getweb();
}

