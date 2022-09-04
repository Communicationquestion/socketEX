#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
char buf[200000];
int main(){
	int ntowrite, nwrite;
	char *ptr;
	ntowrite=read(STDIN_FILENO,buf,sizeof(buf));
	fprintf(stderr,"read %d bytes\n",ntowrite);
	fcntl(STDOUT_FILENO,F_SETFL,fcntl(STDOUT_FILENO,F_GETFL,0)| O_NONBLOCK);
	ptr = buf;
	while(ntowrite>0){
		errno = 0;
		nwrite=write(STDOUT_FILENO,ptr,ntowrite);
		fprintf(stderr,"nwirte=%d,errno=%d,err: %s\n",nwrite,errno,strerror(errno));
		if(nwrite>0){
			ptr+=nwrite;
			ntowrite-=nwrite;

		}
	}
	int oldfl;
	oldfl=fcntl(STDOUT_FILENO,F_GETFL);
	if(oldfl==-1){
		printf("oldfl failed");

	}
	fcntl(STDOUT_FILENO,F_SETFL,oldfl & ~O_NONBLOCK);

}
