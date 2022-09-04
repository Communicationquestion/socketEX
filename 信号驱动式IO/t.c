#include<stdio.h>
#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

static volatile sig_atomic_t gotSigio = 0;
static void handler(int sig){gotSigio = 1;}
int main (int argc, char *argv[]){
		

}


int main(){



return 0;
}
